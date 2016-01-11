#ifndef __MODULEPLAYERTWO_H__
#define __MODULEPLAYERTWO_H__

#include "ModulePlayerDhalsim.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"

class ModulePlayerTwo :
	public ModulePlayerDhalsim
{
public:
	ModulePlayerTwo(bool start_enabled = true);
	virtual ~ModulePlayerTwo();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual void restartPlayer();
};

#endif