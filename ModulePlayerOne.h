#ifndef __MODULEPLAYERONE_H__
#define __MODULEPLAYERONE_H__

#include "ModulePlayerDhalsim.h"

class ModulePlayerOne :	public ModulePlayerDhalsim
{
public:
	ModulePlayerOne(bool start_enabled = true);
	~ModulePlayerOne();

	bool Start();
	void restartPlayer(bool everything);

	bool GetPlayerInput(input_type actionKey);

};

#endif