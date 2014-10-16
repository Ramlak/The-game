#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/timer.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <list>
#include <string>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>


#define BLOCK_SIZE 15
#define MAP_SIZE 600
#define BLOCK_ACCELERATION 0.02*SPEED
#define BLOCK_MAX_SPEED 1.5*SPEED
#define BULLET_SIZE 2
#define GUN_SIZE 1.8
#define FOOTER_SIZE 20
#define BULLET_SPEED 3*SPEED
#define THICKNESS 2
#define FPS 300
#define GUN_RESOLUTION 0.015
#define COLOR_WHITE al_map_rgb(255, 255, 255)
#define COLOR_BLACK al_map_rgb(0, 0, 0)

//OPTIONS
extern float MAX_AMMO;
extern float HP;
extern float SPEED;
extern int ACID;
extern ALLEGRO_DISPLAY * okno;