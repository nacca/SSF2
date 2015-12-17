#include "ModuleParticleSystem.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "Collider.h"
#include <queue>


ModuleParticleSystem::ModuleParticleSystem(bool start_enabled) : Module(start_enabled)
{
}

ModuleParticleSystem::~ModuleParticleSystem()
{
}

bool ModuleParticleSystem::Start()
{
	return true;
}

update_status ModuleParticleSystem::Update()
{
	for (list<Particle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		if (!(*it)->impact)
		{
			(*it)->collider->rect.x += (*it)->speed;
			(*it)->position.x += (*it)->speed;
			App->renderer->Blit((*it)->graphics, (*it)->position.x - 13, (*it)->position.y - 8, &((*it)->particleAnimation.GetCurrentFrame()));
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleParticleSystem::PostUpdate()
{
	queue<list<Particle*>::iterator> queueIterators;
	for (list<Particle*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		if ((*it)->impact)
			queueIterators.push(it);
	}
	while (!queueIterators.empty())
	{
		list<Particle*>::iterator it = queueIterators.front();
		particleList.erase(it);
		queueIterators.pop();
	}

	return UPDATE_CONTINUE;
}

bool ModuleParticleSystem::CleanUp()
{
	return true;
}

bool ModuleParticleSystem::newParticle(iPoint pos, SDL_Rect rec, SDL_Texture* graphics, Animation particleAnimation, int speed)
{
	LOG("NEW PARTICLE");
	Collider* collider = new Collider( rec, PARTICLES, this );
	Particle* particle = new Particle { pos, collider, graphics, particleAnimation, speed, false };
	App->collisions->AddCollider(collider);
	particleList.push_back(particle);
	return true;
}

void ModuleParticleSystem::OnCollision(Collider* c1, Collider* c2)
{
	for (list<Particle*>::iterator it1 = particleList.begin(); it1 != particleList.end(); ++it1)
	{
		if ((*it1)->collider == c1)
		{
				(*it1)->impact = true;
				LOG("COLISION!");
		}		
	}
}