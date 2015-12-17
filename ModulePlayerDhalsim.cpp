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

ModulePlayerDhalsim::ModulePlayerDhalsim(bool start_enabled) : ModulePlayer(start_enabled)
{
	collider.rect.x = 185;
	collider.rect.y = 110;
	collider.rect.w = 30;
	collider.rect.h = 90;
	collider.type = PLAYER;
	collider.module = this;

	position.x = 200;
	position.y = 200;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({ 3, 23, 52, 87 });
	idle.frames.push_back({ 59, 24, 49, 86 });
	idle.frames.push_back({ 113, 29, 55, 81 });
	idle.frames.push_back({ 174, 24, 52, 86 });
	idle.frames.push_back({ 231, 23, 49, 87 });
	idle.frames.push_back({ 284, 28, 54, 82 });
	idle.frames.push_back({ 343, 24, 52, 86 });
	idle.frames.push_back({ 399, 25, 49, 85 });
	idle.frames.push_back({ 454, 28, 54, 82 });
	idle.speed = 0.1f;

	forward.frames.push_back({ 516, 25, 51, 85 });
	forward.frames.push_back({ 572, 22, 50, 88 });
	forward.frames.push_back({ 626, 20, 52, 90 });
	forward.frames.push_back({ 683, 22, 53, 88 });
	forward.frames.push_back({ 742, 23, 49, 87 });
	forward.frames.push_back({ 797, 22, 52, 88 });
	forward.frames.push_back({ 853, 20, 52, 90 });
	forward.frames.push_back({ 909, 22, 50, 88 });
	forward.speed = 0.1f;

	backward.frames.push_back({ 516, 25, 51, 85 });
	backward.frames.push_back({ 572, 22, 50, 88 });
	backward.frames.push_back({ 626, 20, 52, 90 });
	backward.frames.push_back({ 683, 22, 53, 88 });
	backward.frames.push_back({ 742, 23, 49, 87 });
	backward.frames.push_back({ 797, 22, 52, 88 });
	backward.frames.push_back({ 853, 20, 52, 90 });
	backward.frames.push_back({ 909, 22, 50, 88 });
	backward.speed = 0.075f;

	particula.frames.push_back({ 627, 519, 27, 17 });
	particula.frames.push_back({ 663, 519, 26, 17 });
	particula.frames.push_back({ 698, 520, 26, 14 });
	particula.speed = 0.1f;

	jumping = false;

	playerState = PLAYER_IDLE;

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
	App->collisions->AddCollider(&collider);

	return true;
}

// Unload assets
bool ModulePlayerDhalsim::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update
update_status ModulePlayerDhalsim::Update()
{
	switch (playerState)
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
			(App->input->getYdir() == 0))
			playerState = PLAYER_IDLE;
		break;


	}

	SDL_Rect aux;

	switch (playerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		break;

	case PLAYER_WALKING_FORWARD:
		collider.rect.x += 2;
		position.x += 2;
		aux = forward.GetCurrentFrame();
		break;

	case PLAYER_WALKING_BACKWARD:
		collider.rect.x -= 1;
		position.x -= 1;
		aux = backward.GetCurrentFrame();
		break;

	case PLAYER_CROUCHING:
		aux = idle.GetCurrentFrame();
		break;

	case PLAYER_JUMPING_STATIC:
		aux = idle.GetCurrentFrame();
		break;

	}

	App->renderer->Blit(graphics, position.x - 25, position.y - aux.h, &(aux), 1.0f);

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		iPoint particlePosition(15, 150);
		SDL_Rect rec = { 0, 142, 30, 16 };
		App->particles->newParticle(particlePosition, rec, graphics, particula, 1);
	}

	return UPDATE_CONTINUE;
}