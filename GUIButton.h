#pragma once
#include "UIText.h"
#include "GUIBase.h"
#include "Vector2D.h"
#include "Main.h"
class GUIButton : public GUIBase
{
public:

	SDL_Rect posBox;
	GUIButton() :GUIBase(), action(nullptr) {

	}
	void SetTexture(SDL_Renderer* render,const string& path) {
		this->_UITexture.LoadTexFromFile(render, path);
	}
	void SetTexture(SDL_Renderer* render, TTF_Font* font, const string& str) {
		this->_UITexture.loadFromRenderedText(render, font, str, SDL_Color{ 255,255,255 });
	}
	//Set button position
	void Render(SDL_Renderer* renderer);
	//Update button with the change of SDL_Event
	void Update(const SDL_Event&);
	//Handle mouse on event
	void SetButtonEvent(void(*fp)(const SDL_Event&, SDL_Rect&, Texture2D&));

	~GUIButton() {

	}

private:
	void(*action)(const SDL_Event&, SDL_Rect&, Texture2D&);
};
