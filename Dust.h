#pragma once
#include "GameObject.h"
#include "Vector2D.h"
#include "Texture2D.h"

class Dust : public GameObject
{
public:
	int timeleft = 0;

	int type;

	int alpha = 255;

	Vector2D velocity = Vector2D();

	Texture2D dustTex;

	double scale = 1.0;

	_UUID UUID;

	Dust() : GameObject(), type(0) {}
	Dust(const Vector2D& pos) : GameObject(pos), type(0) {}
	Dust(const SDL_Rect& box) : GameObject(), type(0) {
		hitbox = box;
		hitbox.x += rand() % hitbox.w;
		hitbox.y += rand() % hitbox.h;
	}

	void Update() override;

	void Draw() override;

	~Dust();
};

