#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include "Player.h"
#include "Texture2D.h"
#include "Projectile.h"
#include "Enemy.h"
#include "UIText.h"
#include "GUIButton.h"
#include "Dust.h"
#pragma comment(lib, "Winmm.lib")
using namespace std;

/*-------------------------------------Macro declearation---------------------------------------------------*/

#define ALLOW_NESTED_COMMENT /**/0//**/1


#ifdef _MSC_VER       
#if _MSC_VER <=1200    
#else                  
#if _MSC_VER <= 1800  
#include <stdint.h>
#endif
#ifndef __cplusplus    
typedef int bool;
#define true 1
#define false 0
#endif
#endif
#else                 

#include <stdint.h>
#ifndef __cplusplus   

#include <stdbool.h>
#endif
#endif

#define LogErrorMsg(desp) (printf("Error happened on line %d, description: %s Error: %s \n",__LINE__, desp ,SDL_GetError()))
#define LogMUSErrorMsg(desp) (printf("Error happened on line %d, description: %s Error: %s \n",__LINE__, desp ,Mix_GetError()))
//#define DEBUG

using SCORE = uint32_t;
using SpawnValue = uint32_t;
using SpeedValue = double_t;

/*-------------------------------------Constant declearation------------------------------------------------*/

//Screen buff size
const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;

const unsigned MAX_PROJECTILE_TYPE = 128;
const unsigned MAX_PROJECTILE = 128;
const unsigned MAX_NPC_TYPE = 64;
const unsigned MAX_NPCS = 64;
const unsigned MAX_DUSTS = 512;
const unsigned MAX_DUSTS_TYPE = 64;


/*-------------------------------------Variable declearation------------------------------------------------*/

//The window we'll be rendering to
extern SDL_Window* g_Window;

//The surface contained by the window
extern SDL_Surface* g_ScreenSurface;

//Background surface
extern SDL_Surface* g_BackGround;

//Renderer
extern SDL_Renderer* g_GameRenderer;

//Font - Arial
extern TTF_Font* g_Font;

//SDL Event
extern SDL_Event g_event;

//SDL music will play
extern Mix_Music* g_Music;

//The sound effects that will be used
extern Mix_Chunk* g_SoundFXGun;

//Player object
extern Player player;

//Menu texture
extern Texture2D g_MenuTexture;

//Magical texture
extern Texture2D g_magicTex;

//Health bar UI
extern Texture2D g_playerHealthBar;

//Bar frame texture
extern Texture2D g_healthBarFrame;

//Mouse position
extern Vector2D g_MousePosition;

//Exit signal
extern bool quit;

//Is in menu mode
extern bool isMenu;

//Game over
extern bool isGameover;

//Pause the game
extern bool isPaused;

//Time record
extern LONG64 g_totalTime;

//Projectiles array
extern vector<Projectile*> ProjectileGroup;

//Projectiles array
extern vector<Texture2D> ProjectileTextures;

//Projectiles array
extern vector<Enemy*> EnemyGroup;

//Projectiles array
extern vector<Texture2D> EnemyTexture;

//Dusts array
extern vector<Dust*> DustsGroup;

//Dusts texture array
extern vector<Texture2D> DustsTexture;

//Projectile ID counter
extern _UUID g_projID;

//Enemy ID counter
extern _UUID g_npcID;

//Dust ID counter
extern _UUID g_dustID;

//Player's score
extern SCORE g_playerScore;

//Enemy Spawn rate
extern SpawnValue g_spawnRate;

//Enemy spawn number
extern SpawnValue g_spawnNumber;

//Speed multiply of player
extern SpeedValue g_horizontalSpeed;

//Speed multiply of player
extern SpeedValue g_verticalSpeed;




/*-------------------------------------Function declearation------------------------------------------------*/

//Game main loop
void Game_main();

//Starts up function
bool Game_init();

//Load game resources
bool Game_loadContent();

//Destroy game resources and free the memories
void Game_close();

//Event Handle
void Game_Event();

//Draw User Interface
void Game_DrawUI();

//Process keyboard event
void HandleKeyboard();

/**
 * Process mouse movement and mouse position
 */
void HandleMouse();

//Game meun render
void Game_Menu();

//Player fire bullet
void FireProjectile();

//Clear all game object
void ClearAndResetAll();

//Handle Mouse Button Event
void ButtonEvent();

//Add textures to the arraylist
template <typename _T>
void addTextureToArray(vector<_T>& dest, 
	const std::string& str, 
	_UUID& IDcounter)
{
	dest[IDcounter].LoadTexFromFile(g_GameRenderer, str);
	IDcounter++;
}

//Remove resources in the vector
template<typename _T>
void ClearVector(vector<_T>& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		delete vec[i];
	}
}