#ifndef __MODULEPARTICLESYSTEM_H__
#define __MODULEPARTICLESYSTEM_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Application.h"
#include "Point.h"
#include <list>
#include "ModuleCollisions.h"
#include "Collider.h"

struct SDL_Texture;

struct Particle
{
	iPoint position;
	Collider* collider;
	SDL_Texture* graphics;
	Animation particleAnimation;
	int speed;
	bool impact;
};

class ModuleParticleSystem : public Module
{
public:
	ModuleParticleSystem(bool start_enabled = true);
	virtual ~ModuleParticleSystem();

	virtual bool Start();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

	virtual void OnCollision(Collider* c1, Collider* c2);
	
	bool newParticle(iPoint pos, SDL_Rect rec, SDL_Texture* graphics, Animation particleAnimation, int speed);

public:
	list<Particle*> particleList;

};

#endif