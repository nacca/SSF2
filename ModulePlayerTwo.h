#ifndef __MODULEPLAYERTWO_H__
#define __MODULEPLAYERTWO_H__

#include "ModulePlayerDhalsim.h"

class ModulePlayerTwo :
	public ModulePlayerDhalsim
{
public:
	ModulePlayerTwo(bool start_enabled = true);
	~ModulePlayerTwo();

	bool Start();
	update_status PreUpdate();
	void restartPlayer(bool everything);
};

#endif