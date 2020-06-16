#pragma once
#include "GameObject.h"
#include "Texture2D.h"
class Projectile : public GameObject
{
public:
	Vector2D velocity;

	Texture2D projTex;

	int timeLeft = 600;

	int type = 0;

	int damage = 0;

	int knockback = 0;

	bool hostile = false;

	_UUID UUID;

	Projectile() : GameObject(), velocity(0, 0), projTex(nullptr), UUID(0) { active = false; }

	Projectile(Vector2D&& pos, Vector2D&& vec) : GameObject(pos), velocity(vec), projTex(nullptr), UUID(0)
	{
	}

	Projectile(const Vector2D& pos, const Vector2D& vec) : GameObject(pos), velocity(vec), projTex(nullptr), UUID(0)
	{
	}

	virtual ~Projectile();

	void Update() override;

	void Draw() override;

	void Kill();

	void AI();
};

