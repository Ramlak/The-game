#ifndef MENU
#define MENU

#include "stdafx.h"

#define MENU_ACTION 1
#define MENU_OPTION 2

#define MENU_UP 1
#define MENU_DOWN 2
#define MENU_LEFT 3
#define MENU_RIGHT 4
#define MENU_ACT 5

struct menu {
	struct entry {
		int type;
		std::string name;
		int action;
		float* value;
		float min, max, step;
		inline entry(const char *name, int action);
		inline entry(const char *name, float* value, float min, float max, float step);
	};
	void draw();
	
	int active;
	int size;
	std::vector<entry> entries;

	inline int action(int a);
	inline void add_value(const char *name, float* value, float min, float max, float step);
	inline void add_action(const char *name, int action);
	inline menu() {
		active = 0;
		size = 0;
	}
};



inline void menu::add_value(const char *name, float* value, float min, float max, float step = 1.0) {
	entries.push_back(entry(name, value, min, max, step));
	++size;
}
inline void menu::add_action(const char *name, int action) {
	entries.push_back(entry(name, action));
	++size;
}

inline int menu::action(int a) {
	entry& x = entries[active];
	switch (a) {
	case MENU_UP:
		if (active == 0)
			active = size - 1;
		else
			--active;
		break;
	case MENU_DOWN:
		if (active == size - 1)
			active = 0;
		else
			++active;
		break;
	case MENU_LEFT:
		if (x.type == MENU_OPTION)
			if (*(x.value) - x.step <= x.min)
				*(x.value) = x.min;
			else
				*(x.value) -= x.step;
		break;
	case MENU_RIGHT:
		if (x.type == MENU_OPTION)
			if (*(x.value) + x.step >= x.max)
				*(x.value) = x.max;
			else
				*(x.value) += x.step;
		break;
	case MENU_ACT:
		if (x.type == MENU_ACTION)
			return x.action;
		break;
	}
	return -1;
}
inline menu::entry::entry(const char *name, int action) {
	this->name = name;
	this->type = MENU_ACTION;
	this->action = action;
}
inline menu::entry::entry(const char *name, float* value, float min, float max, float step) {
	this->name = name;
	this->type = MENU_OPTION;
	this->value = value;
	this->min = min;
	this->max = max;
	this->step = step;
	this->action = -1;
}
#endif