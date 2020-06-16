#pragma once
#include "Vector2D.h"
#include <SDL.h>

using _UUID = uint16_t;

class GameObject
{
public:
	std::string name;

	Vector2D position;

	SDL_Rect hitbox;

	double rotation;

	bool active;

	GameObject() :position(0, 0), hitbox(), rotation(0), active(true) 
	{}

	explicit GameObject(Vector2D&& pos) :position(pos), hitbox(), rotation(0), active(true) 
	{}

	explicit GameObject(const Vector2D& pos) :position(pos), hitbox(), rotation(0), active(true) 
	{}

	virtual ~GameObject() {}

	//Update function for all derived class
	virtual void Update() = 0;
	//Drawing function
	virtual void Draw() = 0;

	Vector2D Center() const {
		Vector2D res = { position.X + hitbox.w / 2,position.Y + hitbox.h / 2 };
		return res;
	}
};

