#include "stdafx.h"
#include "bullet.h"

// BULLET

void bullet::move()
{
	this->x = this->x + this->v_x;
	this->y = this->y + this->v_y;

	if (this->x >= MAP_SIZE - BULLET_SIZE || this->x <= 0)
		this->alive = false;
	if (this->y >= MAP_SIZE - BULLET_SIZE || this->y <= 0)
		this->alive = false;
}

void bullet::draw()
{
	if (this->alive)
		al_draw_filled_rectangle(this->x, this->y, this->x + BULLET_SIZE, this->y + BULLET_SIZE, this->player_color);
}

bullet::bullet(player * Player)
{
	this->alive = true;
	this->player_color = Player->color;
	this->x = Player->x + BLOCK_SIZE / 2 + cos(Player->gun_alpha) * (BLOCK_SIZE / 2) * GUN_SIZE -1;
	this->y = Player->y + BLOCK_SIZE / 2 - sin(Player->gun_alpha) * (BLOCK_SIZE / 2) * GUN_SIZE -1;
	this->v_x = BULLET_SPEED * cos(Player->gun_alpha);
	this->v_y = -BULLET_SPEED * sin(Player->gun_alpha);
}

// PLAYER

player::player(int X, int Y, ALLEGRO_COLOR COLOR, int KEY_UP, int KEY_DOWN, int KEY_LEFT, int KEY_RIGHT, int KEY_SHOOT, int KEY_GUN_LEFT, int KEY_GUN_RIGHT, vector < player > * players, ALLEGRO_KEYBOARD_STATE * klawiatura)
{
	this->x =  X;
	this->y = Y;
	this->color = COLOR;
	this->key_gun_left = KEY_GUN_LEFT;
	this->key_gun_right = KEY_GUN_RIGHT;
	this->key_down = KEY_DOWN;
	this->key_left = KEY_LEFT;
	this->key_right = KEY_RIGHT;
	this->key_up = KEY_UP;
	this->key_shoot = KEY_SHOOT;
	this->klawiatura = klawiatura;
	this->v_x = 0;
	this->v_y = 0;
	this->gun_alpha = 0;
	this->players = players;
	this->hp = HP;
	this->alive = true;
	this->ammo = MAX_AMMO;
}

void player::change_state()
{
	if (this->hp <= 0)
		this->alive = false;
	if (ammo) {
		if (++this->counter > 3)
		{
		this->counter = 0;
		this->ammo += (ammo == MAX_AMMO) ? 0 : 1;
		}
	}	
	else {
		if (++this->counter > 50){
			this->counter = 0;
			++this->ammo;
		}
	}
	float v1 = this->v_x * this->v_x + this->v_y * this->v_y;

	if (al_key_down(this->klawiatura, this->key_gun_left))
	{
		this->gun_alpha = this->gun_alpha + GUN_RESOLUTION;
	}
	if (al_key_down(this->klawiatura, this->key_gun_right))
	{
		this->gun_alpha = this->gun_alpha - GUN_RESOLUTION;
	}
	if (al_key_down(this->klawiatura, this->key_left))
	{
		this->v_x = this->v_x - BLOCK_ACCELERATION;
	}
	if (al_key_down(this->klawiatura, this->key_right))
	{
		this->v_x = this->v_x + BLOCK_ACCELERATION;
	}
	if (al_key_down(this->klawiatura, this->key_up))
	{
		this->v_y = this->v_y - BLOCK_ACCELERATION;
	}
	if (al_key_down(this->klawiatura, this->key_down))
	{
		this->v_y = this->v_y + BLOCK_ACCELERATION;
	}
	float v2 = this->v_x * this->v_x + this->v_y * this->v_y;
	if (v2 > BLOCK_MAX_SPEED * BLOCK_MAX_SPEED)
	{
		this->v_x = sqrt(v1 / v2) * this->v_x;
		this->v_y = sqrt(v1 / v2) * this->v_y;
	}
}

void player::step()
{
	if (this->x + this->v_x > MAP_SIZE - BLOCK_SIZE)
		this->x = 0;
	if (this->x + this->v_x < 0)
		this->x = MAP_SIZE - BLOCK_SIZE;
	if (this->y + this->v_y > MAP_SIZE - BLOCK_SIZE)
		this->y = 0;
	if (this->y + this->v_y < 0)
		this->y = MAP_SIZE - BLOCK_SIZE;

	this->x = this->x + this->v_x;
	this->y = this->y + this->v_y;
}

void player::move()
{
	this->change_state();
	this->step();
	}

bool player::is_in(float X, float Y)
{
	return (X >= this->x) && (X <= (this->x + BLOCK_SIZE)) && (Y >= this->y) && (Y <= (this->y + BLOCK_SIZE));
}

void player::draw()
{
	al_draw_rectangle(this->x + THICKNESS / 2, this->y + THICKNESS / 2, this->x + BLOCK_SIZE - THICKNESS / 2, this->y + BLOCK_SIZE - THICKNESS / 2, this->color, THICKNESS);
	al_draw_line(this->x + BLOCK_SIZE / 2, this->y + BLOCK_SIZE / 2, this->x + BLOCK_SIZE / 2 + cos(this->gun_alpha) * (BLOCK_SIZE / 2) * GUN_SIZE, this->y + BLOCK_SIZE / 2 - sin(this->gun_alpha) * (BLOCK_SIZE / 2) * GUN_SIZE, al_map_rgb(255, 255, 255), 2);
}

bool player::collision(player &P)
{
	return this->is_in(P.x, P.y) || this->is_in(P.x + BLOCK_SIZE, P.y) || this->is_in(P.x + BLOCK_SIZE, P.y + BLOCK_SIZE) || this->is_in(P.x, P.y + BLOCK_SIZE);
}

void player::shoot(list < bullet > &bullets)
{
	if (al_key_down(this->klawiatura, this->key_shoot) && this->ammo > 0)
	{
			bullets.push_back(bullet(this));
			this->ammo--;
	}
}
