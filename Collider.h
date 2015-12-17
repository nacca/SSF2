#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Globals.h"

class Collider{
public:
	SDL_Rect rect;
	collider_type type;
	Module* module;
	bool needDelete;

	Collider()
	{
		needDelete = false;
	}

	~Collider()
	{

	}

	Collider(SDL_Rect rect, collider_type type, Module* module)
	{
		this->rect = rect;
		this->type = type;
		this->module = module;
		needDelete = false;
	}

};

#endif