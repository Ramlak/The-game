#include "stdafx.h"
#include "acid.h"

ALLEGRO_BITMAP* bg;

std::vector<std::pair<int, int> > stack;

std::vector < std::vector<int> > tab;

float p = 0.400;

float h = 0, s = 1.0, v = 1.0;

void bg_draw() {
	al_set_target_bitmap(bg);
	std::vector<std::pair<int, int>> newstack;

	h = h+2.5;
	if (h > 360) h = 0;
	v = v - 1.0 / 144.9;
	if (v <= 0) bg_clear();
	for (unsigned int i = 0; i < stack.size(); ++i) {

		int x = stack[i].first;
		int y = stack[i].second;
		float r, g, b;
		al_color_hsv_to_rgb(h, s, v, &r, &g, &b);
		al_draw_pixel(x, y, al_map_rgb_f(r,g,b));

		if (x != 0 && tab[x - 1][y] != 1) {
			float prob = 0.001*(rand() % 1000);
			if (prob > p) {
				newstack.push_back(std::make_pair(x - 1, y));
			}
			tab[x - 1][y] = 1;
		}
		if (x != MAP_SIZE-1 && tab[x + 1][y] != 1) {
			float prob = 0.001*(rand() % 1000);
			if (prob > p) {
				newstack.push_back(std::make_pair(x + 1, y));
			}
			tab[x + 1][y] = 1;
		}
		if (y != 0 && tab[x][y-1] != 1) {
			float prob = 0.001*(rand() % 1000);
			if (prob > p) {
				newstack.push_back(std::make_pair(x, y-1));
			}
			tab[x][y-1] = 1;
		}
		if (y != MAP_SIZE-1 && tab[x][y+1] != 1) {
			float prob = 0.001*(rand() % 1000);
			if (prob > p) {
				newstack.push_back(std::make_pair(x, y+1));
			}
			tab[x][y+1] = 1;
		}
	}
	stack = newstack;
	al_set_target_backbuffer(okno);
}
void bg_init() {
	bg = al_create_bitmap(MAP_SIZE, MAP_SIZE);
	tab.resize(MAP_SIZE);
	for (int i = 0; i < MAP_SIZE; ++i)
		tab[i].resize(MAP_SIZE);
	for (int i = 0; i < 20; ++i)
		stack.push_back(std::make_pair(rand() % MAP_SIZE, rand() % MAP_SIZE));
	al_set_target_backbuffer(okno);
}


void bg_clear() {
	stack.clear();
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j)
			tab[i][j] = 0;
	}
	for (int i = 0; i < 20; ++i)
		stack.push_back(std::make_pair(rand() % MAP_SIZE, rand() % MAP_SIZE));

	float r, g, b;
	al_color_hsv_to_rgb((rand()%6)*60, 1.0, 0.23 , &r, &g, &b);
	al_clear_to_color(al_map_rgb_f(r,g,b));
	v = 1.0;
}