#pragma once
#include <SDL_ttf.h>
#include <string>
#include "GUIBase.h"
#include "Utils.h"
class UIText : public GUIBase
{
public:
	//Constructor
	UIText() : GUIBase()
	{
	}
	UIText(TTF_Font* f): font(f),GUIBase()
	{
	}
	~UIText() {

	}

	void Render(SDL_Renderer* renderer, SDL_Rect* rect) override {
		if (text != "") {
			_UITexture.~Texture2D();
			_UITexture.loadFromRenderedText(renderer, font, text, color);
		}
		SDL_RenderCopyEx(renderer, _UITexture.GetPtr(), nullptr, rect, 0, nullptr, SDL_FLIP_NONE);
	}
	void SetText(const std::string& str) {
		text = str;
	}
	void SetColor(SDL_Color color) {
		this->color = color;
	}
private:
	TTF_Font* font;
	std::string text = "";
	SDL_Color color = { 0,0,0 };
};

