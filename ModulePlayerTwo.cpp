#include "ModulePlayerTwo.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModulePlayerOne.h"

ModulePlayerTwo::ModulePlayerTwo(bool start_enabled) : ModulePlayerDhalsim(start_enabled)
{

	position.x = 300;
	position.y = 200;

	player_collider.type = COLLIDER_BODY_PLAYER_TWO;
	player_collider.module = this;

	collider_head.type = COLLIDER_PLAYER_TWO;
	collider_head.module = this;

	collider_body.type = COLLIDER_PLAYER_TWO;
	collider_body.module = this;

	collider_legs.type = COLLIDER_PLAYER_TWO;
	collider_legs.module = this;

	collider_attack.type = COLLIDER_ATTACK_PLAYER_TWO;
	collider_attack.module = this;

}

ModulePlayerTwo::~ModulePlayerTwo()
{

}

// Load assets
bool ModulePlayerTwo::Start()
{
	LOG("Loading Dhalsim");

	graphics = App->textures->Load("dhalsim_recolor.png"); // arcade version

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
	
	otherPlayer = App->player_one;

	if (App->player_one->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;


	return true;
}

void ModulePlayerTwo::restartPlayer(bool everything)
{
	if (everything)
		wins = 0;

	position.x = 300;
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

bool ModulePlayerTwo::GetPlayerInput(input_type actionKey)
{
	switch (actionKey)
	{
	case INPUT_UP:
		return (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX) == KEY_REPEAT || App->input->yDir == -1 || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT);
		break;
	case INPUT_DOWN:
		return (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1 || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT);
		break;
	case INPUT_LEFT:
		return (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1 || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT);
		break;
	case INPUT_RIGHT:
		return (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1 || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT);
		break;
	case INPUT_L_PUNCH:
		return (App->input->GetButton(12) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN);
		break;
	case INPUT_L_KICK:
		return (App->input->GetButton(10) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN);
		break;
	case INPUT_M_PUNCH:
		return (App->input->GetButton(11) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN);
		break;
	case INPUT_M_KICK:
		return (App->input->GetButton(13) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_DOWN);
		break;
	case INPUT_H_PUNCH:
		return (App->input->GetButton(8) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN);
		break;
	case INPUT_H_KICK:
		return (App->input->GetButton(9) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_9) == KEY_DOWN);
		break;
	}
	return false;
}
