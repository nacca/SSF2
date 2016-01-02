#include "ModulePlayerDhalsim.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "Collider.h"

#include <iostream>

ModulePlayerDhalsim::ModulePlayerDhalsim(bool start_enabled) : Module(start_enabled)
{

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({ 3, 23, 52, 87 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 59, 24, 49, 86 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 113, 29, 55, 81 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 174, 24, 52, 86 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 231, 23, 49, 87 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 284, 28, 54, 82 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 343, 24, 52, 86 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 399, 25, 49, 85 }); idle.pivots.push_back(25);
	idle.frames.push_back({ 454, 28, 54, 82 }); idle.pivots.push_back(25);
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.colliders.push_back({ { 3, -81, 10, 12 }, { -6, -69, 19, 21 }, { -12, -50, 25, 48 }, { 0, 0, 0, 0 }, false });
	idle.speed = 0.1f;

	forward.frames.push_back({ 516, 25, 51, 85 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 572, 22, 50, 88 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 626, 20, 52, 90 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 683, 22, 53, 88 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 742, 23, 49, 87 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 797, 22, 52, 88 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 853, 20, 52, 90 }); forward.pivots.push_back(25);
	forward.frames.push_back({ 909, 22, 50, 88 }); forward.pivots.push_back(25);
	forward.colliders.push_back({ { 0, -78, 12, 13 }, { -6, -65, 17, 18 }, { -18, -46, 36, 45 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { -1, -80, 12, 13 }, { -9, -67, 15, 18 }, { -18, -47, 34, 46 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { -1, -82, 12, 13 }, { -9, -69, 17, 18 }, { -10, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { 1, -80, 12, 13 }, { -8, -67, 23, 17 }, { -8, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { -1, -78, 12, 13 }, { -11, -66, 23, 18 }, { -21, -48, 34, 47 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { 2, -80, 12, 13 }, { -7, -67, 22, 18 }, { -7, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { -1, -82, 12, 13 }, { -9, -69, 21, 18 }, { -10, -52, 19, 51 }, { 0, 0, 0, 0 }, false });
	forward.colliders.push_back({ { -1, -80, 12, 13 }, { -8, -67, 19, 17 }, { -13, -50, 28, 48 }, { 0, 0, 0, 0 }, false });
	forward.speed = 0.1f;

	backward.frames.push_back({ 516, 25, 51, 85 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 572, 22, 50, 88 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 626, 20, 52, 90 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 683, 22, 53, 88 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 742, 23, 49, 87 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 797, 22, 52, 88 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 853, 20, 52, 90 }); backward.pivots.push_back(25);
	backward.frames.push_back({ 909, 22, 50, 88 }); backward.pivots.push_back(25);
	backward.colliders.push_back({ { 0, -78, 12, 13 }, { -6, -65, 17, 18 }, { -18, -46, 36, 45 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { -1, -80, 12, 13 }, { -9, -67, 15, 18 }, { -18, -47, 34, 46 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { -1, -82, 12, 13 }, { -9, -69, 17, 18 }, { -10, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { 1, -80, 12, 13 }, { -8, -67, 23, 17 }, { -8, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { -1, -78, 12, 13 }, { -11, -66, 23, 18 }, { -21, -48, 34, 47 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { 2, -80, 12, 13 }, { -7, -67, 22, 18 }, { -7, -50, 19, 49 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { -1, -82, 12, 13 }, { -9, -69, 21, 18 }, { -10, -52, 19, 51 }, { 0, 0, 0, 0 }, false });
	backward.colliders.push_back({ { -1, -80, 12, 13 }, { -8, -67, 19, 17 }, { -13, -50, 28, 48 }, { 0, 0, 0, 0 }, false });
	backward.speed = 0.075f;

	crouching.frames.push_back({ 1144, 42, 45, 68 }); crouching.pivots.push_back(25);
	crouching.frames.push_back({ 1194, 52, 45, 58 }); crouching.pivots.push_back(25);
	crouching.colliders.push_back({ { 4, -67, 11, 13 }, { -8, -54, 22, 17 }, { -7, -36, 30, 35 }, { 0, 0, 0, 0 }, false });
	crouching.colliders.push_back({ { 4, -57, 11, 13 }, { -8, -44, 24, 17 }, { -15, -27, 35, 26 }, { 0, 0, 0, 0 }, false });
	crouching.speed = 0.15f;
	crouching.loop = false;

	particula.frames.push_back({ 627, 519, 27, 17 }); particula.pivots.push_back(25);
	particula.frames.push_back({ 663, 519, 26, 17 }); particula.pivots.push_back(25);
	particula.frames.push_back({ 698, 520, 26, 14 }); particula.pivots.push_back(25);
	particula.speed = 0.1f;

	L_punch.frames.push_back({ 2, 170, 61, 78 }); L_punch.pivots.push_back(25);
	L_punch.frames.push_back({ 68, 175, 91, 73 }); L_punch.pivots.push_back(25);
	L_punch.frames.push_back({ 166, 170, 61, 78 }); L_punch.pivots.push_back(25);
	L_punch.speed = 0.1f;
	L_punch.loop = false;
	L_punch.colliders.push_back({ { 13, -77, 11, 12 }, { 0, -65, 25, 23 }, { -15, -44, 41, 42 }, { 0, 0, 0, 0 }, false });
	L_punch.colliders.push_back({ { 15, -72, 11, 12 }, { 1, -64, 27, 26 }, { -14, -43, 42, 43 }, { 25, -65, 41, 12 }, true });
	L_punch.colliders.push_back({ { 13, -77, 11, 12 }, { 0, -65, 25, 23 }, { -15, -44, 41, 42 }, { 0, 0, 0, 0 }, false });

	L_kick.frames.push_back({ 2, 283, 51, 84 }); L_kick.pivots.push_back(25);
	L_kick.frames.push_back({ 59, 269, 88, 98 }); L_kick.pivots.push_back(25);
	L_kick.frames.push_back({ 153, 269, 116, 98 }); L_kick.pivots.push_back(25);
	L_kick.frames.push_back({ 59, 269, 88, 98 }); L_kick.pivots.push_back(25);
	L_kick.frames.push_back({ 2, 283, 51, 84 }); L_kick.pivots.push_back(25);
	L_kick.speed = 0.1f;
	L_kick.loop = false;
	L_kick.colliders.push_back({ { 16, -83, 10, 15 }, { -4, -83, 22, 35 }, { -8, -51, 24, 48 }, { 0, 0, 0, 0 }, false });
	L_kick.colliders.push_back({ { 10, -97, 10, 15 }, { 1, -82, 23, 26 }, { 3, -57, 21, 55 }, { 24, -81, 39, 16 }, true });
	L_kick.colliders.push_back({ { 10, -97, 10, 15 }, { 2, -82, 21, 27 }, { 3, -56, 16, 55 }, { 52, -90, 39, 16 }, false });
	L_kick.colliders.push_back({ { 10, -97, 10, 15 }, { 1, -82, 23, 26 }, { 3, -57, 21, 55 }, { 24, -81, 39, 16 }, true });
	L_kick.colliders.push_back({ { 16, -83, 10, 15 }, { -4, -83, 22, 35 }, { -8, -51, 24, 48 }, { 0, 0, 0, 0 }, false });

	hit.frames.push_back({ 7, 731, 49, 81 }); hit.pivots.push_back(25);
	hit.frames.push_back({ 64, 725, 49, 87 }); hit.pivots.push_back(25);
	hit.frames.push_back({ 117, 723, 49, 89 }); hit.pivots.push_back(25);
	hit.loop = false;
	hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	hit.speed = 0.1f;

	face_hit.frames.push_back({ 175, 728, 49, 84 }); face_hit.pivots.push_back(25);
	face_hit.frames.push_back({ 227, 725, 51, 87 }); face_hit.pivots.push_back(27);
	face_hit.frames.push_back({ 282, 725, 61, 87 }); face_hit.pivots.push_back(36);
	face_hit.loop = false;
	face_hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	face_hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	face_hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	face_hit.speed = 0.1f;

	crouch_hit.frames.push_back({ 350, 749, 45, 63 }); crouch_hit.pivots.push_back(22);
	crouch_hit.loop = false;
	crouch_hit.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	crouch_hit.speed = 0.033f;

	ko.frames.push_back({ 932, 755, 74, 57 }); ko.pivots.push_back(58);
	ko.frames.push_back({ 1011, 783, 83, 29 }); ko.pivots.push_back(58);
	ko.frames.push_back({ 1098, 755, 74, 57 }); ko.pivots.push_back(58);
	ko.frames.push_back({ 1177, 783, 83, 29 }); ko.pivots.push_back(58);
	ko.loop = false;
	ko.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	ko.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	ko.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	ko.colliders.push_back({ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, false });
	ko.speed = 0.1f;

	playerState = PLAYER_IDLE;
	colliding_players = false;
	jumping = false;
	hitted = false;
	head_hitted = false;
	life = 89;

}

ModulePlayerDhalsim::~ModulePlayerDhalsim()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayerDhalsim::Start()
{
	LOG("Loading Dhalsim");

	graphics = App->textures->Load("dhalsim.png"); // arcade version
//	App->collisions->AddCollider(&collider);

	return true;
}

// Unload assets
bool ModulePlayerDhalsim::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

update_status ModulePlayerDhalsim::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModulePlayerDhalsim::Update()
{
/*	switch (playerState)
	{
	case PLAYER_IDLE:

		if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) ||
			App->input->getXdir() == 1)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||
			App->input->getXdir() == -1)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) ||
			App->input->getYdir() == -1)
		{
			playerState = PLAYER_CROUCHING;
		}
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ||
			App->input->getYdir() == 1)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}

		break;

	case PLAYER_WALKING_FORWARD:

		if (!((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) ||
			!((App->input->getXdir() == 1) ||
			(App->input->getXdir() == -1) ||
			(App->input->getYdir() == 1) ||
			(App->input->getYdir() == -1)))
		{
			playerState = PLAYER_IDLE;
		}
		if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) ||
			App->input->getXdir() == 1)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||
			App->input->getXdir() == -1)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) ||
			App->input->getYdir() == -1)
		{
			playerState = PLAYER_CROUCHING;
		}
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ||
			App->input->getYdir() == 1)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}

		break;

	case PLAYER_WALKING_BACKWARD:

		if (!((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ||
			(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)) ||
			!((App->input->getXdir() == 1) ||
			(App->input->getXdir() == -1) ||
			(App->input->getYdir() == 1) ||
			(App->input->getYdir() == -1)))
		{
			playerState = PLAYER_IDLE;
		}
		if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) ||
			App->input->getXdir() == 1)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||
			App->input->getXdir() == -1)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) ||
			App->input->getYdir() == -1)
		{
			playerState = PLAYER_CROUCHING;
		}
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) ||
			App->input->getYdir() == 1)
		{
			playerState = PLAYER_JUMPING_STATIC;
		}

		break;

	case PLAYER_JUMPING_STATIC:
		playerState = PLAYER_IDLE;
		break;

	case PLAYER_CROUCHING:
		if (!(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) &&
			(App->input->getYdir() != -1))
		{
			playerState = PLAYER_IDLE;
			crouching.RestartFrames();
		}
		break;
	}

	SDL_Rect aux;
	int pivot;

	switch (playerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		break;

	case PLAYER_WALKING_FORWARD:
		collider.rect.x += 2;
		position.x += 2;
		aux = forward.GetCurrentFrame();
		pivot = forward.GetCurrentPivot();
		break;

	case PLAYER_WALKING_BACKWARD:
		collider.rect.x -= 1;
		position.x -= 1;
		aux = backward.GetCurrentFrame();
		pivot = backward.GetCurrentPivot();
		break;

	case PLAYER_CROUCHING:
		aux = crouching.GetCurrentFrame();
		pivot = crouching.GetCurrentPivot();
		break;

	case PLAYER_JUMPING_STATIC:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		break;

	}

	App->renderer->Blit(graphics, position.x - aux.w + pivot, position.y - aux.h, &(aux), 1.0f);

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		iPoint particlePosition(15, 150);
		SDL_Rect rec = { 0, 142, 30, 16 };
		App->particles->newParticle(particlePosition, rec, graphics, particula, 1);
	}
	*/
	return UPDATE_CONTINUE;
}

update_status ModulePlayerDhalsim::PostUpdate()
{
	colliding_players = false;
	return UPDATE_CONTINUE;
}

iPoint ModulePlayerDhalsim::getPosition()
{
	return position;
}

void ModulePlayerDhalsim::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER_ONE && c2->type == COLLIDER_PLAYER_TWO)
		colliding_players = true;
	else if (c1->type == COLLIDER_PLAYER_TWO && c2->type == COLLIDER_PLAYER_ONE)
		colliding_players = true;
	else if (c1->type == COLLIDER_PLAYER_ONE && c2->type == COLLIDER_ATTACK_PLAYER_TWO)
	{
		if (!hitted)
			life -= 15;

		if (life < 0)
		{
			playerState = PLAYER_KO;
			life = 0;
		}

		hitted = true;
		if (c1 == collider_head)
			head_hitted = true;
	}
	else if (c1->type == COLLIDER_PLAYER_TWO && c2->type == COLLIDER_ATTACK_PLAYER_ONE)
	{
		if (!hitted)
			life -= 15;

		if (life < 0)
		{
			playerState = PLAYER_KO;
			life = 0;
		}

		hitted = true;
		if (c1 == collider_head)
			head_hitted = true;
	}
}

bool ModulePlayerDhalsim::playerInCameraLimit() const
{
	if (App->renderer->camera.x <= -(collider_body->rect.x*SCREEN_SIZE))
		return true;
	if (App->renderer->camera.x - App->renderer->camera.w >= -((collider_body->rect.x + collider_body->rect.w)*SCREEN_SIZE))
		return true;
	return false;
}