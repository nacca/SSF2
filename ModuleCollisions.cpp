#include "ModuleCollisions.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL\include\SDL.h"
#include <queue>
#include "ModuleInput.h"

ModuleCollisions::ModuleCollisions(bool start_enabled) : Module(start_enabled)
{

}


ModuleCollisions::~ModuleCollisions()
{
}

bool ModuleCollisions::Start()
{
	show_colliders = false;
	return true;
}

update_status ModuleCollisions::Update()
{

	for (list<Collider*>::iterator it1 = colliders.begin(); it1 != colliders.end(); ++it1)
	{
		if (show_colliders)
		{
			if ((*it1)->type == PARTICLES)
				SDL_SetRenderDrawColor(App->renderer->renderer, 255, 0, 0, 100);
			else if ((*it1)->type == PLAYER)
				SDL_SetRenderDrawColor(App->renderer->renderer, 0, 0, 255, 100);
			if (!App->renderer->DrawRect(&(*it1)->rect))
				return UPDATE_ERROR;
		}
		list<Collider*>::iterator it2 = it1;
		++it2;
		while (it2 != colliders.end())
		{
			if (CanCollide((*it1)->type, (*it2)->type) && IsCollision((*it1)->rect, (*it2)->rect))
			{
				(*it1)->module->OnCollision((*it1), (*it2));
				(*it2)->module->OnCollision((*it2), (*it1));
				if ((*it1)->type == PARTICLES)
					(*it1)->needDelete = true;
				if ((*it2)->type == PARTICLES)
					(*it2)->needDelete = true;

			}
			++it2;
		}
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
			if (show_colliders)
				show_colliders = false;
			else
				show_colliders = true;
		}
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleCollisions::PostUpdate()
{
	queue<list<Collider*>::iterator> queueIterators;
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		if ((*it)->needDelete)
			queueIterators.push(it);
	}
	while (!queueIterators.empty())
	{
		list<Collider*>::iterator it = queueIterators.front();
		colliders.erase(it);
		queueIterators.pop();
	}
	return UPDATE_CONTINUE;
}

bool ModuleCollisions::CleanUp()
{
	return true;
}

void ModuleCollisions::AddCollider(SDL_Rect rec, collider_type type, Module* module)
{
	Collider collider(rec, type, module);
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

bool ModuleCollisions::CanCollide(collider_type type1, collider_type type2)
{
	if (type1 != type2)
		return true;
	else
		return false;
}