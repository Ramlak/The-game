#ifndef PLAYER
#define PLAYER

#include "stdafx.h"
#include "bullet.h"

using namespace std;

struct bullet;

struct player
{
	float x, v_x;
	float y, v_y;
	float gun_alpha;
	float ammo;
	size_t counter;
	bool alive;
	ALLEGRO_COLOR color;
	ALLEGRO_KEYBOARD_STATE * klawiatura;
	vector < player > * players;
	int hp;
	int key_down, key_left, key_up, key_right, key_shoot, key_gun_left, key_gun_right;

	player(int X, int Y, ALLEGRO_COLOR COLOR, int KEY_UP, int KEY_DOWN, int KEY_LEFT, int KEY_RIGHT, int KEY_SHOOT, int KEY_GUN_LEFT, int KEY_GUN_RIGHT, vector < player > * players, ALLEGRO_KEYBOARD_STATE * klawiatura);
	bool collision(player &P);
	bool is_in(float X, float Y);
	void draw();
	void change_state();
	void step();
	void move();
	void shoot(list < bullet > &bullets, ALLEGRO_SAMPLE * shoot);
};

#endif