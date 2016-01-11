#ifndef __MODULEPLAYERDHALSIM_H__
#define __MODULEPLAYERDHALSIM_H__

#include "Module.h"
#include "Animation.h"
#include "Animation2.h"
#include "ParticleAnimation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"

struct SDL_Texture;

class ModulePlayerDhalsim : public Module
{
public:
	ModulePlayerDhalsim(bool start_enabled = true);
	virtual ~ModulePlayerDhalsim();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

	virtual bool IsAttacking();

	virtual iPoint getPosition();
	virtual void OnCollision(Collider* c1, Collider* c2);

	virtual void restartPlayer();
	void MovePlayer(int distance);
	virtual bool playerInCameraLimit() const;

public:
	int wins;
	int life;
	bool win;
	bool dead;
	bool time_0;
	ModulePlayerDhalsim* otherPlayer;
	bool jumping;
	bool looking_right;
	player_state playerState;
	combo_types starting_combo;


protected:
	SDL_Texture* graphics = nullptr;
	iPoint position;
	bool move_right;
	bool move_left;
	Collider collider_head;
	Collider collider_body;
	Collider collider_legs;
	Collider collider_attack;
	Collider player_collider;

	ParticleAnimation particula;
	ParticleAnimation destroy_particula;

	bool colliding_players;
	Uint32 lastShotTimer;

	Animation2 idle;
	Animation2 forward;
	Animation2 backward;
	Animation2 crouching;
	Animation2 block;
	Animation2 crouch_block;
	Animation2 jump;
	Animation2 L_punch;
	Animation2 L_kick;
	Animation2 M_punch;
	Animation2 M_kick;
	Animation2 H_punch;
	Animation2 H_kick;
	Animation2 F_L_punch;
	Animation2 F_M_punch;
	Animation2 F_H_punch;
	Animation2 F_L_kick;
	Animation2 F_M_kick;
	Animation2 F_H_kick;
	Animation2 crouch_punch;
	Animation2 crouch_kick;
	Animation2 jump_punch;
	Animation2 jump_kick;
	Animation2 hit;
	Animation2 face_hit;
	Animation2 crouch_hit;
	Animation2 air_hit;
	Animation2 ko;
	Animation2 victory1;
	Animation2 victory2;
	Animation2 time_out;
	Animation2 yoga_fire;
	Animation2 yoga_flame;
	Animation2 yoga_mummy;
	Animation2 yoga_spear;

	bool hitted;
	bool head_hitted;

	int distance_jumped;
	bool going_up;
	bool jump_attacked;

	int audio_id_yoga_fire;
	int audio_id_yoga_flame;
	int audio_id_dead;
	int audio_id_L_attack;
	int audio_id_M_attack;
	int audio_id_H_attack;
	int audio_id_L_impact;
	int audio_id_M_impact;
	int audio_id_H_impact;

	damage_type damageType;

	direction_jumping directionJump;
	direction_jumping directionMummy;

};

#endif //Dhalsim

