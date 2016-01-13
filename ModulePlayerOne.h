#ifndef __MODULEPLAYERONE_H__
#define __MODULEPLAYERONE_H__

#include "ModulePlayerDhalsim.h"

class ModulePlayerOne :	public ModulePlayerDhalsim
{
public:
	ModulePlayerOne(bool start_enabled = true);
	~ModulePlayerOne();

	bool Start();
	update_status PreUpdate();
	void restartPlayer(bool everything);

};

#endif