#include <Windows.h>
#include "Enemy.h"
#include "Main.h"
#include "Utils.h"

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (immuneTime > 0)
	{
		immuneTime--;
	}
	//Check if the enemy is been damaged
	for (int i = 0; i < MAX_PROJECTILE; i++) {
		if (ProjectileGroup[i]->active && !ProjectileGroup[i]->hostile && ProjectileGroup[i]->damage > 0) {
			if (Intersect(hitbox, ProjectileGroup[i]->hitbox)) {
				DamageEnemy(ProjectileGroup[i]->damage, ProjectileGroup[i]->knockback, ProjectileGroup[i]->UUID);
			}
		}
	}

	//Position update
	if (active)
		CheckDead();
	if (active)
		AI();
}

void Enemy::Draw()
{
	if (!active) {
		return;
	}
	//SDL_Rect renderRect{ hitbox.x - hitbox.w / 2,hitbox.y - hitbox.h / 2,hitbox.w,hitbox.h };

	switch (type)
	{
	case 0:
		enemyTex.SetColor(255, 0, 0);
		break;
	default: 
		break;
	}

	if (SDL_RenderCopyEx(g_GameRenderer, enemyTex.GetPtr(), nullptr, &hitbox, this->rotation, nullptr, SDL_FLIP_NONE) == -1) {
		cout << SDL_GetError() << endl;
	}

#ifdef DEBUG
	g_magicTex.SetColor(255, 0, 0);
	if (SDL_RenderCopyEx(g_GameRenderer, g_magicTex.GetPtr(), nullptr, &hitbox, 0, nullptr, SDL_FLIP_NONE) == -1) {
		cout << SDL_GetError() << endl;
	}
#endif // DEBUG


}

void Enemy::CheckDead()
{
	if (health <= 0) {
		active = false;
		g_playerScore += this->scoreValue;
		if (g_playerScore == 50) {
			g_spawnRate -= 100;
		}
		else if (g_playerScore == 100) {
			g_spawnRate -= 100;
		}
		else if (g_playerScore == 500) {
			g_spawnRate >>= 1;
		}
		else if (g_playerScore == 1000) {
			g_spawnRate >>= 1;
		}
	}
}

void Enemy::AI()
{
	if (type == 0)
	{
		//Chase player
		//Vector2D diff = Center() - player.Center();
		//diff = diff.Normalize() * -1;
		//rotation = diff.GetRotationDegree() + 90;
		//velocity = (velocity * 10 + diff) / 11;
		if (velocity.Length() > 0.01)velocity *= 0.9f;
		LookAt(this, &player, -90);
		timer1++;
		if (timer1 > 80) {
			NewProjectile(this->Center(), GetFromToVector(this->Center(), player.Center()) * 3, 1);
			timer1 = 0;
		}
		position += velocity;
	}
	else if (type == 1) {
		this->rotation += 5;
		if (this->rotation > 360)this->rotation -= 360;
		Vector2D diff = -(this->Center() - player.Center()).Normalize();
		this->velocity = diff * 2;
	}
	if (immuneTime == 0) {
		position += velocity;
	}
	hitbox.x = static_cast<int>(position.X);
	hitbox.y = static_cast<int>(position.Y);
	hitbox.w = enemyTex.getWidth();
	hitbox.h = enemyTex.getHeight();
	if (position.X < 0) position.X = 0;
	else if (position.X + hitbox.w > SCREEN_WIDTH) position.X = SCREEN_WIDTH - hitbox.w - 1;
	if (position.Y < 0) position.Y = 0;
	else if (position.Y + hitbox.h > SCREEN_HEIGHT) position.Y = SCREEN_HEIGHT - hitbox.h - 1;
	if (player.immuneTime == 0) {
		if (hitbox.x + hitbox.w >= player.hitbox.x
			&& hitbox.x <= player.hitbox.x + player.hitbox.w
			&& hitbox.y + hitbox.h >= player.hitbox.y
			&& hitbox.y <= player.hitbox.y + player.hitbox.h)
		{
			player.DamagePlayer(this->damage, this->knockback, this);
		}
	}
}

void Enemy::DamageEnemy(int damage, int knockback, _UUID uuid) {
	health -= damage;
	velocity = (ProjectileGroup[uuid]->Center() - Center()).Normalize() * -knockback;
	ProjectileGroup[uuid]->active = false;
	immuneTime = MAX_IMMUNE_TIME;
	CheckDead();
}