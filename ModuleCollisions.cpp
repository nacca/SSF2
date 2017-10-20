#include "ModuleCollisions.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL\include\SDL.h"
#include <queue>
#include "ModuleInput.h"
#include <iostream>

ModuleCollisions::ModuleCollisions(bool start_enabled) : Module(start_enabled)
{
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_ONE] = false;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_TWO] = true;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_PARTICLES] = true;
	matrix[COLLIDER_PLAYER_ONE][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_ONE] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_BODY_PLAYER_TWO] = true;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_TWO] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_PARTICLES] = false;
	matrix[COLLIDER_BODY_PLAYER_ONE][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_ONE] = false;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_PLAYER_TWO] = true;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_ATTACK_PLAYER_TWO] = true;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_PARTICLES] = true;
	matrix[COLLIDER_ATTACK_PLAYER_ONE][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_PLAYER_TWO][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_ONE] = true;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_TWO] = false;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_PARTICLES] = true;
	matrix[COLLIDER_PLAYER_TWO][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_BODY_PLAYER_ONE] = true;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_ONE] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_TWO] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_PARTICLES] = false;
	matrix[COLLIDER_BODY_PLAYER_TWO][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_PLAYER_ONE] = true;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_ONE] = true;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_ATTACK_PLAYER_TWO] = false;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_PARTICLES] = true;
	matrix[COLLIDER_ATTACK_PLAYER_TWO][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_PARTICLES][COLLIDER_PLAYER_ONE] = true;
	matrix[COLLIDER_PARTICLES][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_PARTICLES][COLLIDER_ATTACK_PLAYER_ONE] = true;
	matrix[COLLIDER_PARTICLES][COLLIDER_PLAYER_TWO] = true;
	matrix[COLLIDER_PARTICLES][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_PARTICLES][COLLIDER_ATTACK_PLAYER_TWO] = true;
	matrix[COLLIDER_PARTICLES][COLLIDER_PARTICLES] = true;
	matrix[COLLIDER_PARTICLES][COLLIDER_OTHERS] = false;

	matrix[COLLIDER_OTHERS][COLLIDER_PLAYER_ONE] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_BODY_PLAYER_ONE] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_ATTACK_PLAYER_ONE] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_PLAYER_TWO] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_BODY_PLAYER_TWO] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_ATTACK_PLAYER_TWO] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_PARTICLES] = false;
	matrix[COLLIDER_OTHERS][COLLIDER_OTHERS] = false;

	show_colliders = false;
}


ModuleCollisions::~ModuleCollisions()
{
}

UpdateStatus ModuleCollisions::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		if (show_colliders)
			show_colliders = false;
		else
			show_colliders = true;
	}
	return UPDATE_CONTINUE;
}

UpdateStatus ModuleCollisions::Update()
{

	for (list<Collider*>::iterator it1 = colliders.begin(); it1 != colliders.end(); ++it1)
	{
		list<Collider*>::iterator it2 = it1;
		++it2;
		while (it2 != colliders.end())
		{
			if (CanCollide((*it1)->type, (*it2)->type) && IsCollision((*it1)->rect, (*it2)->rect))
			{
				(*it1)->module->OnCollision((*it1), (*it2));
				(*it2)->module->OnCollision((*it2), (*it1));
				if ((*it1)->type == COLLIDER_PARTICLES)
					(*it1)->needDelete = true;
				if ((*it2)->type == COLLIDER_PARTICLES)
					(*it2)->needDelete = true;

			}
			++it2;
		}
	}
	
	return UPDATE_CONTINUE;
}

UpdateStatus ModuleCollisions::PostUpdate()
{
	if (show_colliders)
	{
		for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		{
			if ((*it)->type == COLLIDER_PARTICLES || (*it)->type == COLLIDER_ATTACK_PLAYER_ONE || (*it)->type == COLLIDER_ATTACK_PLAYER_TWO)
				SDL_SetRenderDrawColor(App->renderer->renderer, 255, 0, 0, 100);
			else if ((*it)->type == COLLIDER_PLAYER_ONE || (*it)->type == COLLIDER_PLAYER_TWO)
				SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 255, 100);
			else if ((*it)->type == COLLIDER_BODY_PLAYER_ONE || (*it)->type == COLLIDER_BODY_PLAYER_TWO)
				SDL_SetRenderDrawColor(App->renderer->renderer, 0, 255, 0, 100);
			if (!App->renderer->DrawRect(&(*it)->rect))
				return UPDATE_ERROR;
		}
	}

	list<Collider*>::iterator it = colliders.begin();
	while (it != colliders.end())
	{
		if ((*it)->needDelete)
			it = colliders.erase(it);
		else
			++it;
	}
	return UPDATE_CONTINUE;
}

void ModuleCollisions::AddCollider(SDL_Rect rec, ColliderType type, Module* module)
{
	Collider collider = { rec, type, module };
	colliders.push_back(&collider);
}

void ModuleCollisions::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
}

bool ModuleCollisions::IsCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if (!(rec2.x > rec1.x + rec1.w
		|| rec2.x + rec2.w < rec1.x
		|| rec2.y > rec1.y + rec1.h
		|| rec2.y + rec2.h < rec1.y))
		return true;
	else
		return false;
}

bool ModuleCollisions::CanCollide(ColliderType type1, ColliderType type2)
{
	return matrix[type1][type2];
}