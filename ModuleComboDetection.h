#ifndef __MODULECOMBODETECTION_H__
#define __MODULECOMBODETECTION_H__

#include "Module.h"
#include <vector>

enum PreviousComboState
{
	PreviousComboState_Down,
	PreviousComboState_DownFront,
	PreviousComboState_Front,
	PreviousComboState_DownBack,
	PreviousComboState_Back,
	PreviousComboState_Nothing
};

class ModulePlayerDhalsim;

class ModuleComboDetection :
	public Module
{
public:
	ModuleComboDetection(PlayerID playerID, bool start_enabled = true);
	~ModuleComboDetection();

	bool Start();
	UpdateStatus PreUpdate();

private:
	void UpdateCounters();
	void UpdateComboState();

	PlayerID m_PlayerID;
	ModulePlayerDhalsim* m_ModulePlayerDhalsim;

	PreviousComboState m_PreviousState;
	int m_PreviousStateCountdown;
	int m_FirstStateCountdown;
	ComboTypes m_TypeOfCombo;
	bool m_IsAirCombo;
};

#endif