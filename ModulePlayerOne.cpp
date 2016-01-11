#include "ModulePlayerOne.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "Module.h"
#include "ModulePlayerTwo.h"
#include "ModuleRender.h"
#include "ModuleSceneBison.h"
#include "ModuleAudio.h"

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


update_status ModulePlayerOne::PreUpdate()
{
	bool near = false;

	if (otherPlayer->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	if ((otherPlayer->getPosition().x - position.x) < 40 &&
		(otherPlayer->getPosition().x - position.x) > -40)
		near = true;

	if (dead)
	{
		if (playerState != PLAYER_KO)
		{
			playerState = PLAYER_KO;
			otherPlayer->win = true;

		}
		return UPDATE_CONTINUE;
	}
	else if (time_0)
	{
		playerState = PLAYER_TIME_OUT;
	}
	else if (win && 
		(playerState == PLAYER_IDLE ||
		playerState == PLAYER_CROUCHING ||
		playerState == PLAYER_WALKING_FORWARD ||
		playerState == PLAYER_WALKING_BACKWARD))
	{
		if (playerState != PLAYER_WIN_1 && playerState != PLAYER_WIN_2)
		{
			if (wins == 0)
			{
				++wins;
				playerState = PLAYER_WIN_1;
			}
			else
			{
				playerState = PLAYER_WIN_2;
			}
		}
		return UPDATE_CONTINUE;
	}
	else if (playerState == PLAYER_BLOCKING_HITTED || playerState == PLAYER_CROUCH_BLOCKING_HITTED);
	else if (hitted && playerState == PLAYER_BLOCKING)
	{
		playerState = PLAYER_BLOCKING_HITTED;
		block.RestartFrames();
	}
	else if (hitted && playerState == PLAYER_CROUCH_BLOCKING)
	{
		playerState = PLAYER_CROUCH_BLOCKING_HITTED;
		crouch_block.RestartFrames();
	}
	else if ((hitted || head_hitted) && (playerState == PLAYER_JUMPING || playerState == PLAYER_JUMP_PUNCH || playerState == PLAYER_JUMP_KICK || playerState == PLAYER_AIR_HITTED))
	{
		playerState = PLAYER_AIR_HITTED;
	}
	else if ((hitted || head_hitted) && (playerState == PLAYER_CROUCHING || playerState == PLAYER_CROUCH_PUNCH || playerState == PLAYER_CROUCH_KICK))
	{
		playerState = PLAYER_CROUCH_HIT;
	}
	else if (head_hitted)
	{
		playerState = PLAYER_FACE_HIT;
	}
	else if (hitted)
	{
		playerState = PLAYER_HIT;
	}

	switch (playerState)
	{

	case PLAYER_IDLE:
		
		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			looking_right)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			looking_right && otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
			looking_right)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_STATIC;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		break;

	case PLAYER_WALKING_FORWARD:
		
		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (starting_combo == AERIAL_COMBO_PUNCH)
		{
			playerState = PLAYER_YOGA_MUMMY;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (!looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		else
			playerState = PLAYER_IDLE;

		if (near && starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		break;

	case PLAYER_WALKING_BACKWARD:

		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT &&
			App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (looking_right)
				playerState = PLAYER_WALKING_BACKWARD;
			else
				playerState = PLAYER_WALKING_FORWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			playerState = PLAYER_CROUCHING;
		}
		else
		{
			playerState = PLAYER_IDLE;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}

		break;

	case PLAYER_JUMPING:

		if (distance_jumped == 0 && !going_up)
		{
			playerState = PLAYER_IDLE;
			jump_attacked = false;
			jump.RestartFrames();
		}
		else if (starting_combo == AERIAL_COMBO_PUNCH && !jump_attacked)
		{
			jump_attacked = true;
			playerState = PLAYER_YOGA_MUMMY;
			if (looking_right) 
				directionMummy = JUMP_RIGHT;
			else 
				directionMummy = JUMP_LEFT;
		}
		else if (starting_combo == AERIAL_COMBO_KICK && !jump_attacked)
		{
			jump_attacked = true;
			playerState = PLAYER_YOGA_SPEAR;
			if (looking_right)
				directionMummy = JUMP_RIGHT;
			else
				directionMummy = JUMP_LEFT;
		}
		else
		{
			if (((App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)) &&
				!jump_attacked)
			{
				jump_attacked = true;
				playerState = PLAYER_JUMP_PUNCH;
			}

			else if (((App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)) &&
				!jump_attacked)
			{
				jump_attacked = true;
				playerState = PLAYER_JUMP_KICK;
			}
		}
		break;


	case PLAYER_JUMP_PUNCH:

		if (distance_jumped == 0)
		{
			jump_attacked = false;
			playerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_punch.RestartFrames();
		}
		else if (jump_punch.IsEnded())
		{
			jump_punch.RestartFrames();
			playerState = PLAYER_JUMPING;
		}
		break;

	case PLAYER_JUMP_KICK:
		if (distance_jumped == 0)
		{
			jump_attacked = false;
			playerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_kick.RestartFrames();
		}
		else if (jump_kick.IsEnded())
		{
			jump_kick.RestartFrames();
			playerState = PLAYER_JUMPING;
		}
		break;


	case PLAYER_CROUCHING:

		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			&& otherPlayer->IsAttacking() && !looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			&& otherPlayer->IsAttacking() && looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
			(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
			(App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN))
			playerState = PLAYER_CROUCH_PUNCH;

		else if ((App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) ||
			(App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
			(App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
			playerState = PLAYER_CROUCH_KICK;

		else if (!(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT))
		{
			playerState = PLAYER_IDLE;
			crouching.RestartFrames();
		}
		break;

	case PLAYER_LOW_PUNCH:

		if (L_punch.IsEnded())
		{
			L_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_LOW_KICK:

		if (L_kick.IsEnded())
		{
			L_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_PUNCH:

		if (M_punch.IsEnded())
		{
			M_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_KICK:

		if (M_kick.IsEnded())
		{
			M_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_PUNCH:

		if (H_punch.IsEnded())
		{
			H_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_KICK:

		if (H_kick.IsEnded())
		{
			H_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_CROUCH_PUNCH:

		if (crouch_punch.IsEnded())
		{
			crouch_punch.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_CROUCH_KICK:

		if (crouch_kick.IsEnded())
		{
			crouch_kick.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_FORWARD_LOW_PUNCH:

		if (F_L_punch.IsEnded())
		{
			F_L_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_LOW_KICK:

		if (F_L_kick.IsEnded())
		{
			F_L_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:

		if (F_M_punch.IsEnded())
		{
			F_M_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:

		if (F_M_kick.IsEnded())
		{
			F_M_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:

		if (F_H_punch.IsEnded())
		{
			F_H_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_KICK:

		if (F_H_kick.IsEnded())
		{
			F_H_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIT:
		
		if (hit.IsEnded())
		{
			hit.RestartFrames();
			if (distance_jumped > 0){
				going_up = false;
				playerState = PLAYER_JUMPING;
			}
			else
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

	case PLAYER_YOGA_FIRE:
		if (yoga_fire.GetCurrentFrameNumber() == 3 && SDL_GetTicks() - lastShotTimer > 1000)
		{
			if (looking_right)
			{
				iPoint particlePosition(position.x + 42 + 14, position.y - 54 + 10);
				App->particles->newParticle(particlePosition, graphics, particula, destroy_particula, 2);
			}
			else
			{
				iPoint particlePosition(position.x - 42 - 14, position.y - 54 + 10);
				App->particles->newParticle(particlePosition, graphics, particula, destroy_particula, -2);
			}
			lastShotTimer = SDL_GetTicks();
		}

		if (yoga_fire.IsEnded())
		{
			yoga_fire.RestartFrames();
			playerState = PLAYER_IDLE;			
		}
		break;

	case PLAYER_YOGA_FLAME:

		if (yoga_flame.IsEnded())
		{
			yoga_flame.RestartFrames();
			playerState = PLAYER_IDLE;

		}
		break;

	case PLAYER_YOGA_MUMMY:

		if (yoga_mummy.IsEnded() && distance_jumped > 0)
		{
			yoga_mummy.RestartFrames();
			playerState = PLAYER_JUMPING;
			jump_attacked = true;
		}
		else if (yoga_mummy.IsEnded())
		{
			yoga_mummy.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		break;

	case PLAYER_YOGA_SPEAR:

		if (yoga_spear.IsEnded() && distance_jumped > 0)
		{
			yoga_spear.RestartFrames();
			playerState = PLAYER_JUMPING;
			jump_attacked = true;
		}
		else if (yoga_spear.IsEnded())
		{
			yoga_spear.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		break;

	case PLAYER_BLOCKING:
		if (!otherPlayer->IsAttacking())
		{
			block.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		if (block.IsEnded())
			block.RestartFrames();
		break;

	case PLAYER_CROUCH_BLOCKING:
		if (!otherPlayer->IsAttacking())
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		if (crouch_block.IsEnded())
			crouch_block.RestartFrames();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			block.RestartFrames();
			playerState = PLAYER_BLOCKING;
		}
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (crouch_block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCH_BLOCKING;
		}
		break;

	case PLAYER_AIR_HITTED:
		if (air_hit.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			air_hit.RestartFrames();
			going_up = false;
			playerState = PLAYER_JUMPING;
		}
		break;
	}

	starting_combo = COMBO_NOTHING;

	return UPDATE_CONTINUE;
}

void ModulePlayerOne::restartPlayer()
{
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
}
