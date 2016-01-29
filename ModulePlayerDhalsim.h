#ifndef __MODULEPLAYERDHALSIM_H__
#define __MODULEPLAYERDHALSIM_H__

#include "Module.h"
#include "Animation.h"
#include "ParticleAnimation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"

struct SDL_Texture;

class ModulePlayerDhalsim : public Module
{
public:
	ModulePlayerDhalsim(int playerNum, bool start_enabled = true);
	virtual ~ModulePlayerDhalsim();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

	bool IsAttacking() const;

	iPoint getPosition() const;
	void OnCollision(Collider* c1, Collider* c2);

	virtual void restartPlayer(bool everything);
	void MovePlayer(int distance);
	bool playerInCameraLimit() const;

	virtual bool GetPlayerInput(input_type actionKey);

	int GetWins() const;
	void SetWins(int wins);

	int GetLife() const;
	void SetLife(int life);
	void DecreseLife(int life);

	bool GetWin() const;
	void SetWin(bool win);

	bool GetDead() const;
	void SetDead(bool dead);

	bool GetTime_0() const;
	void SetTime_0(bool time_0);

	bool GetJumping() const;
	void SetJumping(bool jumping);

	bool GetLooking_right() const;
	void SetLooking_right(bool looking_right);

	int GetDistanceJumped() const;

	player_state GetPlayerState() const;
	void SetPlayerState(player_state playerState);

	combo_types GetStartingCombo() const;
	void SetStartingCombo(combo_types starting_combo);

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

	Animation idle;
	Animation forward;
	Animation backward;
	Animation crouching;
	Animation block;
	Animation crouch_block;
	Animation jump;
	Animation L_punch;
	Animation L_kick;
	Animation M_punch;
	Animation M_kick;
	Animation H_punch;
	Animation H_kick;
	Animation F_L_punch;
	Animation F_M_punch;
	Animation F_H_punch;
	Animation F_L_kick;
	Animation F_M_kick;
	Animation F_H_kick;
	Animation crouch_punch;
	Animation crouch_kick;
	Animation jump_punch;
	Animation jump_kick;
	Animation hit;
	Animation face_hit;
	Animation crouch_hit;
	Animation air_hit;
	Animation ko;
	Animation victory1;
	Animation victory2;
	Animation time_out;
	Animation yoga_fire;
	Animation yoga_flame;
	Animation yoga_mummy;
	Animation yoga_spear;

	bool hitted;
	bool head_hitted;
	bool leg_hitted;
	bool already_hitted;

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

	ModulePlayerDhalsim* otherPlayer;

	int wins;
	int life;
	bool win;
	bool dead;
	bool time_0;
	bool jumping;
	bool looking_right;

	player_state playerState;
	combo_types starting_combo;

	int numPlayer;

};

#endif //Dhalsim

