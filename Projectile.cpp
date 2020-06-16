#include "Projectile.h"
#include "Main.h"

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	//Position update
	AI();
	timeLeft--;
	//Check time left
	if (timeLeft == 0) {
		Kill();
		return;
	}
	//Check boundary
	if (position.X < 0 || position.X > SCREEN_WIDTH || position.Y < 0 || position.Y >SCREEN_HEIGHT) {
		Kill();
		return;
	}
}

void Projectile::Draw()
{
	if (!active) {
		return;
	}
	SDL_Rect renderRect{ hitbox.x - hitbox.w / 2,hitbox.y - hitbox.h / 2,hitbox.w,hitbox.h };
	
	if (type == 0) {
		projTex.SetColor(10, 10, 255);
	}
	else if (type == 1) {
		projTex.SetColor(255, 10, 10);
	}

	//Put the image on the screen
	if (SDL_RenderCopyEx(g_GameRenderer, projTex.GetPtr(), nullptr, &renderRect, this->rotation, nullptr, SDL_FLIP_NONE) == -1) {
		cout << SDL_GetError() << endl;
	}
}

void Projectile::Kill()
{
	active = false;
}

void Projectile::AI()
{
	//Position
	position += velocity;
	hitbox.x = static_cast<int>(position.X);
	hitbox.y = static_cast<int>(position.Y);
	hitbox.w = projTex.getWidth();
	hitbox.h = projTex.getHeight();

	switch (type)
	{
	case 0:
		rotation = velocity.GetRotationDegree() + 90;
		break;
	case 1:
		rotation = velocity.GetRotationDegree() + 90;
		break;
	default:
		break;
	}
}
