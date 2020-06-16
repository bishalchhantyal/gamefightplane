#pragma once
#include <SDL.h>
#include "Main.h"
#include "Dust.h"
using namespace std;

//Load image to SDL_Surface
SDL_Surface* loadSurface(std::string path);

//Load image to SDL_Texture
SDL_Texture* loadTexture(std::string path);

//Create new projectile
int NewProjectile(Vector2D pos, Vector2D vec, int type);

//Spawn an enemy
int SpawnEnemy(Vector2D pos, Vector2D vec, int type);

//Spawn a dust
int SpawnDusts(SDL_Rect rect, Vector2D vec, int type, int timeLeft);

//Set the properties of enemies
void SpawnEnemySetDefault(Enemy *& enemy, int type);

//Set the properties of proectiles
void SpawnProjSetDefault(Projectile *& enemy, int type);

//Set the properties of dusts
void SpawnDustSetDefault(Dust *& dust, int type, int timeLeft);

//Intersection test
bool Intersect(const SDL_Rect& r1, const SDL_Rect& r2);

//Scale rectangle
void scaleRect(SDL_Rect& rect, double scale);

//Look at gameobject
void LookAt(GameObject* from, GameObject* to, double rot = 0);

//Play mp3 music
inline void PlayMUS(const char* filename) {
	string cmd = "open ";
	cmd.append(filename);
	mciSendString(cmd.c_str(), NULL, 0, 0);
	cmd.replace(0, 4, "play");
	mciSendString(cmd.c_str(), NULL, 0, 0);
}

//Get unit vector of object from one vector to another
inline Vector2D GetFromToVector(const Vector2D& from, const Vector2D& to) {
	return  -(from - to).Normalize();
}

