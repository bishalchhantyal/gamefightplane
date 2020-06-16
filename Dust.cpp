#include "Dust.h"
#include "Main.h"

void Dust::Update()
{
	//if (timeleft>0)
	//	timeleft--;
	//if (timeleft == 0) {
	//	active = false;
	//	return;
	//}

	if (type == 0) {
		position += velocity;
		scale -= 0.01;
		if (scale <= 0)
			active = false;
		return;
	}

}

void Dust::Draw()
{
	if (!active)return;
	SDL_Rect renderRect = { hitbox.x ,hitbox.y,dustTex.getWidth(),dustTex.getHeight() };
	scaleRect(renderRect, scale);
	dustTex.setAlpha(alpha);
	SDL_RenderCopyEx(g_GameRenderer, dustTex.GetPtr(), nullptr, &renderRect, this->rotation, nullptr, SDL_FLIP_NONE);
	dustTex.setAlpha(255);
#ifdef DEBUG
	g_magicTex.SetColor(255, 0, 0);
	if (SDL_RenderCopyEx(g_GameRenderer, g_magicTex.GetPtr(), nullptr, &renderRect, 0, nullptr, SDL_FLIP_NONE) == -1) {
		cout << SDL_GetError() << endl;
	}
#endif // DEBUG
}

Dust::~Dust()
{
}

