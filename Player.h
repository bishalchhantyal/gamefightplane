#pragma once
#include "Vector2D.h"
#include "GameObject.h"
#include "Texture2D.h"
#include <SDL.h>

struct Player : public GameObject
{
	Vector2D velocity;

	Texture2D playerTex;

	int health;

	int damage;

	int immuneTime = 0;

	int alpha = 255;

	bool uncontrol = false;

	static const int MAX_IMMUNE_TIME = 60;
	//???
	Player() : GameObject(),
	           velocity(0, 0),
	           playerTex(), health(100), damage(0)
	{
	}

	virtual ~Player();

	//Check death
	void CheckDead();

	//Update player behavior
	void Update() override;

	//Draw
	void Draw() override;

	//Damaged effect
	void DamagePlayer(int damage, int knockback, GameObject* enemy);
};

