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
	m_PlayerState(PLAYER_IDLE),
	m_AreCollidingPlayers(false),
	m_Jumping(false),
	m_Hitted(false),
	m_HeadHitted(false),
	m_LegHitted(false),
	m_Life(200),
	m_Wins(0),
	m_Win(false),
	m_Dead(false),
	m_Time0(false),
	m_StartingCombo(COMBO_NOTHING),
	m_DamageType(NONE_DAMAGE),
	m_DistanceJumped(0),
	m_JumpAttacked(false),
	m_AlreadyHitted(false)
{
	m_NumPlayer = playerNum;

	SetUpAnimations();
	SetUpPlayer(m_NumPlayer);

	m_LastShotTimer = SDL_GetTicks();
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
	SetParticleAnimationDataFromJSON(m_Particle, root["particle"][0]);
	SetParticleAnimationDataFromJSON(m_DestroyParticle, root["destroy-particle"][0]);
}

void ModulePlayerDhalsim::SetUpPlayer(int numPlayer)
{
	if (numPlayer == 1)
	{
		m_Position.x = 150;
		m_Position.y = 200;

		m_PlayerCollider.type = COLLIDER_BODY_PLAYER_ONE;
		m_ColliderHead.type = m_ColliderBody.type = m_ColliderLegs.type = COLLIDER_PLAYER_ONE;
		m_ColliderAttack.type = COLLIDER_ATTACK_PLAYER_ONE;
	}
	else
	{
		m_Position.x = 300;
		m_Position.y = 200;

		m_PlayerCollider.type = COLLIDER_BODY_PLAYER_TWO;
		m_ColliderHead.type = m_ColliderBody.type = m_ColliderLegs.type = COLLIDER_PLAYER_TWO;
		m_ColliderAttack.type = COLLIDER_ATTACK_PLAYER_TWO;
	}
	m_PlayerCollider.module = m_ColliderHead.module = m_ColliderBody.module = m_ColliderLegs.module = m_ColliderAttack.module = this;
}

// Load assets
bool ModulePlayerDhalsim::Start()
{
	LOG("Loading Dhalsim");

	if (m_NumPlayer == 1)
	{
		m_Graphics = App->textures->Load("Game/dhalsim.png");
		m_OtherPlayer = App->player_two;
	}
	else
	{
		m_Graphics = App->textures->Load("Game/dhalsim_recolor.png");
		m_OtherPlayer = App->player_one;
	}

	App->collisions->AddCollider(&m_PlayerCollider);
	App->collisions->AddCollider(&m_ColliderHead);
	App->collisions->AddCollider(&m_ColliderBody);
	App->collisions->AddCollider(&m_ColliderLegs);
	App->collisions->AddCollider(&m_ColliderAttack);


	m_AudioIdYogaFire = App->audio->LoadFx("Game/yoga_fire.wav");
	m_AudioIdYogaFlame = App->audio->LoadFx("Game/yoga_flame.wav");
	m_AudioIdDead = App->audio->LoadFx("Game/dhalsim_dead.wav");
	m_AudioIdLAttack = App->audio->LoadFx("Game/SF2_hit_1.wav");;
	m_AudioIdMAttack = App->audio->LoadFx("Game/SF2_hit_2.wav");;
	m_AudioIdHAttack = App->audio->LoadFx("Game/SF2_hit_3.wav");;
	m_AudioIdLImpact = App->audio->LoadFx("Game/SF2_impact_1.wav");;
	m_AudioIdMImpact = App->audio->LoadFx("Game/SF2_impact_2.wav");;
	m_AudioIdHImpact = App->audio->LoadFx("Game/SF2_impact_3.wav");;

	if (m_OtherPlayer->getPosition().x > m_Position.x)
		m_LookingRight = true;
	else
		m_LookingRight = false;

	return true;
}

// Unload assets
bool ModulePlayerDhalsim::CleanUp()
{
	LOG("Unloading Base player");

	App->textures->Unload(m_Graphics);

	return true;
}

// PreUpdate
UpdateStatus ModulePlayerDhalsim::PreUpdate()
{
	bool near = false;

	if (m_OtherPlayer->getPosition().x > m_Position.x)
		m_LookingRight = true;
	else
		m_LookingRight = false;

	if ((m_OtherPlayer->getPosition().x - m_Position.x) < 50 &&
		(m_OtherPlayer->getPosition().x - m_Position.x) > -50)
		near = true;

	if (m_Dead)
	{
		if (m_PlayerState != PLAYER_KO)
		{
			m_PlayerState = PLAYER_KO;
			m_OtherPlayer->SetWin(true);

		}
		return UPDATE_CONTINUE;
	}
	else if (m_Time0)
	{
		m_PlayerState = PLAYER_TIME_OUT;
	}
	else if (m_Win &&
		(m_PlayerState == PLAYER_IDLE ||
		m_PlayerState == PLAYER_CROUCHING ||
		m_PlayerState == PLAYER_WALKING_FORWARD ||
		m_PlayerState == PLAYER_WALKING_BACKWARD))
	{
		if (m_PlayerState != PLAYER_WIN_1 && m_PlayerState != PLAYER_WIN_2)
		{
			if (m_Wins == 0)
			{
				++m_Wins;
				m_PlayerState = PLAYER_WIN_1;
			}
			else
			{
				++m_Wins;
				m_PlayerState = PLAYER_WIN_2;
			}
		}
		return UPDATE_CONTINUE;
	}
	else if (m_PlayerState == PLAYER_BLOCKING_HITTED || m_PlayerState == PLAYER_CROUCH_BLOCKING_HITTED);
	else if (m_LegHitted && m_PlayerState == PLAYER_BLOCKING)
	{
		m_PlayerState = PLAYER_HIT;
	}
	else if (m_Hitted && m_PlayerState == PLAYER_BLOCKING)
	{
		m_PlayerState = PLAYER_BLOCKING_HITTED;
		block.RestartFrames();
	}
	else if (m_Hitted && m_PlayerState == PLAYER_CROUCH_BLOCKING)
	{
		m_PlayerState = PLAYER_CROUCH_BLOCKING_HITTED;
		crouch_block.RestartFrames();
	}
	else if ((m_Hitted || m_HeadHitted) && (m_PlayerState == PLAYER_JUMPING || m_PlayerState == PLAYER_JUMP_PUNCH || m_PlayerState == PLAYER_JUMP_KICK || m_PlayerState == PLAYER_AIR_HITTED))
	{
		m_PlayerState = PLAYER_AIR_HITTED;
	}
	else if ((m_Hitted || m_HeadHitted) && (m_PlayerState == PLAYER_CROUCHING || m_PlayerState == PLAYER_CROUCH_PUNCH || m_PlayerState == PLAYER_CROUCH_KICK))
	{
		m_PlayerState = PLAYER_CROUCH_HIT;
	}
	else if (m_HeadHitted)
	{
		m_PlayerState = PLAYER_FACE_HIT;
	}
	else if (m_Hitted)
	{
		m_PlayerState = PLAYER_HIT;
	}

	switch (m_PlayerState)
	{

	case PLAYER_IDLE:

		if (m_StartingCombo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(m_AudioIdYogaFire);
			m_PlayerState = PLAYER_YOGA_FIRE;
		}
		else if (m_StartingCombo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(m_AudioIdYogaFlame);
			m_PlayerState = PLAYER_YOGA_FLAME;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			m_LookingRight)
		{
			m_PlayerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			m_OtherPlayer->IsAttacking())
		{
			m_PlayerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			m_PlayerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			m_LookingRight && m_OtherPlayer->IsAttacking())
		{
			m_PlayerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			m_LookingRight)
		{
			m_PlayerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			m_PlayerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			m_PlayerState = PLAYER_CROUCHING;
		}
		else if (GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_STATIC;
		}
		if (near && m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}
		else if (m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}
		break;

	case PLAYER_WALKING_FORWARD:

		if (m_StartingCombo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(m_AudioIdYogaFire);
			m_PlayerState = PLAYER_YOGA_FIRE;
		}
		else if (m_StartingCombo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(m_AudioIdYogaFlame);
			m_PlayerState = PLAYER_YOGA_FLAME;
		}
		else if (m_StartingCombo == AERIAL_COMBO_PUNCH)
		{
			m_PlayerState = PLAYER_YOGA_MUMMY;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (m_LookingRight)
				m_PlayerState = PLAYER_WALKING_FORWARD;
			else if (m_OtherPlayer->IsAttacking())
				m_PlayerState = PLAYER_BLOCKING;
			else
				m_PlayerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (!m_LookingRight)
				m_PlayerState = PLAYER_WALKING_FORWARD;
			else if (m_OtherPlayer->IsAttacking())
				m_PlayerState = PLAYER_BLOCKING;
			else
				m_PlayerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			m_PlayerState = PLAYER_CROUCHING;
		}
		else
			m_PlayerState = PLAYER_IDLE;

		if (near && m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}
		else if (m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}
		break;

	case PLAYER_WALKING_BACKWARD:

		if (m_StartingCombo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(m_AudioIdYogaFire);
			m_PlayerState = PLAYER_YOGA_FIRE;
		}
		else if (m_StartingCombo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(m_AudioIdYogaFlame);
			m_PlayerState = PLAYER_YOGA_FLAME;
		}
		else if (m_OtherPlayer->IsAttacking())
		{
			m_PlayerState = PLAYER_BLOCKING;
			break;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			m_DistanceJumped = 0;
			m_GoingUp = true;
			m_PlayerState = PLAYER_JUMPING;
			m_DirectionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (m_LookingRight)
				m_PlayerState = PLAYER_WALKING_FORWARD;
			else
				m_PlayerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (m_LookingRight)
				m_PlayerState = PLAYER_WALKING_BACKWARD;
			else
				m_PlayerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			m_PlayerState = PLAYER_CROUCHING;
		}
		else
		{
			m_PlayerState = PLAYER_IDLE;
		}
		if (near && m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}
		else if (m_StartingCombo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				m_PlayerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				m_PlayerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(m_AudioIdLAttack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				m_PlayerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				m_PlayerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(m_AudioIdMAttack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				m_PlayerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				m_PlayerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(m_AudioIdHAttack);
			}
		}

		break;

	case PLAYER_JUMPING:

		if (m_DistanceJumped == 0 && !m_GoingUp)
		{
			m_AlreadyHitted = false;
			m_PlayerState = PLAYER_IDLE;
			m_JumpAttacked = false;
			jump.RestartFrames();
		}
		else if (m_StartingCombo == AERIAL_COMBO_PUNCH && !m_JumpAttacked)
		{
			m_AlreadyHitted = false;
			m_JumpAttacked = true;
			m_PlayerState = PLAYER_YOGA_MUMMY;
			if (m_LookingRight)
				m_DirectionMummy = JUMP_RIGHT;
			else
				m_DirectionMummy = JUMP_LEFT;
		}
		else if (m_StartingCombo == AERIAL_COMBO_KICK && !m_JumpAttacked)
		{
			m_AlreadyHitted = false;
			m_JumpAttacked = true;
			m_PlayerState = PLAYER_YOGA_SPEAR;
			if (m_LookingRight)
				m_DirectionMummy = JUMP_RIGHT;
			else
				m_DirectionMummy = JUMP_LEFT;
		}
		else
		{
			if (((GetPlayerInput(INPUT_L_PUNCH)) ||
				(GetPlayerInput(INPUT_M_PUNCH)) ||
				(GetPlayerInput(INPUT_H_PUNCH))) &&
				!m_JumpAttacked)
			{
				m_JumpAttacked = true;
				m_PlayerState = PLAYER_JUMP_PUNCH;
			}

			else if (((GetPlayerInput(INPUT_L_KICK)) ||
				(GetPlayerInput(INPUT_M_KICK)) ||
				(GetPlayerInput(INPUT_H_KICK))) &&
				!m_JumpAttacked)
			{
				m_JumpAttacked = true;
				m_PlayerState = PLAYER_JUMP_KICK;
			}
		}
		break;


	case PLAYER_JUMP_PUNCH:

		if (m_DistanceJumped == 0)
		{
			m_AlreadyHitted = false;
			m_JumpAttacked = false;
			m_PlayerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_punch.RestartFrames();
		}
		else if (jump_punch.IsEnded())
		{
			jump_punch.RestartFrames();
			m_PlayerState = PLAYER_JUMPING;
		}
		break;

	case PLAYER_JUMP_KICK:
		if (m_DistanceJumped == 0)
		{
			m_AlreadyHitted = false;
			m_JumpAttacked = false;
			m_PlayerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_kick.RestartFrames();
		}
		else if (jump_kick.IsEnded())
		{
			jump_kick.RestartFrames();
			m_PlayerState = PLAYER_JUMPING;
		}
		break;


	case PLAYER_CROUCHING:

		if ((GetPlayerInput(INPUT_RIGHT))
			&& m_OtherPlayer->IsAttacking() && !m_LookingRight)
			m_PlayerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_LEFT))
			&& m_OtherPlayer->IsAttacking() && m_LookingRight)
			m_PlayerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_L_PUNCH)) ||
			(GetPlayerInput(INPUT_M_PUNCH)) ||
			(GetPlayerInput(INPUT_H_PUNCH)))
			m_PlayerState = PLAYER_CROUCH_PUNCH;

		else if ((GetPlayerInput(INPUT_L_KICK)) ||
			(GetPlayerInput(INPUT_M_KICK)) ||
			(GetPlayerInput(INPUT_H_KICK)))
			m_PlayerState = PLAYER_CROUCH_KICK;

		else if (!(GetPlayerInput(INPUT_DOWN)))
		{
			m_PlayerState = PLAYER_IDLE;
			crouching.RestartFrames();
		}
		break;

	case PLAYER_LOW_PUNCH:

		if (L_punch.IsEnded())
		{
			L_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_LOW_KICK:

		if (L_kick.IsEnded())
		{
			L_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_PUNCH:

		if (M_punch.IsEnded())
		{
			M_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_KICK:

		if (M_kick.IsEnded())
		{
			M_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_PUNCH:

		if (H_punch.IsEnded())
		{
			H_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_KICK:

		if (H_kick.IsEnded())
		{
			H_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_CROUCH_PUNCH:

		if (crouch_punch.IsEnded())
		{
			crouch_punch.RestartFrames();
			m_PlayerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_CROUCH_KICK:

		if (crouch_kick.IsEnded())
		{
			crouch_kick.RestartFrames();
			m_PlayerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_FORWARD_LOW_PUNCH:

		if (F_L_punch.IsEnded())
		{
			F_L_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_LOW_KICK:

		if (F_L_kick.IsEnded())
		{
			F_L_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:

		if (F_M_punch.IsEnded())
		{
			F_M_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:

		if (F_M_kick.IsEnded())
		{
			F_M_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:

		if (F_H_punch.IsEnded())
		{
			F_H_punch.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_KICK:

		if (F_H_kick.IsEnded())
		{
			F_H_kick.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIT:

		if (hit.IsEnded())
		{
			hit.RestartFrames();
			if (m_DistanceJumped > 0){
				m_GoingUp = false;
				m_PlayerState = PLAYER_JUMPING;
			}
			else
				m_PlayerState = PLAYER_IDLE;
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			m_AlreadyHitted = false;
		}
		break;

	case PLAYER_CROUCH_HIT:

		if (crouch_hit.IsEnded())
		{
			crouch_hit.RestartFrames();
			m_PlayerState = PLAYER_CROUCHING;
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			m_AlreadyHitted = false;
		}
		break;

	case PLAYER_FACE_HIT:

		if (face_hit.IsEnded())
		{
			face_hit.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			m_AlreadyHitted = false;
		}
		break;

	case PLAYER_YOGA_FIRE:
		if (yoga_fire.GetCurrentFrameNumber() == 3 && SDL_GetTicks() - m_LastShotTimer > 500)
		{
			if (m_LookingRight)
			{
				iPoint particlePosition(m_Position.x + 42 + 14, m_Position.y - 54 + 10);
				App->particles->newParticle(particlePosition, m_Graphics, m_Particle, m_DestroyParticle, 2);
			}
			else
			{
				iPoint particlePosition(m_Position.x - 42 - 14, m_Position.y - 54 + 10);
				App->particles->newParticle(particlePosition, m_Graphics, m_Particle, m_DestroyParticle, -2);
			}
			m_LastShotTimer = SDL_GetTicks();
		}

		if (yoga_fire.IsEnded())
		{
			yoga_fire.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_YOGA_FLAME:

		if (yoga_flame.IsEnded())
		{
			yoga_flame.RestartFrames();
			m_PlayerState = PLAYER_IDLE;

		}
		break;

	case PLAYER_YOGA_MUMMY:

		m_GoingUp = false;
		if (m_DistanceJumped == 0)
		{
			m_AlreadyHitted = false;
			yoga_mummy.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
			m_JumpAttacked = false;
		}
		else if (yoga_mummy.IsEnded() && m_DistanceJumped > 0)
		{
			yoga_mummy.RestartFrames();
			m_PlayerState = PLAYER_JUMPING;
			m_JumpAttacked = true;
		}
		else if (yoga_mummy.IsEnded())
		{
			yoga_mummy.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
			m_JumpAttacked = false;
		}
		break;

	case PLAYER_YOGA_SPEAR:

		m_GoingUp = false;
		if (m_DistanceJumped == 0)
		{
			m_AlreadyHitted = false;
			yoga_spear.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
			m_JumpAttacked = false;
		}
		else if (yoga_spear.IsEnded() && m_DistanceJumped > 0)
		{
			yoga_spear.RestartFrames();
			m_PlayerState = PLAYER_JUMPING;
			m_JumpAttacked = true;
		}
		else if (yoga_spear.IsEnded())
		{
			yoga_spear.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
			m_JumpAttacked = false;
		}
		break;

	case PLAYER_BLOCKING:
		if (!m_OtherPlayer->IsAttacking() ||
			(m_LookingRight && !(GetPlayerInput(INPUT_LEFT))) ||
			(!m_LookingRight && !(GetPlayerInput(INPUT_RIGHT))))
		{
			block.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		else if (m_OtherPlayer->IsAttacking() &&
			(GetPlayerInput(INPUT_DOWN)))
		{
			block.RestartFrames();
			m_PlayerState = PLAYER_CROUCH_BLOCKING;
		}
		else if (block.IsEnded())
			block.RestartFrames();
		break;

	case PLAYER_CROUCH_BLOCKING:
		if ((!m_OtherPlayer->IsAttacking() ||
			(m_LookingRight && !(GetPlayerInput(INPUT_LEFT))) ||
			(!m_LookingRight && !(GetPlayerInput(INPUT_RIGHT))))
			&& (GetPlayerInput(INPUT_DOWN)))
		{
			crouch_block.RestartFrames();
			m_PlayerState = PLAYER_CROUCHING;
		}
		else if (!(GetPlayerInput(INPUT_DOWN)) && (!m_OtherPlayer->IsAttacking() ||
			(m_LookingRight && !(GetPlayerInput(INPUT_LEFT))) ||
			(!m_LookingRight && !(GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			m_PlayerState = PLAYER_IDLE;
		}
		else if (m_OtherPlayer->IsAttacking() && !(GetPlayerInput(INPUT_DOWN)) &&
			((m_LookingRight && (GetPlayerInput(INPUT_LEFT))) ||
			(!m_LookingRight && (GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			m_PlayerState = PLAYER_BLOCKING;
		}
		else if (crouch_block.IsEnded())
			crouch_block.RestartFrames();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (block.IsEnded())
		{
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			m_AlreadyHitted = false;
			block.RestartFrames();
			m_PlayerState = PLAYER_BLOCKING;
		}
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (crouch_block.IsEnded())
		{
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			m_AlreadyHitted = false;
			crouch_block.RestartFrames();
			m_PlayerState = PLAYER_CROUCH_BLOCKING;
		}
		break;

	case PLAYER_AIR_HITTED:
		if (air_hit.IsEnded())
		{
			m_Hitted = false;
			m_HeadHitted = false;
			m_LegHitted = false;
			air_hit.RestartFrames();
			m_GoingUp = false;
			m_PlayerState = PLAYER_JUMPING;
		}
		break;
	}

	m_StartingCombo = COMBO_NOTHING;

	return UPDATE_CONTINUE;
}

// Update
UpdateStatus ModulePlayerDhalsim::Update()
{
	SDL_Rect aux;
	int pivot;
	Collider_player_structure cps;

	if (m_OtherPlayer->playerInCameraLimit() &&
		((App->renderer->ScreenLeftLimit() && m_OtherPlayer->GetLookingRight()) ||
		(App->renderer->ScreenRightLimit() && !m_OtherPlayer->GetLookingRight())) &&
		((m_OtherPlayer->GetPlayerState() == PLAYER_AIR_HITTED) ||
		(m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_HIT) ||
		(m_OtherPlayer->GetPlayerState() == PLAYER_HIT) ||
		(m_OtherPlayer->GetPlayerState() == PLAYER_FACE_HIT) ||
		(m_OtherPlayer->GetPlayerState() == PLAYER_BLOCKING_HITTED) ||
		(m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_BLOCKING_HITTED)) &&
		(m_PlayerState != PLAYER_JUMP_KICK) && (m_PlayerState != PLAYER_JUMP_PUNCH) && 
		(m_PlayerState != PLAYER_JUMPING) && (m_PlayerState != PLAYER_AIR_HITTED))
	{
		if (m_LookingRight && (m_OtherPlayer->GetPlayerState() == PLAYER_AIR_HITTED))
			MovePlayer(-3);
		else if (m_LookingRight)
			MovePlayer(-1);
		else if (m_OtherPlayer->GetPlayerState() == PLAYER_AIR_HITTED)
			MovePlayer(3);
		else
			MovePlayer(1);
	}

	switch (m_PlayerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		cps = idle.GetCurrentCollider();
		idle.NextFrame();
		break;

	case PLAYER_WALKING_FORWARD:
		if (!m_AreCollidingPlayers)
		{
			if (m_LookingRight)
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
		if (m_LookingRight)
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
		if (m_GoingUp && m_DistanceJumped < 105)
		{
			m_DistanceJumped += 3;
		}
		else if (m_GoingUp)
			m_GoingUp = false;
		else
		{
			m_DistanceJumped -= 3;
		}

		if (m_DirectionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (m_DirectionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		if (m_DistanceJumped > 90)
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
		m_ColliderAttack.damageType = L_ATTACK;
		m_ColliderAttack.damage = 12;
		break;

	case PLAYER_LOW_KICK:
		aux = L_kick.GetCurrentFrame();
		pivot = L_kick.GetCurrentPivot();
		cps = L_kick.GetCurrentCollider();
		L_kick.NextFrame();
		m_ColliderAttack.damageType = L_ATTACK;
		m_ColliderAttack.damage = 12;
		break;

	case PLAYER_MEDIUM_PUNCH:
		aux = M_punch.GetCurrentFrame();
		pivot = M_punch.GetCurrentPivot();
		cps = M_punch.GetCurrentCollider();
		M_punch.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 16;
		break;

	case PLAYER_MEDIUM_KICK:
		aux = M_kick.GetCurrentFrame();
		pivot = M_kick.GetCurrentPivot();
		cps = M_kick.GetCurrentCollider();
		M_kick.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 16;
		break;

	case PLAYER_HIGH_PUNCH:
		aux = H_punch.GetCurrentFrame();
		pivot = H_punch.GetCurrentPivot();
		cps = H_punch.GetCurrentCollider();
		H_punch.NextFrame();
		m_ColliderAttack.damageType = H_ATTACK;
		m_ColliderAttack.damage = 20;
		break;

	case PLAYER_HIGH_KICK:
		aux = H_kick.GetCurrentFrame();
		pivot = H_kick.GetCurrentPivot();
		cps = H_kick.GetCurrentCollider();
		H_kick.NextFrame();
		m_ColliderAttack.damageType = H_ATTACK;
		m_ColliderAttack.damage = 20;
		break;

	case PLAYER_FORWARD_LOW_PUNCH:
		aux = F_L_punch.GetCurrentFrame();
		pivot = F_L_punch.GetCurrentPivot();
		cps = F_L_punch.GetCurrentCollider();
		F_L_punch.NextFrame();
		m_ColliderAttack.damageType = L_ATTACK;
		m_ColliderAttack.damage = 14;
		break;

	case PLAYER_FORWARD_LOW_KICK:
		aux = F_L_kick.GetCurrentFrame();
		pivot = F_L_kick.GetCurrentPivot();
		cps = F_L_kick.GetCurrentCollider();
		F_L_kick.NextFrame();
		m_ColliderAttack.damageType = L_ATTACK;
		m_ColliderAttack.damage = 14;
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:
		aux = F_M_punch.GetCurrentFrame();
		pivot = F_M_punch.GetCurrentPivot();
		cps = F_M_punch.GetCurrentCollider();
		F_M_punch.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:
		aux = F_M_kick.GetCurrentFrame();
		pivot = F_M_kick.GetCurrentPivot();
		cps = F_M_kick.GetCurrentCollider();
		F_M_kick.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:
		aux = F_H_punch.GetCurrentFrame();
		pivot = F_H_punch.GetCurrentPivot();
		cps = F_H_punch.GetCurrentCollider();
		F_H_punch.NextFrame();
		m_ColliderAttack.damageType = H_ATTACK;
		m_ColliderAttack.damage = 22;
		break;

	case PLAYER_FORWARD_HIGH_KICK:
		aux = F_H_kick.GetCurrentFrame();
		pivot = F_H_kick.GetCurrentPivot();
		cps = F_H_kick.GetCurrentCollider();
		F_H_kick.NextFrame();
		m_ColliderAttack.damageType = H_ATTACK;
		m_ColliderAttack.damage = 22;
		break;

	case PLAYER_CROUCH_PUNCH:
		aux = crouch_punch.GetCurrentFrame();
		pivot = crouch_punch.GetCurrentPivot();
		cps = crouch_punch.GetCurrentCollider();
		crouch_punch.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 16;
		break;

	case PLAYER_CROUCH_KICK:
		if (m_LookingRight)
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
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 16;
		break;

	case PLAYER_JUMP_PUNCH:
		if (m_GoingUp && m_DistanceJumped < 105)
		{
			m_DistanceJumped += 3;
		}
		else if (m_GoingUp)
			m_GoingUp = false;
		else
		{
			m_DistanceJumped -= 3;
		}

		if (m_DirectionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (m_DirectionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_punch.GetCurrentFrame();
		pivot = jump_punch.GetCurrentPivot();
		cps = jump_punch.GetCurrentCollider();
		jump_punch.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
		break;


	case PLAYER_JUMP_KICK:
		if (m_GoingUp && m_DistanceJumped < 105)
		{
			m_DistanceJumped += 3;
		}
		else if (m_GoingUp)
			m_GoingUp = false;
		else
		{
			m_DistanceJumped -= 3;
		}

		if (m_DirectionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (m_DirectionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_kick.GetCurrentFrame();
		pivot = jump_kick.GetCurrentPivot();
		cps = jump_kick.GetCurrentCollider();
		jump_kick.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
		break;

	case PLAYER_HIT:
		if (!m_LookingRight)
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
		if (!m_LookingRight)
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
		if (!m_LookingRight)
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
		if (m_DistanceJumped > 0)
		{
			m_DistanceJumped -= 3;
		}
		if (!ko.IsEnded()) {
			if (m_LookingRight)
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
		if (m_DistanceJumped > 0)
		{
			m_DistanceJumped -= 3;
		}
		aux = victory1.GetCurrentFrame();
		pivot = victory1.GetCurrentPivot();
		cps = victory1.GetCurrentCollider();
		victory1.NextFrame();
		break;

	case PLAYER_WIN_2:
		if (m_DistanceJumped > 0)
		{
			m_DistanceJumped -= 3;
		}
		aux = victory2.GetCurrentFrame();
		pivot = victory2.GetCurrentPivot();
		cps = victory2.GetCurrentCollider();
		victory2.NextFrame();
		break;

	case PLAYER_TIME_OUT:
		if (m_DistanceJumped > 0)
		{
			m_DistanceJumped -= 3;
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
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 28;
		break;

	case PLAYER_YOGA_MUMMY:

		m_DistanceJumped -= 3;
		if (m_DirectionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (m_DirectionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_mummy.GetCurrentFrame();
		pivot = yoga_mummy.GetCurrentPivot();
		cps = yoga_mummy.GetCurrentCollider();
		yoga_mummy.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
		break;

	case PLAYER_YOGA_SPEAR:

		m_DistanceJumped -= 3;
		if (m_DirectionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (m_DirectionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_spear.GetCurrentFrame();
		pivot = yoga_spear.GetCurrentPivot();
		cps = yoga_spear.GetCurrentCollider();
		yoga_spear.NextFrame();
		m_ColliderAttack.damageType = M_ATTACK;
		m_ColliderAttack.damage = 18;
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
		if (!m_LookingRight)
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
		if (!m_LookingRight)
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
		if (!m_LookingRight)
		{
			MovePlayer(2);
			m_DirectionJump = JUMP_RIGHT;
		}
		else
		{
			MovePlayer(-2);
			m_DirectionJump = JUMP_LEFT;
		}
		m_DistanceJumped += 3;
		m_GoingUp = true;
		aux = air_hit.GetCurrentFrame();
		pivot = air_hit.GetCurrentPivot();
		cps = air_hit.GetCurrentCollider();
		air_hit.NextFrame();
		break;

	}

	m_PlayerCollider.rect = cps.Position_collider;
	m_ColliderHead.rect = cps.Collider_head;
	m_ColliderBody.rect = cps.Collider_body;
	m_ColliderLegs.rect = cps.Collider_legs;
	m_ColliderAttack.rect = cps.Collider_attack;

	if (m_LookingRight)
	{
		m_PlayerCollider.rect.x += m_Position.x;
		m_PlayerCollider.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderHead.rect.x += m_Position.x;
		m_ColliderHead.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderBody.rect.x += m_Position.x;
		m_ColliderBody.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderLegs.rect.x += m_Position.x;
		m_ColliderLegs.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderAttack.rect.x += m_Position.x;
		m_ColliderAttack.rect.y += m_Position.y - m_DistanceJumped;
		App->renderer->Blit(m_Graphics, m_Position.x - pivot, m_Position.y - aux.h - m_DistanceJumped, &(aux), 1.0f, SDL_FLIP_NONE);
	}
	else
	{
		m_PlayerCollider.rect.x = m_Position.x - m_PlayerCollider.rect.x - m_PlayerCollider.rect.w;
		m_PlayerCollider.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderHead.rect.x = m_Position.x - m_ColliderHead.rect.x - m_ColliderHead.rect.w;
		m_ColliderHead.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderBody.rect.x = m_Position.x - m_ColliderBody.rect.x - m_ColliderBody.rect.w;
		m_ColliderBody.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderLegs.rect.x = m_Position.x - m_ColliderLegs.rect.x - m_ColliderLegs.rect.w;
		m_ColliderLegs.rect.y += m_Position.y - m_DistanceJumped;
		m_ColliderAttack.rect.x = m_Position.x - m_ColliderAttack.rect.x - m_ColliderAttack.rect.w;
		m_ColliderAttack.rect.y += m_Position.y - m_DistanceJumped;
		App->renderer->Blit(m_Graphics, m_Position.x - aux.w + pivot, m_Position.y - aux.h - m_DistanceJumped, &(aux), 1.0f, SDL_FLIP_HORIZONTAL);
	}
	
	return UPDATE_CONTINUE;
}

// PostUpdate
UpdateStatus ModulePlayerDhalsim::PostUpdate()
{
	m_AreCollidingPlayers = false;
	return UPDATE_CONTINUE;
}

// On Collision
void ModulePlayerDhalsim::OnCollision(Collider* c1, Collider* c2)
{
	if (!m_Win && !m_Dead && !m_Time0){
		if (c1->type == COLLIDER_BODY_PLAYER_ONE && c2->type == COLLIDER_BODY_PLAYER_TWO)
			m_AreCollidingPlayers = true;
		else if (c1->type == COLLIDER_BODY_PLAYER_TWO && c2->type == COLLIDER_BODY_PLAYER_ONE)
			m_AreCollidingPlayers = true;
		else if (c1->type == COLLIDER_PLAYER_ONE && c2->type == COLLIDER_ATTACK_PLAYER_TWO && c2->rect.w * c2->rect.h != 0 && !m_AlreadyHitted)
		{
			if (!m_Hitted && m_PlayerState != PLAYER_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_BLOCKING_HITTED && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= c2->damage;
				m_AlreadyHitted = true;
			}
			else if ((m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= c2->damage;
				m_AlreadyHitted = true;
				m_LegHitted = true;
			}

			if (m_Life < 0)
			{
				App->scene_bison->RestartScene(m_OtherPlayer->m_Wins + 1);
				App->audio->PlayFx(m_AudioIdDead);
				m_Dead = true;
				m_Life = 0;
			}
			else if (!m_Hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(m_AudioIdLImpact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(m_AudioIdMImpact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(m_AudioIdHImpact);
			}

			m_Hitted = true;
			if (c1 == &m_ColliderHead)
				m_HeadHitted = true;
		}
		else if (c1->type == COLLIDER_PLAYER_TWO && c2->type == COLLIDER_ATTACK_PLAYER_ONE  && c2->rect.w * c2->rect.h != 0 && !m_AlreadyHitted)
		{
			if (!m_Hitted && m_PlayerState != PLAYER_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_BLOCKING_HITTED && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= c2->damage;
				m_AlreadyHitted = true;
			}
			else if ((m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || m_OtherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= c2->damage;
				m_AlreadyHitted = true;
				m_LegHitted = true;
			}


			if (m_Life < 0)
			{
				App->scene_bison->RestartScene(m_OtherPlayer->m_Wins + 1);
				App->audio->PlayFx(m_AudioIdDead);
				m_Dead = true;
				m_Life = 0;
			}
			else if (!m_Hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(m_AudioIdLImpact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(m_AudioIdMImpact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(m_AudioIdHImpact);
			}


			m_Hitted = true;
			if (c1 == &m_ColliderHead)
				m_HeadHitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_ONE || c1->type == COLLIDER_ATTACK_PLAYER_ONE) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!m_Hitted && m_PlayerState != PLAYER_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_BLOCKING_HITTED && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= 18;
				App->audio->PlayFx(m_AudioIdLImpact);
			}

			if (m_Life < 0)
			{
				App->scene_bison->RestartScene(m_OtherPlayer->m_Wins + 1);
				App->audio->PlayFx(m_AudioIdDead);
				m_Dead = true;
				m_Life = 0;
			}

			m_Hitted = true;
			if (c1 == &m_ColliderHead)
				m_HeadHitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_TWO || c1->type == COLLIDER_ATTACK_PLAYER_TWO) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!m_Hitted && m_PlayerState != PLAYER_BLOCKING && m_PlayerState != PLAYER_CROUCH_BLOCKING && m_PlayerState != PLAYER_BLOCKING_HITTED && m_PlayerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				m_Life -= 18;
				App->audio->PlayFx(m_AudioIdLImpact);
			}

			if (m_Life < 0)
			{
				App->scene_bison->RestartScene(m_OtherPlayer->m_Wins + 1);
				App->audio->PlayFx(m_AudioIdDead);
				m_Dead = true;
				m_Life = 0;
			}

			m_Hitted = true;
			if (c1 == &m_ColliderHead)
				m_HeadHitted = true;
		}
	}
}

// Returns true if the player is the limit of the camera
bool ModulePlayerDhalsim::playerInCameraLimit() const
{
	if (App->renderer->camera.x <= -((m_PlayerCollider.rect.x - m_PlayerCollider.rect.w)*SCREEN_SIZE))
		return true;
	if (App->renderer->camera.x - App->renderer->camera.w >= -((m_PlayerCollider.rect.x + m_PlayerCollider.rect.w * 2)*SCREEN_SIZE))
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
			if (!m_OtherPlayer->playerInCameraLimit())
			{
				m_Position.x += distance;
				if (App->renderer->camera.x - App->renderer->camera.w >= -((m_PlayerCollider.rect.x + m_PlayerCollider.rect.w*2)*SCREEN_SIZE))
					App->renderer->camera.x -= SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x - App->renderer->camera.w < -((m_PlayerCollider.rect.x + m_PlayerCollider.rect.w*2)*SCREEN_SIZE))
					m_Position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x - App->renderer->camera.w < -((m_PlayerCollider.rect.x + m_PlayerCollider.rect.w*2)*SCREEN_SIZE))
				m_Position.x += distance;
		}
	}
	else
	{
		if (!App->renderer->ScreenLeftLimit())
		{
			if (!m_OtherPlayer->playerInCameraLimit())
			{
				m_Position.x += distance;
				if (App->renderer->camera.x <= -(m_PlayerCollider.rect.x*SCREEN_SIZE) + (m_PlayerCollider.rect.w*SCREEN_SIZE))
					App->renderer->camera.x += SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x > -(m_PlayerCollider.rect.x*SCREEN_SIZE) + (m_PlayerCollider.rect.w*SCREEN_SIZE))
					m_Position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x > -(m_PlayerCollider.rect.x*SCREEN_SIZE) + (m_PlayerCollider.rect.w*SCREEN_SIZE))
				m_Position.x += distance;
		}
	}
}

// Resets the player states
void ModulePlayerDhalsim::restartPlayer(bool everything)
{

	if (m_NumPlayer == 1)
	{
		m_Position.x = 150;
		m_Position.y = 200;
	}
	else
	{
		m_Position.x = 300;
		m_Position.y = 200;
	}

	if (everything)
		m_Wins = 0;

	m_PlayerState = PLAYER_IDLE;
	m_AreCollidingPlayers = false;
	m_Jumping = false;
	m_Hitted = false;
	m_HeadHitted = false;
	m_Life = 200;
	m_Win = false;
	m_Dead = false;
	m_Time0 = false;
	m_StartingCombo = COMBO_NOTHING;

	ko.RestartFrames();
	victory1.RestartFrames();
	victory2.RestartFrames();

	m_DistanceJumped = 0;
	m_JumpAttacked = false;
	m_LegHitted = false;
	m_AlreadyHitted = false;
}

// Get and Set variables

iPoint ModulePlayerDhalsim::getPosition() const
{
	return m_Position;
}

bool ModulePlayerDhalsim::IsAttacking() const
{
	if (m_PlayerState == PLAYER_LOW_PUNCH ||
		m_PlayerState == PLAYER_LOW_KICK ||
		m_PlayerState == PLAYER_MEDIUM_PUNCH ||
		m_PlayerState == PLAYER_MEDIUM_KICK ||
		m_PlayerState == PLAYER_HIGH_PUNCH ||
		m_PlayerState == PLAYER_HIGH_KICK ||
		m_PlayerState == PLAYER_CROUCH_PUNCH ||
		m_PlayerState == PLAYER_CROUCH_KICK ||
		m_PlayerState == PLAYER_JUMP_PUNCH ||
		m_PlayerState == PLAYER_JUMP_KICK ||
		m_PlayerState == PLAYER_FORWARD_LOW_PUNCH ||
		m_PlayerState == PLAYER_FORWARD_LOW_KICK ||
		m_PlayerState == PLAYER_FORWARD_MEDIUM_PUNCH ||
		m_PlayerState == PLAYER_FORWARD_MEDIUM_KICK ||
		m_PlayerState == PLAYER_FORWARD_HIGH_PUNCH ||
		m_PlayerState == PLAYER_FORWARD_HIGH_KICK ||
		m_PlayerState == PLAYER_YOGA_FIRE ||
		m_PlayerState == PLAYER_YOGA_FLAME ||
		m_PlayerState == PLAYER_YOGA_MUMMY ||
		m_PlayerState == PLAYER_YOGA_SPEAR)
	{
		return true;
	}
	else if (App->particles->GetNumberParticles() > 0)
	{
		if (m_OtherPlayer->m_LookingRight)
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x > m_OtherPlayer->m_Position.x && App->particles->GetParticleSpeed(i) < 0)
					return true;
			}
		}
		else
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x < m_OtherPlayer->m_Position.x && App->particles->GetParticleSpeed(i) > 0)
					return true;
			}
		}
	}
	return false;
}

bool ModulePlayerDhalsim::GetPlayerInput(InputType actionKey)
{
	if (m_NumPlayer == 1)
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


int ModulePlayerDhalsim::GetWins() const
{
	return m_Wins;
}

void ModulePlayerDhalsim::SetWins(int wins)
{
	m_Wins = wins;
}

int ModulePlayerDhalsim::GetLife() const
{
	return m_Life;
}

void ModulePlayerDhalsim::SetLife(int life)
{
	m_Life = life;
}

void ModulePlayerDhalsim::DecreseLife(int life)
{
	m_Life -= life;
}

bool ModulePlayerDhalsim::GetWin() const
{
	return m_Win;
}

void ModulePlayerDhalsim::SetWin(bool win)
{
	m_Win = win;
}

bool ModulePlayerDhalsim::GetDead() const
{
	return m_Dead;
}

void ModulePlayerDhalsim::SetDead(bool dead)
{
	m_Dead = dead;
}

bool ModulePlayerDhalsim::GetTime_0() const
{
	return m_Time0;
}

void ModulePlayerDhalsim::SetTime_0(bool time_0)
{
	m_Time0 = time_0;
}

bool ModulePlayerDhalsim::GetJumping() const
{
	return m_Jumping;
}

void ModulePlayerDhalsim::SetJumping(bool jumping)
{
	m_Jumping = jumping;
}

bool ModulePlayerDhalsim::GetLookingRight() const
{
	return m_LookingRight;
}

void ModulePlayerDhalsim::SetLookingRight(bool lookingRight)
{
	m_LookingRight = lookingRight;
}

PlayerState ModulePlayerDhalsim::GetPlayerState() const
{
	return m_PlayerState;
}

void ModulePlayerDhalsim::SetPlayerState(PlayerState playerState)
{
	m_PlayerState = playerState;
}

ComboTypes ModulePlayerDhalsim::GetStartingCombo() const
{
	return m_StartingCombo;
}

void ModulePlayerDhalsim::SetStartingCombo(ComboTypes starting_combo)
{
	m_StartingCombo = starting_combo;
}

int ModulePlayerDhalsim::GetDistanceJumped() const
{
	return m_DistanceJumped;
}
