#ifndef __MODULEPLAYERONE_H__
#define __MODULEPLAYERONE_H__

#include "ModulePlayerDhalsim.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"

class ModulePlayerOne :	public ModulePlayerDhalsim
{
public:
	ModulePlayerOne(bool start_enabled = true);
	virtual ~ModulePlayerOne();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual void restartPlayer();

};

#endif