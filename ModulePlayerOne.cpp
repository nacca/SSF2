#include "ModulePlayerOne.h"
#include "Globals.h" 
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayerTwo.h"
#include <iostream>

ModulePlayerOne::ModulePlayerOne(bool start_enabled) : ModulePlayerDhalsim(start_enabled)
{

	position.x = 150;
	position.y = 200;
	
	player_collider.type = COLLIDER_BODY_PLAYER_ONE;
	player_collider.module = this;

	collider_head.type = COLLIDER_PLAYER_ONE;
	collider_head.module = this;

	collider_body.type = COLLIDER_PLAYER_ONE;
	collider_body.module = this;

	collider_legs.type = COLLIDER_PLAYER_ONE;
	collider_legs.module = this;

	collider_attack.type = COLLIDER_ATTACK_PLAYER_ONE;
	collider_attack.module = this;

}

ModulePlayerOne::~ModulePlayerOne()
{
}

// Load assets
bool ModulePlayerOne::Start()
{
	LOG("Loading Dhalsim");

	graphics = App->textures->Load("dhalsim.png"); // arcade version
	App->collisions->AddCollider(&player_collider);
	App->collisions->AddCollider(&collider_head);
	App->collisions->AddCollider(&collider_body);
	App->collisions->AddCollider(&collider_legs);
	App->collisions->AddCollider(&collider_attack);

	audio_id_yoga_fire = App->audio->LoadFx("yoga_fire.wav");
	audio_id_yoga_flame = App->audio->LoadFx("yoga_flame.wav");
	audio_id_dead = App->audio->LoadFx("dhalsim_dead.wav");
	audio_id_L_attack = App->audio->LoadFx("SF2_hit_1.wav");;
	audio_id_M_attack = App->audio->LoadFx("SF2_hit_2.wav");;
	audio_id_H_attack = App->audio->LoadFx("SF2_hit_3.wav");;
	audio_id_L_impact = App->audio->LoadFx("SF2_impact_1.wav");;
	audio_id_M_impact = App->audio->LoadFx("SF2_impact_2.wav");;
	audio_id_H_impact = App->audio->LoadFx("SF2_impact_3.wav");;

	otherPlayer = App->player_two;

	if (App->player_two->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	return true;
}


void ModulePlayerOne::restartPlayer(bool everything)
{
	if (everything)
		wins = 0;

	position.x = 150;
	position.y = 200;

	playerState = PLAYER_IDLE;
	colliding_players = false;
	jumping = false;
	hitted = false;
	head_hitted = false;
	life = 200;
	win = false;
	dead = false;
	time_0 = false;
	starting_combo = COMBO_NOTHING;

	ko.RestartFrames();
	victory1.RestartFrames();
	victory2.RestartFrames();

	distance_jumped = 0;
	jump_attacked = false;
	leg_hitted = false;
	already_hitted = false;
}

bool ModulePlayerOne::GetPlayerInput(input_type actionKey)
{
	switch (actionKey)
	{
		case INPUT_UP:
			return App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT;
			break;
		case INPUT_DOWN:
			return App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT;
			break;
		case INPUT_LEFT:
			return App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT;
			break;
		case INPUT_RIGHT:
			return App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT;
			break;
		case INPUT_L_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
			break;
		case INPUT_L_KICK:
			return App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN;
			break;
		case INPUT_M_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
			break;
		case INPUT_M_KICK:
			return App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
			break;
		case INPUT_H_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN;
			break;
		case INPUT_H_KICK:
			return App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN;
			break;
	}
	return false;
}
