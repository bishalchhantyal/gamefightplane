#pragma once
#include "Texture2D.h"
#include "Vector2D.h"
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Texture2D enemyTex;

	Vector2D velocity;

	int health;

	int damage;				//Enemy damage

	int knockback;

	int type;				//NPC type

	int immuneTime = 0;

	int scoreValue = 0;

	_UUID UUID;				//Unique id

	int timer1 = 0;

	int timer2 = 0;

	static const int  MAX_IMMUNE_TIME = 10;
	//Constructor
	Enemy() : GameObject(),
	          enemyTex(nullptr), velocity(0, 0), health(0), damage(0), type(0), UUID(0)
	{
	}

	Enemy(Vector2D pos, Vector2D vel) : GameObject(pos),
		enemyTex(nullptr), velocity(vel), health(0), damage(0), type(0), UUID(0)
	{
	}

	Enemy(Vector2D pos, int health, int damage, int type) : GameObject(pos),
		enemyTex(nullptr), velocity(0, 0),
		health(health), damage(damage),
		type(type), UUID(0)
	{
	}

	~Enemy();

	void Update() override;

	void Draw() override;

	void CheckDead();

	void AI();

	void DamageEnemy(int damage, int knockback, _UUID uuid);
};
