#ifndef __MODULECOLLISIONS_H__
#define __MODULECOLLISIONS_H__

#include "Module.h"
#include "Collider.h"
#include <list>

using namespace std;

class ModuleCollisions :
	public Module
{
public:
	ModuleCollisions(bool start_enabled = true);
	virtual ~ModuleCollisions();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

	void AddCollider(SDL_Rect rec, collider_type type, Module* module);
	void AddCollider(Collider* collider);

	bool CanCollide(collider_type type1, collider_type type2);
	bool IsCollision(SDL_Rect rec1, SDL_Rect rec2);

public:
	list<Collider*> colliders;
	bool show_colliders;
};

#endif