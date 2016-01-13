#ifndef __MODULECOLLISIONS_H__
#define __MODULECOLLISIONS_H__

#include "Module.h"
#include <list>

using namespace std;

struct Collider{

	SDL_Rect rect;
	collider_type type;
	Module* module;
	bool needDelete;
	int damage;
	damage_type damageType;

	Collider()
	{
		rect = { 0, 0, 0, 0 };
		type = COLLIDER_OTHERS;
		module = NULL;
		needDelete = false;
		damage = 0;
		damageType = NONE_DAMAGE;
	}

	Collider(SDL_Rect rect, collider_type type, Module* module)
	{
		this->rect = rect;
		this->type = type;
		this->module = module;
		needDelete = false;
		damage = 0;
		damageType = NONE_DAMAGE;
	}

};


class ModuleCollisions :
	public Module
{
public:
	ModuleCollisions(bool start_enabled = true);
	virtual ~ModuleCollisions();

	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();

	void AddCollider(SDL_Rect rec, collider_type type, Module* module);
	void AddCollider(Collider* collider);

	bool CanCollide(collider_type type1, collider_type type2);
	bool IsCollision(SDL_Rect rec1, SDL_Rect rec2);

private:
	list<Collider*> colliders;
	bool show_colliders;
	bool matrix[8][8];

};

#endif