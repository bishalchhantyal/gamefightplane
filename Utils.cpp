
#include "Utils.h"

SDL_Surface * loadSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = nullptr;

	//Load image at this path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, g_ScreenSurface->format, NULL);
		if (optimizedSurface == nullptr)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(g_GameRenderer, loadedSurface);
		if (newTexture == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int NewProjectile(Vector2D pos, Vector2D vec, int type)
{
	if (g_projID < MAX_PROJECTILE) {
		_UUID id = g_projID;
		auto tmpProj = new Projectile(pos, vec);
		SpawnProjSetDefault(tmpProj, type);
		tmpProj->UUID = ProjectileGroup[g_projID]->UUID;
		ProjectileGroup[g_projID] = tmpProj;
		g_projID++;
		return id;
	}
	else
	{
		auto index = find_if(ProjectileGroup.begin(),
			ProjectileGroup.end(), 
			[](const vector<Projectile*>::value_type& i)
				->bool {return !i->active; });
		//Override disactive projectile
		if(index != ProjectileGroup.end())
		{
			auto tmpProj = new Projectile(pos, vec);
			SpawnProjSetDefault(tmpProj, type);
			tmpProj->UUID = (*index)->UUID;
			*index = tmpProj;
			return tmpProj->UUID;
		}
		else
		{
			return -1;
		}
	}
}

int SpawnEnemy(Vector2D pos, Vector2D vec, int type)
{
	if (g_npcID< MAX_NPCS) {
		_UUID id = g_npcID;
		auto tmpEnemy = new Enemy(pos,vec);
		SpawnEnemySetDefault(tmpEnemy, type);
		tmpEnemy->UUID = EnemyGroup[g_npcID]->UUID;
		EnemyGroup[g_npcID] = tmpEnemy;
		g_npcID++;
		return id;
	}
	else
	{
		auto index = find_if(EnemyGroup.begin(),
			EnemyGroup.end(),
			[](const vector<Enemy*>::value_type& i)
			->bool {return !i->active; });
		//Override disactive projectile
		if (index != EnemyGroup.end())
		{
			auto tmpEnemy = new Enemy(pos, vec);
			SpawnEnemySetDefault(tmpEnemy, type);
			tmpEnemy->UUID = (*index)->UUID;
			*index = tmpEnemy;
			return tmpEnemy->UUID;
		}
		else
		{
			return -1;
		}
	}
}

int SpawnDusts(SDL_Rect rect, Vector2D vec, int type, int timeLeft)
{

	if (g_dustID< MAX_DUSTS) {
		_UUID id = g_dustID;
		auto tmpDust = new Dust(rect);
		SpawnDustSetDefault(tmpDust, type, timeLeft);
		tmpDust->UUID = DustsGroup[g_dustID]->UUID;
		DustsGroup[g_dustID] = tmpDust;
		g_dustID++;
		return id;
	}
	else
	{
		auto index = find_if(DustsGroup.begin(),
			DustsGroup.end(),
			[](const vector<Dust*>::value_type& i)
			->bool {return !i->active; });
		//Override disactive projectile
		if (index != DustsGroup.end())
		{
			auto tmpDust = new Dust(rect);
			SpawnDustSetDefault(tmpDust, type, timeLeft);
			tmpDust->UUID = (*index)->UUID;
			*index = tmpDust;
			return tmpDust->UUID;
		}
		else
		{
			return -1;
		}
	}
}

void SpawnEnemySetDefault(Enemy *& enemy, int type)
{
	enemy->active = true;
	enemy->type = type;
	enemy->enemyTex = EnemyTexture[type];
	if (type == 0) {
		enemy->name = "Turret";
		enemy->damage = 5;
		enemy->knockback = 3;
		enemy->health = 120;
		enemy->scoreValue = 10;
	}
	else if (type == 1) {
		enemy->name = "Slayer";
		enemy->damage = 10;
		enemy->knockback = 5;
		enemy->health = 90;
		enemy->scoreValue = 10;
	}
}

void SpawnProjSetDefault(Projectile *& tmpProj, int type)
{
	tmpProj->active = true;
	tmpProj->type = type;
	tmpProj->projTex = ProjectileTextures[type];
	if (type == 0) {
		tmpProj->name = "Std::Bullet";
		tmpProj->damage = 10;
		tmpProj->knockback = 3;
	}
	if (type == 1) {
		tmpProj->name = "Turret Bullet";
		tmpProj->damage = 10;
		tmpProj->hostile = true;
		tmpProj->knockback = 2;
	}
}

void SpawnDustSetDefault(Dust *& dust, int type, int timeLeft)
{
	dust->active = true;
	dust->type = type;
	dust->dustTex = DustsTexture[type];
	dust->timeleft = timeLeft;

}

bool Intersect(const SDL_Rect & r1, const SDL_Rect & r2)
{
	return r1.x + r1.w >= r2.x
		&& r1.x <= r2.x + r2.w
		&& r1.y + r1.h >= r2.y
		&& r1.y <= r2.y + r2.h;
}

void scaleRect(SDL_Rect & rect, double scale)
{
	if (scale == 1.0) {
		return;
	}
	int oldW = rect.w;
	int oldH = rect.h;
	rect.w *= scale;
	rect.h *= scale;
	oldH -= rect.h;
	oldW -= rect.w;
	rect.x += oldW/2;
	rect.y += oldH/2;
}

void LookAt(GameObject * from, GameObject * to, double rot)
{
	Vector2D diff = from->Center() - to->Center();
	from->rotation = diff.GetRotationDegree() + rot;
}
