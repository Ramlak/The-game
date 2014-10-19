#include "stdafx.h"
#include "bullet.h"
#include "menu.h"
#include "acid.h"
#include <Windows.h>

#define FOR(x) for (size_t i = 0; i < (x).size(); i++)

using namespace std;

ALLEGRO_KEYBOARD_STATE klawiatura;
ALLEGRO_DISPLAY * okno;
ALLEGRO_FONT * font, *foot_font, *menu_font;
ALLEGRO_TIMER * timer;
ALLEGRO_EVENT_QUEUE * queue;
ALLEGRO_EVENT event;
ALLEGRO_SAMPLE *theme, *shoot;


vector < player > players;
list < bullet > bullets;

int music_on = 1;
extern float PAN;
extern int GUNSOUND;
int ACID;

void Message(const char * a)
{
	MessageBox(NULL, a, "Error", MB_OK);
}

void check_collisions()
{
	for (size_t i = 0; i < players.size(); i++)
	{
		for (size_t j = i + 1; j < players.size(); j++)
		{
			if (players[i].collision(players[j]))
			{
				float v_x_1 = players[i].v_x;
				float v_x_2 = players[j].v_x;
				float v_y_1 = players[i].v_y;
				float v_y_2 = players[j].v_y;
				
				players[i].v_x = v_x_2;
				players[i].v_y = v_y_2;
				players[j].v_x = v_x_1;
				players[j].v_y = v_y_1;

				players[i].step();
				players[j].step();
			}
		}
	}
}

void check_hits()
{
	FOR(players)
	{
		for (auto j = bullets.begin(); j != bullets.end(); j++)
		{

			if (players[i].is_in(j->x, j->y) && j->alive)
			{
				bullets.erase(j);
				players[i].hp = players[i].hp - 1;
			}
		}
	}
}

void draw_footer()
{
	FOR(players)
	{

		if (players[i].alive)
		{
			al_draw_filled_rectangle(MAP_SIZE / players.size() * i, MAP_SIZE + 2, MAP_SIZE / players.size() * i + MAP_SIZE / players.size() * players[i].hp / HP, MAP_SIZE + FOOTER_SIZE, players[i].color);
		}
		char * buffer = new char[100];

		sprintf_s(buffer, 100, "Player %d", i + 1);
		al_draw_line(MAP_SIZE / players.size() * i, MAP_SIZE + 1, MAP_SIZE / players.size() * i + MAP_SIZE / players.size() * players[i].ammo / MAX_AMMO, MAP_SIZE + 1, al_map_rgb(255, 255, 255), 2);
		al_draw_text(foot_font, al_map_rgb(0, 0, 255), MAP_SIZE / players.size() * i + MAP_SIZE / players.size() / 2, MAP_SIZE + 1, ALLEGRO_ALIGN_CENTER, buffer);

		delete[] buffer;
	}
}

int test_deads()
{
	FOR(players)
	{
		if (!players[i].alive)
		{
			return i + 1;
			break;
		}
	}
	return 0;
}

int game(void) {
	players.clear();
	bullets.clear();
	players.push_back(player(0, 0, al_map_rgb(255, 0, 0), ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_C, ALLEGRO_KEY_V, ALLEGRO_KEY_B, &players, &klawiatura));
	players.push_back(player(MAP_SIZE - BLOCK_SIZE, MAP_SIZE - BLOCK_SIZE, al_map_rgb(0, 255, 0), ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_SLASH, ALLEGRO_KEY_COMMA, ALLEGRO_KEY_FULLSTOP, &players, &klawiatura));
	players[1].gun_alpha = 3.1415926;

	while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
	{
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&klawiatura);
			if (ACID) {
				unsigned char r, g, b;
				int x;

				FOR(players) {
					al_unmap_rgb(players[i].color, &r, &g, &b);
					x = rand() % 6;
					switch (x) {
						case 0:
							if (r >= 3)
								r -= 3;
							break;
						case 1:
							if (r <= 250)
								r += 3;
							break;
						case 2:
							if (g >= 3)
								g -= 3;
							break;
						case 3:
							if (g <= 250)
								g += 3;
							break;
						case 4:
							if (b >= 3)
								b -= 3;
							break;
						case 5:
							if (r <= 250)
								b += 3;
							break;
					}
					players[i].color = al_map_rgb(r, g, b);
				}

			}
			if (test_deads() == 0)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));
				if (ACID) {
					bg_draw();
					al_draw_bitmap(bg, 0.0, 0.0, 0);
				}

				FOR(players)
					players[i].move();
				for (auto j = bullets.begin(); j != bullets.end(); j++)
				{
					j->move();
					if (!j->alive)
						bullets.erase(j);
				}

				FOR(players)
					players[i].shoot(bullets, shoot);
				if (GUNSOUND) {
					al_play_sample(shoot, 1.0*(GUNSOUND+2)/4, PAN/GUNSOUND, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);


					GUNSOUND = 0;
					PAN = 0;
				}
				check_collisions();
				check_hits();

				FOR(players)
					players[i].draw();
				for (auto j = bullets.begin(); j != bullets.end(); j++)
					j->draw();

				draw_footer();
			}
			else
			{
				char * buffer = new char[100];
				sprintf_s(buffer, 100, "Player %d loses!", test_deads());
				al_draw_text(font, COLOR_WHITE, MAP_SIZE / 2, MAP_SIZE / 2 - 72 / 2, ALLEGRO_ALIGN_CENTER, buffer);
				delete[] buffer;
				draw_footer();
				al_flip_display();
				al_rest(2.0);
				return -1;
			}
		}
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return -1;

		al_flip_display();
	}
	return -1;
}

int o_menu(void) {
	menu options_menu;
	int music = 1 - music_on;
	{
		std::vector<std::string> options;
		options.push_back(std::string("ON"));
		options.push_back(std::string("OFF"));
		options_menu.add_enum("MUSIC", &music, 2, options);
	}
	{
		std::vector<std::string> options;
		options.push_back(std::string("NO"));
		options.push_back(std::string("OH YEAH"));
		options_menu.add_enum("ACID", &ACID, 2, options);
	}
	options_menu.add_value("SPEED", &SPEED, 1.0, 3.0, 0.1);
	options_menu.add_value("AMMO", &MAX_AMMO, 100.0, 500.0, 25.0);
	options_menu.add_value("HEALTH", &HP, 100.0, 500.0, 50.0);
	options_menu.add_action("BACK", 4);
	while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE)) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		al_get_keyboard_state(&klawiatura);
		options_menu.draw();
		if (al_key_down(&klawiatura, ALLEGRO_KEY_UP)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_UP)) {
				al_get_keyboard_state(&klawiatura);
			}
			options_menu.action(MENU_UP);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)) {
				al_get_keyboard_state(&klawiatura);
			}
			options_menu.action(MENU_DOWN);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT)) {
				al_get_keyboard_state(&klawiatura);
			}
			options_menu.action(MENU_LEFT);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT)) {
				al_get_keyboard_state(&klawiatura);
			}
			options_menu.action(MENU_RIGHT);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE) || al_key_down(&klawiatura, ALLEGRO_KEY_ENTER)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE) || al_key_down(&klawiatura, ALLEGRO_KEY_ENTER)) {
				al_get_keyboard_state(&klawiatura);
			}
			if (options_menu.action(MENU_ACT) == 4) {
				if (music == 1 &&  music_on == 1) {
					al_stop_samples();
					music_on = 0;
				}
				else if (music == 0 && music_on == 0) {
					al_play_sample(theme, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					music_on = 1;
				}
				break;
			}
		}
	}
	return -1;
}

int m_menu(void) {
	srand(time(0));

	al_play_sample(theme, 0.7, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	//al_play_sample(shoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	menu main_menu;
	int i=0;
	main_menu.add_action("START", 1);
	main_menu.add_action("OPTIONS", 2);
	main_menu.add_action("CREDITS", 3);
	main_menu.add_action("EXIT", 4);
	while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE)) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		al_get_keyboard_state(&klawiatura);
		main_menu.draw();
		if (al_key_down(&klawiatura, ALLEGRO_KEY_UP)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_UP)) {
				al_get_keyboard_state(&klawiatura);
			}
			main_menu.action(MENU_UP);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_DOWN)) {
				al_get_keyboard_state(&klawiatura);
			}
			main_menu.action(MENU_DOWN);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT)) {
				al_get_keyboard_state(&klawiatura);
			}
			main_menu.action(MENU_LEFT);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT)) {
				al_get_keyboard_state(&klawiatura);
			}
			main_menu.action(MENU_RIGHT);
		}
		if (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE) || al_key_down(&klawiatura, ALLEGRO_KEY_ENTER)) {
			while (al_key_down(&klawiatura, ALLEGRO_KEY_SPACE) || al_key_down(&klawiatura, ALLEGRO_KEY_ENTER)) {
				al_get_keyboard_state(&klawiatura);
			}
			if (main_menu.action(MENU_ACT) == 1) game();
			if (main_menu.action(MENU_ACT) == 2) o_menu();
			if (main_menu.action(MENU_ACT) == 4) break;
		}
	}
	return -1;
}

int main(void)
{
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();

	al_install_audio();
	al_install_keyboard();

	al_reserve_samples(2);
	if (!(theme = al_load_sample("hydrogen.ogg")) || !(shoot = al_load_sample("shoot.ogg")))
	{
		Message("Cannot load sample!");
	}

	font = al_load_font("LiberationSerif-Regular.ttf", 72, 0);
	menu_font = al_load_font("LiberationSerif-Regular.ttf", 36, 0);
	foot_font = al_load_font("LiberationSerif-Regular.ttf", FOOTER_SIZE - 4, 0);
	if (!font || !foot_font)
	{
		MessageBox(NULL, "Cannot load font!", "Error", MB_OK);
		return 1;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED);
	okno = al_create_display(MAP_SIZE, MAP_SIZE + FOOTER_SIZE);
	al_set_window_title(okno, "Gra");
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_hide_mouse_cursor(okno);
	al_start_timer(timer);
	
	al_register_event_source(queue, al_get_display_event_source(okno));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	bg_init();

	m_menu();

	bullets.clear();
	players.clear();
	al_destroy_timer(timer);
	al_destroy_display(okno);
	al_destroy_sample(theme);
	al_destroy_sample(shoot);
	return 0;
}