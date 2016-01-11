#ifndef __MODULECOMBODETECTION_H__
#define __MODULECOMBODETECTION_H__

#include "Module.h"
#include <vector>

enum previous_states{
	COMBO_DOWN,
	COMBO_DOWN_FRONT,
	COMBO_FRONT,
	COMBO_DOWN_BACK,
	COMBO_BACK,
	NOTHING
};

class ModuleComboDetection :
	public Module
{
public:
	ModuleComboDetection(bool start_enabled = true);
	virtual ~ModuleComboDetection();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual bool CleanUp();

public:
	previous_states previous_state_player_one;
	int countdown_previous_state_player_one;
	int countdown_first_state_player_one;
	combo_types type_of_combo_player_one;
	previous_states previous_state_player_two;
	int countdown_previous_state_player_two;
	int countdown_first_state_player_two;
	combo_types type_of_combo_player_two;
};

#endif