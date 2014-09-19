#include "menu.h"

extern ALLEGRO_FONT * menu_font;

void menu::draw() {
	al_clear_to_color(COLOR_BLACK);
	al_draw_rectangle(MAP_SIZE / 2 - 60, 120, MAP_SIZE / 2 + 60, 240, al_map_rgb(0, 255, 00), 10);
	al_draw_filled_rectangle(MAP_SIZE / 2, 175, MAP_SIZE / 2 + 108, 185, COLOR_WHITE);
	char buffer[100];
	for (int i = 0; i < size; ++i) {
		if (entries[i].type == MENU_OPTION) {
			sprintf_s(buffer, 100, "%s: %.1f", entries[i].name.c_str(), *entries[i].value);
			al_draw_text(menu_font, (i == active) ? al_map_rgb(255, 0, 0) : COLOR_WHITE, MAP_SIZE / 2, MAP_SIZE / 2 + 60 * i, ALLEGRO_ALIGN_CENTER, buffer);
		}
		else {
			al_draw_text(menu_font, (i == active) ? al_map_rgb(255, 0, 0) : COLOR_WHITE, MAP_SIZE / 2, MAP_SIZE / 2 + 60 * i, ALLEGRO_ALIGN_CENTER, this->entries[i].name.c_str());
		}
	}
	al_flip_display();
}