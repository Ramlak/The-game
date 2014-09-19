#ifndef BULLET
#define BULLET

#include "stdafx.h"
#include "player.h"

using namespace std;

struct bullet
{
	float x, v_x;
	float y, v_y;
	ALLEGRO_COLOR player_color;
	bool alive;

	bullet(player * Player);
	bullet() {}

	void move();
	void draw();
};

#endif