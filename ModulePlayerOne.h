#ifndef __MODULEPLAYERONE_H__
#define __MODULEPLAYERONE_H__

#include "ModulePlayerDhalsim.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModuleCollisions.h"
#include "Collider.h"

class ModulePlayerOne :	public ModulePlayerDhalsim
{
public:
	ModulePlayerOne(bool start_enabled = true);
	virtual ~ModulePlayerOne();

	virtual bool Start();
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual update_status PostUpdate();
	virtual bool CleanUp();

};

#endif