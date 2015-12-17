#ifndef __MODULEPLAYERDHALSIM_H__
#define __MODULEPLAYERDHALSIM_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "ModulePlayer.h"

class ModulePlayerDhalsim : public ModulePlayer
{
public:
	ModulePlayerDhalsim(bool start_enabled = true);
	virtual ~ModulePlayerDhalsim();

	virtual bool Start();
	virtual update_status Update();
	virtual bool CleanUp();

private:
	const int sombra = 49;
	bool jumping;
	player_state playerState;
	Animation particula;
};

#endif //Dhalsim

