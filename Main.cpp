#include <iostream>
#include <ctime>
#include <fstream>
#include "Main.h"
#include "Utils.h"
#include "SDL.h"


SDL_Window* g_Window = nullptr;
SDL_Surface* g_ScreenSurface = nullptr;
SDL_Surface* g_BackGround = nullptr;
SDL_Renderer* g_GameRenderer = nullptr;
TTF_Font* g_Font = nullptr;
Mix_Music* g_Music = nullptr;
Mix_Chunk* g_SoundFXGun = nullptr;
Vector2D g_MousePosition = Vector2D();
Texture2D g_MenuTexture;
Texture2D g_magicTex;
Texture2D g_playerHealthBar;
Texture2D g_healthBarFrame;
SDL_Event g_event;

/*Game object pool*/
vector<Projectile*> ProjectileGroup(MAX_PROJECTILE, nullptr);
vector<Texture2D> ProjectileTextures(MAX_PROJECTILE_TYPE, Texture2D());
vector<Enemy*> EnemyGroup(MAX_NPC_TYPE, nullptr);
vector<Texture2D> EnemyTexture(MAX_NPC_TYPE, Texture2D());
vector<Dust*> DustsGroup(MAX_DUSTS, nullptr);
vector<Texture2D> DustsTexture(MAX_DUSTS_TYPE, Texture2D());
Player player;
bool quit = false;
bool isMenu = true;
bool isMenuFade = false;
bool isGameover = true;
bool isPaused = false;
LONG64 g_totalTime = 0;
Uint8 menu_alpha = 255;
_UUID g_projID = 0;
_UUID g_npcID = 0;
_UUID projTexID = 0;
_UUID g_dustID = 0;
Uint64 elaspTime = 0;
SCORE g_playerScore = 0;
SpawnValue g_spawnRate = 600;
SpawnValue g_spawnNumber = 1;
SpeedValue g_horizontalSpeed = 1.2;
SpeedValue g_verticalSpeed = 1.2;
UIText FPSText;
UIText PlayerHealthText;
UIText ScoreText;
GUIButton pointButton;
GUIButton exitButton;

int main(int argc, char** args)
{
	//Initializaion check!
	if (!Game_init()) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Check game contect condition
		if (!Game_loadContent()) {
			printf("SDL could not load game content! SDL_Error: %s\n", SDL_GetError());
		}
		else{
			Game_main();
		}
	}
	Game_close();
	Sleep(2000);
	return 0;
}




void Game_main()
{
	//While application is running
	while (!quit)
	{
		//FPS counting
		char fpstxt[25];
		sprintf_s(fpstxt, "FPS: %I64d", 1000 / ((clock() - elaspTime) > 0 ? (clock() - elaspTime) : 1));
		SDL_Rect fpsArea{ 2,2,65,20 };

		elaspTime = clock();

		Game_Event();

		//If not in menu mode then draw game elements
		if (!isMenu) 
		{
			if (!isPaused) 
			{
				g_totalTime++;
				if (g_totalTime % g_spawnRate < 1) {
					for (SpawnValue i = 0; i < g_spawnNumber; i++) {
						SpawnEnemy({ 20.0 + rand() % 760 , 20.0 + rand() % 560 }, { 0, 0 }, rand() % 2);
					}
				}
				//Renderer
				SDL_RenderClear(g_GameRenderer);
				if (player.active) {
					//Update
					player.Update();
					player.Draw();
				}
				if (isGameover) {
					isMenu = true;
					menu_alpha = 255;
					isMenuFade = false;
					ofstream fout("Asset/document.dat", ios::binary);
					if (fout.is_open()) {
						char buffer[256];
						memcpy(buffer, &g_playerScore, sizeof(SCORE));
						fout.write(buffer, sizeof(SCORE));
						fout.close();
					}
					else {
						cout << "Cannot open data file!!" << endl;
					}
				}
				//Draw enemies
				for (auto& enemy : EnemyGroup)
				{
					if (enemy && enemy->active)
					{
						enemy->Update();
						enemy->Draw();
					}
				}
				//Draw projectiles
				for (auto& proj : ProjectileGroup) {
					if (proj && proj->active) {
						proj->Update();
						proj->Draw();
					}
				}

				//Draw dusts
				for (auto& dust : DustsGroup) {
					if (dust && dust->active) {
						dust->Update();
						dust->Draw();
					}
				}

				FPSText.SetText(fpstxt);
				FPSText.Render(g_GameRenderer, &fpsArea);
				Game_DrawUI();
				SDL_RenderPresent(g_GameRenderer);
			}
		}
		else {
			//Update
			Game_Menu();

			//Render menu
			SDL_RenderClear(g_GameRenderer);
			g_MenuTexture.setAlpha(menu_alpha);
			SDL_RenderCopyEx(g_GameRenderer, g_MenuTexture.GetPtr(), nullptr, nullptr, 0, &SDL_Point(), SDL_FLIP_NONE);
			pointButton.Render(g_GameRenderer);
			exitButton.Render(g_GameRenderer);
			SDL_RenderPresent(g_GameRenderer);
		}
		while (clock() - elaspTime < 16);
	}
	//Limit FPS

}

bool Game_init()
{
	//Success flag
	auto success = true;
	srand(unsigned(time));
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		return false;
	}
	else
	{
		//Create window
		g_Window = SDL_CreateWindow("FighterPlane", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (g_Window == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Start renderer
			g_GameRenderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
			if (g_GameRenderer == nullptr)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color to white
				SDL_SetRenderDrawColor(g_GameRenderer, 255, 225, 0, 255);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return false;
				}
				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					return false;
				}
				//Dangerous! Pointer may be null!!
				for (_UUID i = 0; i < ProjectileGroup.size(); i++)
				{
					Projectile* newProj = new Projectile();
					ProjectileGroup[i] = newProj;
					ProjectileGroup[i]->UUID = i;
				}
				for (_UUID i = 0; i < EnemyGroup.size(); i++)
				{
					Enemy* newEnemy = new Enemy();
					EnemyGroup[i] = newEnemy;
					EnemyGroup[i]->UUID = i;
				}
				for (_UUID i = 0; i < DustsGroup.size(); i++)
				{
					Dust* newEnemy = new Dust();
					DustsGroup[i] = newEnemy;
					DustsGroup[i]->UUID = i;
				}
				ButtonEvent();
			}
		}
	}
	return success;
}

bool Game_loadContent()
{
	//Success flag
	auto success = true;

	g_Music = Mix_LoadMUS("Asset/Music/StringTek.wav");
	if (g_Music == nullptr) {
		LogMUSErrorMsg("Failed to load music");
		return false;
	}

	Mix_PlayMusic(g_Music, -1);

	g_SoundFXGun = Mix_LoadWAV("Asset/Music/LazerPulse.wav");
	if (g_SoundFXGun == nullptr) {
		LogMUSErrorMsg("Failed to load music");
	}

	//Load font
	g_Font = TTF_OpenFont("Asset/Font/Arial.ttf", 30);
	if (g_Font == nullptr)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	FPSText = UIText(g_Font);
	PlayerHealthText = UIText(g_Font);
	ScoreText = UIText(g_Font);
	pointButton.SetTexture(g_GameRenderer, g_Font, "Game Point");
	exitButton.SetTexture(g_GameRenderer, g_Font, "Quit Game");
	player.playerTex.LoadTexFromFile(g_GameRenderer, "Asset/Player2.png");
	g_MenuTexture.LoadTexFromFile(g_GameRenderer, "Asset/Space.jpg");
	g_MenuTexture.setBlendMode(SDL_BLENDMODE_BLEND);
	g_magicTex.LoadTexFromFile(g_GameRenderer, "Asset/Test.png");
	g_playerHealthBar.LoadTexFromFile(g_GameRenderer, "Asset/UI/UI_Bar.png");
	g_healthBarFrame.LoadTexFromFile(g_GameRenderer, "Asset/UI/UI_BarFrame.png");

	//Load textures array
	//TODO: Add texture list according to projectiles
	addTextureToArray<Texture2D>(ProjectileTextures, "Asset/Projectile/proj0.png", g_projID);
	addTextureToArray<Texture2D>(ProjectileTextures, "Asset/Projectile/proj1.png", g_projID);

	addTextureToArray<Texture2D>(EnemyTexture, "Asset/Enemy/enemy_4.png", g_npcID);
	addTextureToArray<Texture2D>(EnemyTexture, "Asset/Enemy/enemy_1.png", g_npcID);

	addTextureToArray<Texture2D>(DustsTexture, "Asset/Dusts/dust_0.png", g_dustID);

	g_projID = 0;
	g_npcID = 0;
	g_dustID = 0;
	return success;
}


void Game_close()
{
	cout << endl;
	


	//Destroy music
	Mix_FreeMusic(g_Music);
	Mix_FreeChunk(g_SoundFXGun);

	//Destroy all game objects
	ClearVector(ProjectileGroup);
	ClearVector(EnemyGroup);
	ClearVector(DustsGroup);

	ProjectileGroup.clear();
	EnemyGroup.clear();
	DustsGroup.clear();

	//Close font and release resources
	TTF_CloseFont(g_Font);
	g_Font = nullptr;

	//Destroy the window
	SDL_DestroyRenderer(g_GameRenderer);
	SDL_DestroyWindow(g_Window);
	g_GameRenderer = nullptr;
	g_Window = nullptr;

	//Quit SDL
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game_Event()
{
	//Event processing
	while (SDL_PollEvent(&g_event)) {
		if (isMenu) {
			pointButton.Update(g_event);
			exitButton.Update(g_event);
		}
		switch (g_event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			HandleKeyboard();
			break;
		case SDL_KEYUP:
			if (!player.uncontrol)
				player.velocity = Vector2D(0, 0);
			break;
		case SDL_MOUSEMOTION:
			HandleMouse();
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(!isMenu)
			FireProjectile();
			break;
		default:
			break;
		}
	}
}

void Game_DrawUI()
{
	//g_playerHealthBar.SetColor(255, 30, 30);
	g_playerHealthBar.SetColor(30, 255, 30);
	SDL_Rect barPos = { SCREEN_WIDTH - 130, SCREEN_HEIGHT - 40 , static_cast<int>(120 * (player.health / 100.0)), 10 };
	SDL_RenderCopyEx(g_GameRenderer, g_playerHealthBar.GetPtr(), nullptr, &barPos, 0, &SDL_Point(), SDL_FLIP_NONE);
	barPos = { SCREEN_WIDTH - 132, SCREEN_HEIGHT - 42, 124, 14 };
	SDL_RenderCopyEx(g_GameRenderer, g_healthBarFrame.GetPtr(), nullptr, &barPos, 0, &SDL_Point(), SDL_FLIP_NONE);
	char healthTxt[25];
	sprintf_s(healthTxt, "%d / %d", player.health, 100);
	SDL_Rect healthTXTRect{ SCREEN_WIDTH - 100,SCREEN_HEIGHT - 30,60,20 };
	PlayerHealthText.SetText(healthTxt);
	PlayerHealthText.Render(g_GameRenderer, &healthTXTRect);
	char scoretxt[25];
	sprintf_s(scoretxt, "SCORE: %d", g_playerScore);
	int tmp = strlen(scoretxt);
	SDL_Rect ScorePos{ 780 - tmp*10, 0, tmp * 10, 22 };
	ScoreText.SetText(scoretxt);
	ScoreText.Render(g_GameRenderer, &ScorePos);
}

void HandleKeyboard()
{
	switch (g_event.key.keysym.sym)
	{
	case SDLK_w:
		if(!player.uncontrol && !isPaused && !isMenu)
		player.velocity.Y = -g_verticalSpeed;
		break;
	case SDLK_s:
		if(!player.uncontrol && !isPaused && !isMenu)
		player.velocity.Y = g_verticalSpeed;
		break;
	case SDLK_a:
		if(!player.uncontrol && !isPaused && !isMenu)
		player.velocity.X = -g_horizontalSpeed;
		break;
	case SDLK_d:
		if(!player.uncontrol && !isPaused && !isMenu)
		player.velocity.X = g_horizontalSpeed;
		break;
	case SDLK_ESCAPE:
		//quit = true;
		isPaused = !isPaused;
		break;
	case SDLK_SPACE:
		if (isMenu)
			isMenuFade = true;
		break;
	default:
		break;

	}
}

void HandleMouse()
{
	//Set mouse position
	g_MousePosition.X = g_event.motion.x;
	g_MousePosition.Y = g_event.motion.y;
}

void Game_Menu()
{
	//Fade out!
	if (isMenuFade) {
		if (menu_alpha > 0)
			menu_alpha -= 5;
		if (menu_alpha == 0) {
			isMenu = false;
			if (isGameover) {
				isGameover = false;
				ClearAndResetAll();
				if (SpawnEnemy({ 600, 300 }, { 0, 0 }, 1) == -1) {
					printf("Error when spawning enemy. SDL Error: %s", SDL_GetError());
				}
			}
		}
	}
}

void FireProjectile()
{
	if (!player.uncontrol && !isPaused && !isMenu) {
		//Fire a projectile toward mouse position
		auto firePos = player.Center();
		auto fireVel = (g_MousePosition - firePos).Normalize();
		firePos += fireVel * 10;
		fireVel *= 7;
		NewProjectile(firePos, fireVel, 0);
		Mix_PlayChannel(-1, g_SoundFXGun, 0);
	}
}

void ClearAndResetAll()
{
	for (auto n : EnemyGroup) {
		if (n->active)n->active = false;
	}
	for (auto n : ProjectileGroup) {
		if (n->active)n->active = false;
	}
	player.active = true;
	player.health = 100;
	player.position = Vector2D(0, 0);
	player.velocity.Zero();
	player.immuneTime = 0;
	player.rotation = 0;
	player.uncontrol = false;
	g_totalTime = 0;
	g_spawnRate = 600;
	g_spawnNumber = 1;
	//Reset Score
	g_playerScore = 0;
}

void DefaultButton(const SDL_Event& e, SDL_Rect& rect, Texture2D& tex) {
	if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (g_MousePosition.X < rect.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (g_MousePosition.X > rect.x + rect.w)
		{
			inside = false;
		}
		//Mouse above the button
		else if (g_MousePosition.Y < rect.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (g_MousePosition.Y > rect.y + rect.h)
		{
			inside = false;
		}
		if (inside) {
			tex.SetColor(255, 100, 100);
		}
		else {
			tex.SetColor(255, 255, 255);
		}
	}
}

void ButtonEvent()
{
	//Setting button
	pointButton.posBox = { SCREEN_WIDTH/2 - 40, SCREEN_HEIGHT - 150 , 80, 30 };
	pointButton.SetButtonEvent([](const SDL_Event& e, SDL_Rect& rect, Texture2D& tex) {
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (g_MousePosition.X < rect.x)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (g_MousePosition.X > rect.x + rect.w)
			{
				inside = false;
			}
			//Mouse above the button
			else if (g_MousePosition.Y < rect.y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (g_MousePosition.Y > rect.y + rect.h)
			{
				inside = false;
			}
			tex.setAlpha(menu_alpha);
			if (inside) {
				tex.SetColor(255, 100, 100);
				if (e.type == SDL_MOUSEBUTTONUP) {
					ifstream fin("Asset/document.dat", ios::binary);
					if (fin.is_open()) {
						char buffer[256];
						fin.read(buffer, sizeof(int));
						int value;
						memcpy(&value, buffer, sizeof(int));
						cout << value << endl;
					}
					else {
						cout << "Cannot open data file!!" << endl;
					}
				}
			}
			else {
				tex.SetColor(0, 0, 0);
			}
		}
	});

	//Exit game button
	exitButton.posBox = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 100,100,30 };
	exitButton.SetButtonEvent([](const SDL_Event& e, SDL_Rect& rect, Texture2D& tex) {
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (g_MousePosition.X < rect.x)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (g_MousePosition.X > rect.x + rect.w)
			{
				inside = false;
			}
			//Mouse above the button
			else if (g_MousePosition.Y < rect.y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (g_MousePosition.Y > rect.y + rect.h)
			{
				inside = false;
			}
			tex.setAlpha(menu_alpha);
			if (inside) {
				tex.SetColor(255, 100, 100);
				if (e.type == SDL_MOUSEBUTTONUP) {
					quit = true;
				}
			}
			else {
				tex.SetColor(0, 0, 0);
			}
		}
	});

}
