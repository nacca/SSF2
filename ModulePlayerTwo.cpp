#include "ModulePlayerTwo.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "Collider.h"
#include "Module.h"
#include "ModulePlayerOne.h"
#include <iostream>
#include "ModuleRender.h"

using namespace std;

ModulePlayerTwo::ModulePlayerTwo(bool start_enabled) : ModulePlayerDhalsim(start_enabled)
{

	position.x = 300;
	position.y = 200;

	collider_head = new Collider();
	collider_body = new Collider();
	collider_legs = new Collider();
	collider_attack = new Collider();

	collider_head->type = COLLIDER_PLAYER_TWO;
	collider_head->module = this;

	collider_body->type = COLLIDER_PLAYER_TWO;
	collider_body->module = this;

	collider_legs->type = COLLIDER_PLAYER_TWO;
	collider_legs->module = this;

	collider_attack->type = COLLIDER_ATTACK_PLAYER_TWO;
	collider_attack->module = this;

	looking_right = false;

}

ModulePlayerTwo::~ModulePlayerTwo()
{
}

// Load assets
bool ModulePlayerTwo::Start()
{
	LOG("Loading Dhalsim");

	graphics = App->textures->Load("dhalsim.png"); // arcade version
	App->collisions->AddCollider(collider_head);
	App->collisions->AddCollider(collider_body);
	App->collisions->AddCollider(collider_legs);
	App->collisions->AddCollider(collider_attack);

	if (App->player_one->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;


	return true;
}

// Unload assets
bool ModulePlayerTwo::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

update_status ModulePlayerTwo::PreUpdate()
{
	if (App->player_one->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	if (playerState == PLAYER_KO)
		return UPDATE_CONTINUE;

	if (hitted && playerState == PLAYER_CROUCHING)
	{
		playerState = PLAYER_CROUCH_HIT;
	}
	else if (head_hitted && playerState != PLAYER_CROUCH_HIT)
	{
		playerState = PLAYER_FACE_HIT;
	}
	else if (hitted && playerState != PLAYER_CROUCH_HIT && playerState != PLAYER_FACE_HIT)
	{
		playerState = PLAYER_HIT;
	}

	switch (playerState)
	{
	case PLAYER_IDLE:

		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT)
		{
			if (!looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX) == KEY_REPEAT)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}
		if (App->input->GetButton(12) == KEY_DOWN)
		{
			playerState = PLAYER_LEFT_PUNCH;
		}
		if (App->input->GetButton(10) == KEY_DOWN)
		{
			playerState = PLAYER_LEFT_KICK;
		}

		break;

	case PLAYER_WALKING_FORWARD:

		if (!((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX))))
		{
			playerState = PLAYER_IDLE;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT)
		{
			if (!looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX) == KEY_REPEAT)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}

		break;

	case PLAYER_WALKING_BACKWARD:

		if (!((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY)) ||
			(App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX))))
		{
			playerState = PLAYER_IDLE;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_BACKWARD;
			else
				playerState = PLAYER_WALKING_FORWARD;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX) == KEY_REPEAT)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}

		break;

	case PLAYER_JUMPING_STATIC:
		playerState = PLAYER_IDLE;
		break;

	case PLAYER_CROUCHING:
		if (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) != KEY_REPEAT)
		{
			playerState = PLAYER_IDLE;
			crouching.RestartFrames();
		}
		break;

	case PLAYER_LEFT_PUNCH:
		if (L_punch.IsEnded())
		{
			L_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_LEFT_KICK:
		if (L_kick.IsEnded())
		{
			L_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIT:

		if (hit.IsEnded())
		{
			hit.RestartFrames();
			playerState = PLAYER_IDLE;
			hitted = false;
			head_hitted = false;
		}
		break;

	case PLAYER_CROUCH_HIT:
		if (crouch_hit.IsEnded())
		{
			crouch_hit.RestartFrames();
			playerState = PLAYER_CROUCHING;
			hitted = false;
			head_hitted = false;
		}
		break;

	case PLAYER_FACE_HIT:
		if (face_hit.IsEnded())
		{
			face_hit.RestartFrames();
			playerState = PLAYER_IDLE;
			hitted = false;
			head_hitted = false;
		}
		break;
	}

	return UPDATE_CONTINUE;
}

// Update
update_status ModulePlayerTwo::Update()
{

	SDL_Rect aux;
	int pivot;
	Collider_player_structure cps;

	switch (playerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		cps = idle.GetCurrentCollider();
		break;

	case PLAYER_WALKING_FORWARD:
		if (!colliding_players)
		{
			if (looking_right)
				position.x += 2;
			else
				position.x -= 2;
		}
		aux = forward.GetCurrentFrame();
		pivot = forward.GetCurrentPivot();
		cps = forward.GetCurrentCollider();
		break;

	case PLAYER_WALKING_BACKWARD:
		if (looking_right)
		{
			if (!App->renderer->ScreenLeftLimit())
			{
				if (!App->player_one->playerInCameraLimit())
				{
					position.x -= 1;
					if (App->renderer->camera.x <= -(collider_body->rect.x*SCREEN_SIZE))
						App->renderer->camera.x += SCREEN_SIZE;
				}
				else
				{
					if (App->renderer->camera.x > -(collider_body->rect.x*SCREEN_SIZE))
						position.x -= 1;
				}
			}
			else
			{
				if (App->renderer->camera.x > -(collider_body->rect.x*SCREEN_SIZE))
					position.x -= 1;
			}
		}
		else
		{
			if (!App->renderer->ScreenRightLimit())
			{
				if (!App->player_one->playerInCameraLimit())
				{
					position.x += 1;
					if (App->renderer->camera.x - App->renderer->camera.w >= -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
						App->renderer->camera.x -= SCREEN_SIZE;
				}
				else
				{
					if (App->renderer->camera.x - App->renderer->camera.w < -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
						position.x += 1;
				}
			}
			else
			{
				if (App->renderer->camera.x - App->renderer->camera.w < -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
					position.x += 1;
			}
		}
		aux = backward.GetCurrentFrame();
		pivot = backward.GetCurrentPivot();
		cps = backward.GetCurrentCollider();
		break;

	case PLAYER_CROUCHING:
		aux = crouching.GetCurrentFrame();
		pivot = crouching.GetCurrentPivot();
		cps = crouching.GetCurrentCollider();
		break;

	case PLAYER_JUMPING_STATIC:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		cps = idle.GetCurrentCollider();
		break;

	case PLAYER_LEFT_PUNCH:
		aux = L_punch.GetCurrentFrame();
		pivot = L_punch.GetCurrentPivot();
		cps = L_punch.GetCurrentCollider();
		break;

	case PLAYER_LEFT_KICK:
		aux = L_kick.GetCurrentFrame();
		pivot = L_kick.GetCurrentPivot();
		cps = L_kick.GetCurrentCollider();
		break;

	case PLAYER_HIT:
		aux = hit.GetCurrentFrame();
		pivot = hit.GetCurrentPivot();
		cps = hit.GetCurrentCollider();
		break;

	case PLAYER_CROUCH_HIT:
		aux = crouch_hit.GetCurrentFrame();
		pivot = crouch_hit.GetCurrentPivot();
		cps = crouch_hit.GetCurrentCollider();
		break;

	case PLAYER_FACE_HIT:
		aux = face_hit.GetCurrentFrame();
		pivot = face_hit.GetCurrentPivot();
		cps = face_hit.GetCurrentCollider();
		break;

	case PLAYER_KO:
		if (!ko.IsEnded())
		{
			if (looking_right)
			{
				if (!App->renderer->ScreenLeftLimit())
				{
					if (!App->player_one->playerInCameraLimit())
					{
						position.x -= 1;
						if (App->renderer->camera.x <= -(collider_body->rect.x*SCREEN_SIZE))
							App->renderer->camera.x += SCREEN_SIZE;
					}
					else
					{
						if (App->renderer->camera.x > -(collider_body->rect.x*SCREEN_SIZE))
							position.x -= 1;
					}
				}
				else
				{
					if (App->renderer->camera.x > -(collider_body->rect.x*SCREEN_SIZE))
						position.x -= 1;
				}
			}
			else
			{
				if (!App->renderer->ScreenRightLimit())
				{
					if (!App->player_one->playerInCameraLimit())
					{
						position.x += 1;
						if (App->renderer->camera.x - App->renderer->camera.w >= -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
							App->renderer->camera.x -= SCREEN_SIZE;
					}
					else
					{
						if (App->renderer->camera.x - App->renderer->camera.w < -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
							position.x += 1;
					}
				}
				else
				{
					if (App->renderer->camera.x - App->renderer->camera.w < -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
						position.x += 1;
				}
			}
		}
		aux = ko.GetCurrentFrame();
		pivot = ko.GetCurrentPivot();
		cps = ko.GetCurrentCollider();
		break;
	}

	collider_head->rect = cps.Collider_head;
	collider_body->rect = cps.Collider_body;
	collider_legs->rect = cps.Collider_legs;
	collider_attack->rect = cps.Collider_attack;

	if (looking_right)
	{
		collider_head->rect.x += position.x;
		collider_head->rect.y += position.y;
		collider_body->rect.x += position.x;
		collider_body->rect.y += position.y;
		collider_legs->rect.x += position.x;
		collider_legs->rect.y += position.y;
		collider_attack->rect.x += position.x;
		collider_attack->rect.y += position.y;
		App->renderer->Blit(graphics, position.x - pivot, position.y - aux.h, &(aux), 1.0f, SDL_FLIP_NONE);
	}
	else
	{
		collider_head->rect.x = position.x - collider_head->rect.x - collider_head->rect.w;
		collider_head->rect.y += position.y;
		collider_body->rect.x = position.x - collider_body->rect.x - collider_body->rect.w;
		collider_body->rect.y += position.y;
		collider_legs->rect.x = position.x - collider_legs->rect.x - collider_legs->rect.w;
		collider_legs->rect.y += position.y;
		collider_attack->rect.x = position.x - collider_attack->rect.x - collider_attack->rect.w;
		collider_attack->rect.y += position.y;
		App->renderer->Blit(graphics, position.x - aux.w + pivot, position.y - aux.h, &(aux), 1.0f, SDL_FLIP_HORIZONTAL);
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		iPoint particlePosition(15, 150);
		SDL_Rect rec = { 0, 142, 30, 16 };
		App->particles->newParticle(particlePosition, rec, graphics, particula, 1);
	}

	return UPDATE_CONTINUE;
}

update_status ModulePlayerTwo::PostUpdate()
{
	colliding_players = false;
	return UPDATE_CONTINUE;
}