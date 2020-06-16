#pragma once
#include "Texture2D.h"
class GUIBase
{
public:
	GUIBase() :_UITexture(nullptr){

	}
	~GUIBase() {

	}

	virtual void Render(SDL_Renderer* renderer, SDL_Rect* rect){}


protected:
	Texture2D _UITexture;
};
