#include "GUIButton.h"

void GUIButton::Render(SDL_Renderer * renderer)
{
	SDL_RenderCopyEx(renderer, this->_UITexture.GetPtr(), nullptr, &this->posBox, 0, nullptr, SDL_FLIP_NONE);
}

void GUIButton::Update(const SDL_Event & event)
{
	if (this->action)
		this->action(event, this->posBox, this->_UITexture);
}

void GUIButton::SetButtonEvent(void(*fp)(const SDL_Event &, SDL_Rect&, Texture2D&))
{
	this->action = fp;
}
