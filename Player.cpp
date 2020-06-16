#include "Player.h"
#include "Main.h"

Player::~Player()
{
}

void Player::CheckDead()
{
	if (health <= 0) {
		active = false;
		isGameover = true;
	}
}

void Player::Update()
{
	if (immuneTime > 0)
	{
		immuneTime--;
		alpha = (255 / 4) * (MAX_IMMUNE_TIME - immuneTime + 1);
		if (immuneTime < 30) {
			uncontrol = false;
		}
	}
	else {
		alpha = 255;
		//Check if player is been damaged
		for (int i = 0; i < MAX_PROJECTILE; i++) {
			if (ProjectileGroup[i]->active && ProjectileGroup[i]->hostile && ProjectileGroup[i]->damage > 0) {
				if (Intersect(hitbox, ProjectileGroup[i]->hitbox)) {
					DamagePlayer(ProjectileGroup[i]->damage, 2, ProjectileGroup[i]);
				}
			}
		}

	}
	//Position update
	position += velocity;
	if (position.X < 0) position.X = 0;
	else if (position.X + hitbox.w > SCREEN_WIDTH) position.X = SCREEN_WIDTH - hitbox.w - 1;
	if (position.Y < 0) position.Y = 0;
	else if (position.Y + hitbox.h > SCREEN_HEIGHT) position.Y = SCREEN_HEIGHT - hitbox.h -1;
	hitbox.x = static_cast<int>(position.X);
	hitbox.y = static_cast<int>(position.Y);
	hitbox.w = playerTex.getWidth();
	hitbox.h = playerTex.getHeight();
	if (g_totalTime % 10 < 1) {
		int id = SpawnDusts(hitbox, Vector2D(), 0, 120);
		DustsGroup[id]->alpha = 100;
	}


	//Rotation update
	if (!uncontrol) {
		Vector2D mouseDiff = g_MousePosition - Center();
		rotation = mouseDiff.GetRotationDegree() + 90;
	}
}

void Player::Draw()
{
	this->playerTex.SetColor(0, 0, 255);
	this->playerTex.setAlpha(alpha);
	SDL_RenderCopyEx(g_GameRenderer, playerTex.GetPtr(), nullptr, &hitbox, this->rotation, nullptr, SDL_FLIP_NONE);
#ifdef DEBUG
	g_magicTex.SetColor(255, 0, 0);
	if (SDL_RenderCopyEx(g_GameRenderer, g_magicTex.GetPtr(), nullptr, &hitbox, 0, nullptr, SDL_FLIP_NONE) == -1) {
		cout << SDL_GetError() << endl;
	}
#endif // DEBUG
}

void Player::DamagePlayer(int damage, int knockback, GameObject* enemy)
{
	uncontrol = true;
	velocity = (enemy->Center() - Center()).Normalize() * -knockback;
	health -= damage;
	CheckDead();
	immuneTime = MAX_IMMUNE_TIME;
}
