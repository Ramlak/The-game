#include "menu.h"

extern ALLEGRO_FONT * menu_font;

void menu::draw() {
	al_clear_to_color(COLOR_BLACK);
	char buffer[40];
		for (int i = 0; i < size; ++i) {
		al_draw_text(menu_font, (i == active) ? al_map_rgb(255, 0, 0) : COLOR_WHITE, 30, MAP_SIZE / 2 + 60 * i, ALLEGRO_ALIGN_LEFT, this->entries[i].name.c_str());
		if (entries[i].type==MENU_OPTION) {
			sprintf_s(buffer, 20, "%19.1f", *entries[i].value);
			al_draw_text(menu_font, (i == active) ? al_map_rgb(255, 0, 0) : COLOR_WHITE, MAP_SIZE-30, MAP_SIZE / 2 + 60 * i, ALLEGRO_ALIGN_RIGHT, buffer);
		}
	}
	al_flip_display();
}