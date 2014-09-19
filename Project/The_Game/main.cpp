#include "stdafx.h"
#include "bullet.h"
#include "menu.h"
#include <Windows.h>

#define FOR(x) for (size_t i = 0; i < (x).size(); i++)

using namespace std;

ALLEGRO_KEYBOARD_STATE klawiatura;
ALLEGRO_DISPLAY * okno;
ALLEGRO_FONT * font, *foot_font;
ALLEGRO_TIMER * timer;
ALLEGRO_EVENT_QUEUE * queue;

vector < player > players;
list < bullet > bullets;

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
				players[i].step();
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


int main(void)
{
	al_init();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();

	font = al_load_font("Times.ttf", 72, 0);
	foot_font = al_load_font("Times.ttf", FOOTER_SIZE - 4, 0);

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

	al_start_timer(timer);
	
	ALLEGRO_EVENT event;
	al_register_event_source(queue, al_get_display_event_source(okno));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	players.push_back(player(0, 0, al_map_rgb(255, 0, 0), ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_C, ALLEGRO_KEY_V, ALLEGRO_KEY_B, &players, &klawiatura));
	players.push_back(player(MAP_SIZE - BLOCK_SIZE, 0, al_map_rgb(0, 255, 0), ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_SLASH, ALLEGRO_KEY_COMMA, ALLEGRO_KEY_FULLSTOP, &players, &klawiatura));
	
	menu main_menu;
	float x;
	main_menu.add_action("START", 1);
	main_menu.add_value("OPTION",&x,0.0,1.0,0.1);
	main_menu.add_action("EXIT", 1);

	while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE)) {
		al_get_keyboard_state(&klawiatura);
		main_menu.draw();

	}

	while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
	{
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&klawiatura);
			if (test_deads() == 0)
			{
				al_clear_to_color(al_map_rgb(0, 0, 0));

				FOR(players)
					players[i].move();
				for (auto j = bullets.begin(); j != bullets.end(); j++)
				{
					j->move();
					if (!j->alive)
						bullets.erase(j);
				}

				FOR(players)
					players[i].shoot(bullets);

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
				while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
				{
					al_get_keyboard_state(&klawiatura);
					al_wait_for_event(queue, &event);
					if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
						goto EXIT;
				}
				goto EXIT;
			}
		}
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			goto EXIT;

		al_flip_display();
	}
EXIT:
	bullets.clear();
	players.clear();
	al_destroy_timer(timer);
	al_destroy_display(okno);
	return 0;
}