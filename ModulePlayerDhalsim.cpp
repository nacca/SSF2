#include "ModulePlayerDhalsim.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "ModuleSceneBison.h"
#include "ModuleAudio.h"

#include <fstream>

#include <iostream>
using namespace std;

// Basic Module Operations

// Constructors

ModulePlayerDhalsim::ModulePlayerDhalsim(int playerNum, bool start_enabled) :
	Module(start_enabled),
	playerState(PLAYER_IDLE),
	colliding_players(false),
	jumping(false),
	hitted(false),
	head_hitted(false),
	leg_hitted(false),
	life(200),
	wins(0),
	win(false),
	dead(false),
	time_0(false),
	starting_combo(COMBO_NOTHING),
	damageType(NONE_DAMAGE),
	distance_jumped(0),
	jump_attacked(false),
	already_hitted(false)
{
	numPlayer = playerNum;

	SetUpAnimations();
	SetUpPlayer(numPlayer);

	lastShotTimer = SDL_GetTicks();
}

// Destructor
ModulePlayerDhalsim::~ModulePlayerDhalsim()
{
}

void ModulePlayerDhalsim::SetUpAnimations()
{
	Json::Value root;

	std::filebuf fb;
	if (fb.open("Game/dhalsim_data.json", std::ios::in))
	{
		std::istream is(&fb);
		is >> root;
		fb.close();
	}

	// Movement animations
	SetPlayerAnimationDataFromJSON(idle, root["idle"][0]);
	SetPlayerAnimationDataFromJSON(forward, root["forward"][0]);
	SetPlayerAnimationDataFromJSON(backward, root["backward"][0]);
	SetPlayerAnimationDataFromJSON(crouching, root["crouching"][0]);
	SetPlayerAnimationDataFromJSON(block, root["block"][0]);
	SetPlayerAnimationDataFromJSON(crouch_block, root["crouch-block"][0]);
	SetPlayerAnimationDataFromJSON(jump, root["jump"][0]);

	// Far attacks animations
	SetPlayerAnimationDataFromJSON(L_punch, root["L_punch"][0]);
	SetPlayerAnimationDataFromJSON(M_punch, root["M_punch"][0]);
	SetPlayerAnimationDataFromJSON(H_punch, root["H_punch"][0]);
	SetPlayerAnimationDataFromJSON(L_kick, root["L_kick"][0]);
	SetPlayerAnimationDataFromJSON(M_kick, root["M_kick"][0]);
	SetPlayerAnimationDataFromJSON(H_kick, root["H_kick"][0]);

	// Near attacks
	SetPlayerAnimationDataFromJSON(F_L_punch, root["F_L_punch"][0]);
	SetPlayerAnimationDataFromJSON(F_M_punch, root["F_M_punch"][0]);
	SetPlayerAnimationDataFromJSON(F_H_punch, root["F_H_punch"][0]);
	SetPlayerAnimationDataFromJSON(F_L_kick, root["F_L_kick"][0]);
	SetPlayerAnimationDataFromJSON(F_M_kick, root["F_M_kick"][0]);
	SetPlayerAnimationDataFromJSON(F_H_kick, root["F_H_kick"][0]);

	// Especial position attacks
	SetPlayerAnimationDataFromJSON(crouch_punch, root["crouch_punch"][0]);
	SetPlayerAnimationDataFromJSON(crouch_kick, root["crouch_kick"][0]);
	SetPlayerAnimationDataFromJSON(jump_punch, root["jump_punch"][0]);
	SetPlayerAnimationDataFromJSON(jump_kick, root["jump_kick"][0]);

	// Hit animation
	SetPlayerAnimationDataFromJSON(hit, root["hit"][0]);
	SetPlayerAnimationDataFromJSON(face_hit, root["face_hit"][0]);
	SetPlayerAnimationDataFromJSON(crouch_hit, root["crouch_hit"][0]);
	SetPlayerAnimationDataFromJSON(air_hit, root["air_hit"][0]);

	// End animations
	SetPlayerAnimationDataFromJSON(ko, root["ko"][0]);
	SetPlayerAnimationDataFromJSON(victory1, root["victory1"][0]);
	SetPlayerAnimationDataFromJSON(victory2, root["victory2"][0]);
	SetPlayerAnimationDataFromJSON(time_out, root["time_out"][0]);

	// Special attacks
	SetPlayerAnimationDataFromJSON(yoga_fire, root["yoga_fire"][0]);
	SetPlayerAnimationDataFromJSON(yoga_flame, root["yoga_flame"][0]);
	SetPlayerAnimationDataFromJSON(yoga_mummy, root["yoga_mummy"][0]);
	SetPlayerAnimationDataFromJSON(yoga_spear, root["yoga_spear"][0]);

	// particle animations
	SetParticleAnimationDataFromJSON(particula, root["particle"][0]);
	SetParticleAnimationDataFromJSON(destroy_particula, root["destroy-particle"][0]);
}

void ModulePlayerDhalsim::SetUpPlayer(int numPlayer)
{
	if (numPlayer == 1)
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
	else
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
}

// Load assets
bool ModulePlayerDhalsim::Start()
{
	LOG("Loading Dhalsim");

	if (numPlayer == 1)
	{
		graphics = App->textures->Load("Game/dhalsim.png");
		otherPlayer = App->player_two;
	}
	else
	{
		graphics = App->textures->Load("Game/dhalsim_recolor.png");
		otherPlayer = App->player_one;
	}

	App->collisions->AddCollider(&player_collider);
	App->collisions->AddCollider(&collider_head);
	App->collisions->AddCollider(&collider_body);
	App->collisions->AddCollider(&collider_legs);
	App->collisions->AddCollider(&collider_attack);

	audio_id_yoga_fire = App->audio->LoadFx("Game/yoga_fire.wav");
	audio_id_yoga_flame = App->audio->LoadFx("Game/yoga_flame.wav");
	audio_id_dead = App->audio->LoadFx("Game/dhalsim_dead.wav");
	audio_id_L_attack = App->audio->LoadFx("Game/SF2_hit_1.wav");;
	audio_id_M_attack = App->audio->LoadFx("Game/SF2_hit_2.wav");;
	audio_id_H_attack = App->audio->LoadFx("Game/SF2_hit_3.wav");;
	audio_id_L_impact = App->audio->LoadFx("Game/SF2_impact_1.wav");;
	audio_id_M_impact = App->audio->LoadFx("Game/SF2_impact_2.wav");;
	audio_id_H_impact = App->audio->LoadFx("Game/SF2_impact_3.wav");;

	if (otherPlayer->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	return true;
}

// Unload assets
bool ModulePlayerDhalsim::CleanUp()
{
	LOG("Unloading Base player");

	App->textures->Unload(graphics);

	return true;
}

// PreUpdate
update_status ModulePlayerDhalsim::PreUpdate()
{
	bool near = false;

	if (otherPlayer->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	if ((otherPlayer->getPosition().x - position.x) < 50 &&
		(otherPlayer->getPosition().x - position.x) > -50)
		near = true;

	if (dead)
	{
		if (playerState != PLAYER_KO)
		{
			playerState = PLAYER_KO;
			otherPlayer->SetWin(true);

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
				++wins;
				playerState = PLAYER_WIN_2;
			}
		}
		return UPDATE_CONTINUE;
	}
	else if (playerState == PLAYER_BLOCKING_HITTED || playerState == PLAYER_CROUCH_BLOCKING_HITTED);
	else if (leg_hitted && playerState == PLAYER_BLOCKING)
	{
		playerState = PLAYER_HIT;
	}
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
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			looking_right)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			looking_right && otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			looking_right)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else if (GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_STATIC;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
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
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (!looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else
			playerState = PLAYER_IDLE;

		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
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
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_BACKWARD;
			else
				playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else
		{
			playerState = PLAYER_IDLE;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}

		break;

	case PLAYER_JUMPING:

		if (distance_jumped == 0 && !going_up)
		{
			already_hitted = false;
			playerState = PLAYER_IDLE;
			jump_attacked = false;
			jump.RestartFrames();
		}
		else if (starting_combo == AERIAL_COMBO_PUNCH && !jump_attacked)
		{
			already_hitted = false;
			jump_attacked = true;
			playerState = PLAYER_YOGA_MUMMY;
			if (looking_right)
				directionMummy = JUMP_RIGHT;
			else
				directionMummy = JUMP_LEFT;
		}
		else if (starting_combo == AERIAL_COMBO_KICK && !jump_attacked)
		{
			already_hitted = false;
			jump_attacked = true;
			playerState = PLAYER_YOGA_SPEAR;
			if (looking_right)
				directionMummy = JUMP_RIGHT;
			else
				directionMummy = JUMP_LEFT;
		}
		else
		{
			if (((GetPlayerInput(INPUT_L_PUNCH)) ||
				(GetPlayerInput(INPUT_M_PUNCH)) ||
				(GetPlayerInput(INPUT_H_PUNCH))) &&
				!jump_attacked)
			{
				jump_attacked = true;
				playerState = PLAYER_JUMP_PUNCH;
			}

			else if (((GetPlayerInput(INPUT_L_KICK)) ||
				(GetPlayerInput(INPUT_M_KICK)) ||
				(GetPlayerInput(INPUT_H_KICK))) &&
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
			already_hitted = false;
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
			already_hitted = false;
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

		if ((GetPlayerInput(INPUT_RIGHT))
			&& otherPlayer->IsAttacking() && !looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_LEFT))
			&& otherPlayer->IsAttacking() && looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_L_PUNCH)) ||
			(GetPlayerInput(INPUT_M_PUNCH)) ||
			(GetPlayerInput(INPUT_H_PUNCH)))
			playerState = PLAYER_CROUCH_PUNCH;

		else if ((GetPlayerInput(INPUT_L_KICK)) ||
			(GetPlayerInput(INPUT_M_KICK)) ||
			(GetPlayerInput(INPUT_H_KICK)))
			playerState = PLAYER_CROUCH_KICK;

		else if (!(GetPlayerInput(INPUT_DOWN)))
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
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_CROUCH_HIT:

		if (crouch_hit.IsEnded())
		{
			crouch_hit.RestartFrames();
			playerState = PLAYER_CROUCHING;
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_FACE_HIT:

		if (face_hit.IsEnded())
		{
			face_hit.RestartFrames();
			playerState = PLAYER_IDLE;
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_YOGA_FIRE:
		if (yoga_fire.GetCurrentFrameNumber() == 3 && SDL_GetTicks() - lastShotTimer > 500)
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

		going_up = false;
		if (distance_jumped == 0)
		{
			already_hitted = false;
			yoga_mummy.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		else if (yoga_mummy.IsEnded() && distance_jumped > 0)
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

		going_up = false;
		if (distance_jumped == 0)
		{
			already_hitted = false;
			yoga_spear.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		else if (yoga_spear.IsEnded() && distance_jumped > 0)
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
		if (!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT))))
		{
			block.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		else if (otherPlayer->IsAttacking() &&
			(GetPlayerInput(INPUT_DOWN)))
		{
			block.RestartFrames();
			playerState = PLAYER_CROUCH_BLOCKING;
		}
		else if (block.IsEnded())
			block.RestartFrames();
		break;

	case PLAYER_CROUCH_BLOCKING:
		if ((!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT))))
			&& (GetPlayerInput(INPUT_DOWN)))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		else if (!(GetPlayerInput(INPUT_DOWN)) && (!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		else if (otherPlayer->IsAttacking() && !(GetPlayerInput(INPUT_DOWN)) &&
			((looking_right && (GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && (GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_BLOCKING;
		}
		else if (crouch_block.IsEnded())
			crouch_block.RestartFrames();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
			block.RestartFrames();
			playerState = PLAYER_BLOCKING;
		}
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (crouch_block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCH_BLOCKING;
		}
		break;

	case PLAYER_AIR_HITTED:
		if (air_hit.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			air_hit.RestartFrames();
			going_up = false;
			playerState = PLAYER_JUMPING;
		}
		break;
	}

	starting_combo = COMBO_NOTHING;

	return UPDATE_CONTINUE;
}

// Update
update_status ModulePlayerDhalsim::Update()
{
	SDL_Rect aux;
	int pivot;
	Collider_player_structure cps;

	if (otherPlayer->playerInCameraLimit() &&
		((App->renderer->ScreenLeftLimit() && otherPlayer->GetLooking_right()) ||
		(App->renderer->ScreenRightLimit() && !otherPlayer->GetLooking_right())) &&
		((otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED) ||
		(otherPlayer->GetPlayerState() == PLAYER_CROUCH_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_FACE_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_BLOCKING_HITTED) ||
		(otherPlayer->GetPlayerState() == PLAYER_CROUCH_BLOCKING_HITTED)) &&
		(playerState != PLAYER_JUMP_KICK) && (playerState != PLAYER_JUMP_PUNCH) && 
		(playerState != PLAYER_JUMPING) && (playerState != PLAYER_AIR_HITTED))
	{
		if (looking_right && (otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED))
			MovePlayer(-3);
		else if (looking_right)
			MovePlayer(-1);
		else if (otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED)
			MovePlayer(3);
		else
			MovePlayer(1);
	}

	switch (playerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		cps = idle.GetCurrentCollider();
		idle.NextFrame();
		break;

	case PLAYER_WALKING_FORWARD:
		if (!colliding_players)
		{
			if (looking_right)
				MovePlayer(2);
			else
				MovePlayer(-2);
		}
		aux = forward.GetCurrentFrame();
		pivot = forward.GetCurrentPivot();
		cps = forward.GetCurrentCollider();
		forward.NextFrame();
		break;

	case PLAYER_WALKING_BACKWARD:
		if (looking_right)
			MovePlayer(-1);
		else
			MovePlayer(1);
		aux = backward.GetCurrentFrame();
		pivot = backward.GetCurrentPivot();
		cps = backward.GetCurrentCollider();
		backward.NextFrame();
		break;

	case PLAYER_CROUCHING:
		aux = crouching.GetCurrentFrame();
		pivot = crouching.GetCurrentPivot();
		cps = crouching.GetCurrentCollider();
		crouching.NextFrame();
		break;

	case PLAYER_JUMPING:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		if (distance_jumped > 90)
		{
			aux = jump.frames[2].frame;
			pivot = jump.frames[2].pivot;
			cps = jump.frames[2].colliders;
		}
		else
		{
			aux = jump.frames[1].frame;
			pivot = jump.frames[1].pivot;
			cps = jump.frames[1].colliders;
		}

		break;


	case PLAYER_LOW_PUNCH:
		aux = L_punch.GetCurrentFrame();
		pivot = L_punch.GetCurrentPivot();
		cps = L_punch.GetCurrentCollider();
		L_punch.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 12;
		break;

	case PLAYER_LOW_KICK:
		aux = L_kick.GetCurrentFrame();
		pivot = L_kick.GetCurrentPivot();
		cps = L_kick.GetCurrentCollider();
		L_kick.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 12;
		break;

	case PLAYER_MEDIUM_PUNCH:
		aux = M_punch.GetCurrentFrame();
		pivot = M_punch.GetCurrentPivot();
		cps = M_punch.GetCurrentCollider();
		M_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_MEDIUM_KICK:
		aux = M_kick.GetCurrentFrame();
		pivot = M_kick.GetCurrentPivot();
		cps = M_kick.GetCurrentCollider();
		M_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_HIGH_PUNCH:
		aux = H_punch.GetCurrentFrame();
		pivot = H_punch.GetCurrentPivot();
		cps = H_punch.GetCurrentCollider();
		H_punch.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 20;
		break;

	case PLAYER_HIGH_KICK:
		aux = H_kick.GetCurrentFrame();
		pivot = H_kick.GetCurrentPivot();
		cps = H_kick.GetCurrentCollider();
		H_kick.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 20;
		break;

	case PLAYER_FORWARD_LOW_PUNCH:
		aux = F_L_punch.GetCurrentFrame();
		pivot = F_L_punch.GetCurrentPivot();
		cps = F_L_punch.GetCurrentCollider();
		F_L_punch.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 14;
		break;

	case PLAYER_FORWARD_LOW_KICK:
		aux = F_L_kick.GetCurrentFrame();
		pivot = F_L_kick.GetCurrentPivot();
		cps = F_L_kick.GetCurrentCollider();
		F_L_kick.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 14;
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:
		aux = F_M_punch.GetCurrentFrame();
		pivot = F_M_punch.GetCurrentPivot();
		cps = F_M_punch.GetCurrentCollider();
		F_M_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:
		aux = F_M_kick.GetCurrentFrame();
		pivot = F_M_kick.GetCurrentPivot();
		cps = F_M_kick.GetCurrentCollider();
		F_M_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:
		aux = F_H_punch.GetCurrentFrame();
		pivot = F_H_punch.GetCurrentPivot();
		cps = F_H_punch.GetCurrentCollider();
		F_H_punch.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 22;
		break;

	case PLAYER_FORWARD_HIGH_KICK:
		aux = F_H_kick.GetCurrentFrame();
		pivot = F_H_kick.GetCurrentPivot();
		cps = F_H_kick.GetCurrentCollider();
		F_H_kick.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 22;
		break;

	case PLAYER_CROUCH_PUNCH:
		aux = crouch_punch.GetCurrentFrame();
		pivot = crouch_punch.GetCurrentPivot();
		cps = crouch_punch.GetCurrentCollider();
		crouch_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_CROUCH_KICK:
		if (looking_right)
		{
			MovePlayer(2);
		}
		else
		{
			MovePlayer(-2);
		}
		aux = crouch_kick.GetCurrentFrame();
		pivot = crouch_kick.GetCurrentPivot();
		cps = crouch_kick.GetCurrentCollider();
		crouch_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_JUMP_PUNCH:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_punch.GetCurrentFrame();
		pivot = jump_punch.GetCurrentPivot();
		cps = jump_punch.GetCurrentCollider();
		jump_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;


	case PLAYER_JUMP_KICK:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_kick.GetCurrentFrame();
		pivot = jump_kick.GetCurrentPivot();
		cps = jump_kick.GetCurrentCollider();
		jump_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = hit.GetCurrentFrame();
		pivot = hit.GetCurrentPivot();
		cps = hit.GetCurrentCollider();
		hit.NextFrame();
		break;

	case PLAYER_CROUCH_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = crouch_hit.GetCurrentFrame();
		pivot = crouch_hit.GetCurrentPivot();
		cps = crouch_hit.GetCurrentCollider();
		crouch_hit.NextFrame();
		break;

	case PLAYER_FACE_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = face_hit.GetCurrentFrame();
		pivot = face_hit.GetCurrentPivot();
		cps = face_hit.GetCurrentCollider();
		face_hit.NextFrame();
		break;

	case PLAYER_KO:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		if (!ko.IsEnded()) {
			if (looking_right)
				MovePlayer(-1);
			else
				MovePlayer(1);
		}
		aux = ko.GetCurrentFrame();
		pivot = ko.GetCurrentPivot();
		cps = ko.GetCurrentCollider();
		ko.NextFrame();

		break;

	case PLAYER_WIN_1:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = victory1.GetCurrentFrame();
		pivot = victory1.GetCurrentPivot();
		cps = victory1.GetCurrentCollider();
		victory1.NextFrame();
		break;

	case PLAYER_WIN_2:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = victory2.GetCurrentFrame();
		pivot = victory2.GetCurrentPivot();
		cps = victory2.GetCurrentCollider();
		victory2.NextFrame();
		break;

	case PLAYER_TIME_OUT:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = time_out.GetCurrentFrame();
		pivot = time_out.GetCurrentPivot();
		cps = time_out.GetCurrentCollider();
		time_out.NextFrame();
		break;

	case PLAYER_YOGA_FIRE:
		aux = yoga_fire.GetCurrentFrame();
		pivot = yoga_fire.GetCurrentPivot();
		cps = yoga_fire.GetCurrentCollider();
		yoga_fire.NextFrame();
		break;

	case PLAYER_YOGA_FLAME:
		aux = yoga_flame.GetCurrentFrame();
		pivot = yoga_flame.GetCurrentPivot();
		cps = yoga_flame.GetCurrentCollider();
		yoga_flame.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 28;
		break;

	case PLAYER_YOGA_MUMMY:

		distance_jumped -= 3;
		if (directionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (directionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_mummy.GetCurrentFrame();
		pivot = yoga_mummy.GetCurrentPivot();
		cps = yoga_mummy.GetCurrentCollider();
		yoga_mummy.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_YOGA_SPEAR:

		distance_jumped -= 3;
		if (directionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (directionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_spear.GetCurrentFrame();
		pivot = yoga_spear.GetCurrentPivot();
		cps = yoga_spear.GetCurrentCollider();
		yoga_spear.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_BLOCKING:
		aux = block.GetCurrentFrame();
		pivot = block.GetCurrentPivot();
		cps = block.GetCurrentCollider();
		block.NextFrame();
		break;

	case PLAYER_CROUCH_BLOCKING:
		aux = crouch_block.GetCurrentFrame();
		pivot = crouch_block.GetCurrentPivot();
		cps = crouch_block.GetCurrentCollider();
		crouch_block.NextFrame();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (!looking_right)
		{
			MovePlayer(2);
		}
		else
		{
			MovePlayer(-2);
		}
		aux = block.GetCurrentFrame();
		pivot = block.GetCurrentPivot();
		cps = block.GetCurrentCollider();
		block.NextFrame();
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = crouch_block.GetCurrentFrame();
		pivot = crouch_block.GetCurrentPivot();
		cps = crouch_block.GetCurrentCollider();
		crouch_block.NextFrame();
		break;

	case PLAYER_AIR_HITTED:
		if (!looking_right)
		{
			MovePlayer(2);
			directionJump = JUMP_RIGHT;
		}
		else
		{
			MovePlayer(-2);
			directionJump = JUMP_LEFT;
		}
		distance_jumped += 3;
		going_up = true;
		aux = air_hit.GetCurrentFrame();
		pivot = air_hit.GetCurrentPivot();
		cps = air_hit.GetCurrentCollider();
		air_hit.NextFrame();
		break;

	}

	player_collider.rect = cps.Position_collider;
	collider_head.rect = cps.Collider_head;
	collider_body.rect = cps.Collider_body;
	collider_legs.rect = cps.Collider_legs;
	collider_attack.rect = cps.Collider_attack;

	if (looking_right)
	{
		player_collider.rect.x += position.x;
		player_collider.rect.y += position.y - distance_jumped;
		collider_head.rect.x += position.x;
		collider_head.rect.y += position.y - distance_jumped;
		collider_body.rect.x += position.x;
		collider_body.rect.y += position.y - distance_jumped;
		collider_legs.rect.x += position.x;
		collider_legs.rect.y += position.y - distance_jumped;
		collider_attack.rect.x += position.x;
		collider_attack.rect.y += position.y - distance_jumped;
		App->renderer->Blit(graphics, position.x - pivot, position.y - aux.h - distance_jumped, &(aux), 1.0f, SDL_FLIP_NONE);
	}
	else
	{
		player_collider.rect.x = position.x - player_collider.rect.x - player_collider.rect.w;
		player_collider.rect.y += position.y - distance_jumped;
		collider_head.rect.x = position.x - collider_head.rect.x - collider_head.rect.w;
		collider_head.rect.y += position.y - distance_jumped;
		collider_body.rect.x = position.x - collider_body.rect.x - collider_body.rect.w;
		collider_body.rect.y += position.y - distance_jumped;
		collider_legs.rect.x = position.x - collider_legs.rect.x - collider_legs.rect.w;
		collider_legs.rect.y += position.y - distance_jumped;
		collider_attack.rect.x = position.x - collider_attack.rect.x - collider_attack.rect.w;
		collider_attack.rect.y += position.y - distance_jumped;
		App->renderer->Blit(graphics, position.x - aux.w + pivot, position.y - aux.h - distance_jumped, &(aux), 1.0f, SDL_FLIP_HORIZONTAL);
	}
	
	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModulePlayerDhalsim::PostUpdate()
{
	colliding_players = false;
	return UPDATE_CONTINUE;
}

// On Collision
void ModulePlayerDhalsim::OnCollision(Collider* c1, Collider* c2)
{
	if (!win && !dead && !time_0){
		if (c1->type == COLLIDER_BODY_PLAYER_ONE && c2->type == COLLIDER_BODY_PLAYER_TWO)
			colliding_players = true;
		else if (c1->type == COLLIDER_BODY_PLAYER_TWO && c2->type == COLLIDER_BODY_PLAYER_ONE)
			colliding_players = true;
		else if (c1->type == COLLIDER_PLAYER_ONE && c2->type == COLLIDER_ATTACK_PLAYER_TWO && c2->rect.w * c2->rect.h != 0 && !already_hitted)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
			}
			else if ((otherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || otherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
				leg_hitted = true;
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}
			else if (!hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(audio_id_L_impact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(audio_id_M_impact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(audio_id_H_impact);
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if (c1->type == COLLIDER_PLAYER_TWO && c2->type == COLLIDER_ATTACK_PLAYER_ONE  && c2->rect.w * c2->rect.h != 0 && !already_hitted)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
			}
			else if ((otherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || otherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
				leg_hitted = true;
			}


			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}
			else if (!hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(audio_id_L_impact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(audio_id_M_impact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(audio_id_H_impact);
			}


			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_ONE || c1->type == COLLIDER_ATTACK_PLAYER_ONE) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= 18;
				App->audio->PlayFx(audio_id_L_impact);
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_TWO || c1->type == COLLIDER_ATTACK_PLAYER_TWO) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= 18;
				App->audio->PlayFx(audio_id_L_impact);
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
	}
}

// Returns true if the player is the limit of the camera
bool ModulePlayerDhalsim::playerInCameraLimit() const
{
	if (App->renderer->camera.x <= -((player_collider.rect.x - player_collider.rect.w)*SCREEN_SIZE))
		return true;
	if (App->renderer->camera.x - App->renderer->camera.w >= -((player_collider.rect.x + player_collider.rect.w * 2)*SCREEN_SIZE))
		return true;
	return false;
}

// Moves horizontaly the player, it can move the camera
void ModulePlayerDhalsim::MovePlayer(int distance)
{
	if (distance > 0)
	{
		if (!App->renderer->ScreenRightLimit())
		{
			if (!otherPlayer->playerInCameraLimit())
			{
				position.x += distance;
				if (App->renderer->camera.x - App->renderer->camera.w >= -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
					App->renderer->camera.x -= SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x - App->renderer->camera.w < -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
					position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x - App->renderer->camera.w < -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
				position.x += distance;
		}
	}
	else
	{
		if (!App->renderer->ScreenLeftLimit())
		{
			if (!otherPlayer->playerInCameraLimit())
			{
				position.x += distance;
				if (App->renderer->camera.x <= -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
					App->renderer->camera.x += SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x > -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
					position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x > -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
				position.x += distance;
		}
	}
}

// Resets the player states
void ModulePlayerDhalsim::restartPlayer(bool everything)
{

	if (numPlayer == 1)
	{
		position.x = 150;
		position.y = 200;
	}
	else
	{
		position.x = 300;
		position.y = 200;
	}

	if (everything)
		wins = 0;

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

// Get and Set variables

iPoint ModulePlayerDhalsim::getPosition() const
{
	return position;
}

bool ModulePlayerDhalsim::IsAttacking() const
{
	if (playerState == PLAYER_LOW_PUNCH ||
		playerState == PLAYER_LOW_KICK ||
		playerState == PLAYER_MEDIUM_PUNCH ||
		playerState == PLAYER_MEDIUM_KICK ||
		playerState == PLAYER_HIGH_PUNCH ||
		playerState == PLAYER_HIGH_KICK ||
		playerState == PLAYER_CROUCH_PUNCH ||
		playerState == PLAYER_CROUCH_KICK ||
		playerState == PLAYER_JUMP_PUNCH ||
		playerState == PLAYER_JUMP_KICK ||
		playerState == PLAYER_FORWARD_LOW_PUNCH ||
		playerState == PLAYER_FORWARD_LOW_KICK ||
		playerState == PLAYER_FORWARD_MEDIUM_PUNCH ||
		playerState == PLAYER_FORWARD_MEDIUM_KICK ||
		playerState == PLAYER_FORWARD_HIGH_PUNCH ||
		playerState == PLAYER_FORWARD_HIGH_KICK ||
		playerState == PLAYER_YOGA_FIRE ||
		playerState == PLAYER_YOGA_FLAME ||
		playerState == PLAYER_YOGA_MUMMY ||
		playerState == PLAYER_YOGA_SPEAR)
	{
		return true;
	}
	else if (App->particles->GetNumberParticles() > 0)
	{
		if (otherPlayer->looking_right)
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x > otherPlayer->position.x && App->particles->GetParticleSpeed(i) < 0)
					return true;
			}
		}
		else
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x < otherPlayer->position.x && App->particles->GetParticleSpeed(i) > 0)
					return true;
			}
		}
	}
	return false;
}

int ModulePlayerDhalsim::GetWins() const
{
	return wins;
}

void ModulePlayerDhalsim::SetWins(int wins)
{
	this->wins = wins;
}

int ModulePlayerDhalsim::GetLife() const
{
	return life;
}

void ModulePlayerDhalsim::SetLife(int life)
{
	this->life = life;
}

void ModulePlayerDhalsim::DecreseLife(int life)
{
	this->life -= life;
}

bool ModulePlayerDhalsim::GetWin() const
{
	return win;
}

void ModulePlayerDhalsim::SetWin(bool win)
{
	this->win = win;
}

bool ModulePlayerDhalsim::GetDead() const
{
	return dead;
}

void ModulePlayerDhalsim::SetDead(bool dead)
{
	this->dead = dead;
}

bool ModulePlayerDhalsim::GetTime_0() const
{
	return time_0;
}

void ModulePlayerDhalsim::SetTime_0(bool time_0)
{
	this->time_0 = time_0;
}

bool ModulePlayerDhalsim::GetJumping() const
{
	return jumping;
}

void ModulePlayerDhalsim::SetJumping(bool jumping)
{
	this->jumping = jumping;
}

bool ModulePlayerDhalsim::GetLooking_right() const
{
	return looking_right;
}

void ModulePlayerDhalsim::SetLooking_right(bool looking_right)
{
	this->looking_right = looking_right;
}

player_state ModulePlayerDhalsim::GetPlayerState() const
{
	return playerState;
}

void ModulePlayerDhalsim::SetPlayerState(player_state playerState)
{
	this->playerState = playerState;
}

combo_types ModulePlayerDhalsim::GetStartingCombo() const
{
	return starting_combo;
}

void ModulePlayerDhalsim::SetStartingCombo(combo_types starting_combo)
{
	this->starting_combo = starting_combo;
}

int ModulePlayerDhalsim::GetDistanceJumped() const
{
	return distance_jumped;
}

bool ModulePlayerDhalsim::GetPlayerInput(input_type actionKey)
{
	if (numPlayer == 1)
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
	}
	else
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
	}
	return false;
}

void ModulePlayerDhalsim::SetSDLRectFromData(SDL_Rect& sdl_rect, const Json::Value& jsonValue)
{
	sdl_rect.x = jsonValue[0].asInt();
	sdl_rect.y = jsonValue[1].asInt();
	sdl_rect.w = jsonValue[2].asInt();
	sdl_rect.h = jsonValue[3].asInt();
}

void ModulePlayerDhalsim::SetPlayerAnimationDataFromJSON(Animation& animation, Json::Value& jsonValue)
{
	int numberOfFrames = jsonValue.get("numberOfFrames", "0").asInt();

	for (int i = 0; i < numberOfFrames; ++i)
	{
		Json::Value currentValue = jsonValue["frames"][i];
		AnimationStructure animationStructure;

		SetSDLRectFromData(animationStructure.frame, currentValue["frame"]);

		animationStructure.pivot = currentValue["pivot"].asInt();

		SetSDLRectFromData(animationStructure.colliders.Position_collider, currentValue["position-collider"]);
		SetSDLRectFromData(animationStructure.colliders.Collider_head, currentValue["head-collider"]);
		SetSDLRectFromData(animationStructure.colliders.Collider_body, currentValue["body-collider"]);
		SetSDLRectFromData(animationStructure.colliders.Collider_legs, currentValue["legs-collider"]);
		SetSDLRectFromData(animationStructure.colliders.Collider_attack, currentValue["attack-collider"]);

		animationStructure.duration = currentValue["duration"].asInt();

		animation.frames.push_back(animationStructure);
		animation.loop = jsonValue.get("loop", true).asBool();
	}
}

void ModulePlayerDhalsim::SetParticleAnimationDataFromJSON(ParticleAnimation& particleAnimation, Json::Value& jsonValue)
{
	int numberOfFrames = jsonValue.get("numberOfFrames", "0").asInt();

	for (int i = 0; i < numberOfFrames; ++i)
	{
		Json::Value currentValue = jsonValue["frames"][i];
		ParticleAnimationStructure particleAnimationStructure;

		SetSDLRectFromData(particleAnimationStructure.frame, currentValue["frame"]);

		particleAnimationStructure.pivot = currentValue["pivot"].asInt();

		SetSDLRectFromData(particleAnimationStructure.collider, currentValue["collider"]);

		particleAnimationStructure.duration = currentValue["duration"].asInt();

		particleAnimation.frames.push_back(particleAnimationStructure);
		particleAnimation.loop = jsonValue.get("loop", true).asBool();
	}
}