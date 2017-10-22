#include "Application.h"
#include "ModuleInput.h"
#include "ModuleComboDetection.h"
#include "ModulePlayerDhalsim.h"
#include "SDL/include/SDL.h"

ModuleComboDetection::ModuleComboDetection(PlayerID playerID, bool start_enabled)
	: Module(start_enabled),
	m_PreviousState(PreviousComboState_Nothing),
	m_TypeOfCombo(ComboTypes_ComboNothing),
	m_PreviousStateCountdown(0),
	m_FirstStateCountdown(0),
	m_IsAirCombo(false)
{
	m_PlayerID = playerID;
}

ModuleComboDetection::~ModuleComboDetection()
{

}

bool ModuleComboDetection::Start()
{
	if (m_PlayerID == PlayerID_One)
	{
		m_ModulePlayerDhalsim = App->player_one;
	}
	else
	{
		m_ModulePlayerDhalsim = App->player_two;
	}

	return true;
}

UpdateStatus ModuleComboDetection::PreUpdate()
{
	UpdateCounters();
	UpdateComboState();

	return UpdateStatus_Continue;
}

void ModuleComboDetection::UpdateCounters()
{
	--m_PreviousStateCountdown;
	--m_FirstStateCountdown;
}

void ModuleComboDetection::UpdateComboState()
{
	switch (m_PreviousState)
	{
	case PreviousComboState_Nothing:
		if (m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down))
		{
			if (m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping)
			{
				m_IsAirCombo = true;
			}
			m_PreviousState = PreviousComboState_Down;
			m_TypeOfCombo = ComboTypes_ComboYogaFire;
			m_PreviousStateCountdown = 15;
			m_FirstStateCountdown = 75;
		}
		if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)) &&
			(m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping) &&
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right) && !m_ModulePlayerDhalsim->IsLookingRight()))
		{
			if (m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping)
				m_IsAirCombo = true;				m_PreviousState = PreviousComboState_DownBack;
			m_TypeOfCombo = ComboTypes_AerialCombo;
			m_PreviousStateCountdown = 15;
			m_FirstStateCountdown = 75;
		}
		if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right) && !m_ModulePlayerDhalsim->IsLookingRight()))
		{
			if (m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping)
				m_IsAirCombo = true;				m_PreviousState = PreviousComboState_Back;
			m_TypeOfCombo = ComboTypes_ComboYogaFlame;
			m_PreviousStateCountdown = 15;
			m_FirstStateCountdown = 75;
		}
		break;

	case PreviousComboState_Back:
		if (m_PreviousStateCountdown <= 0 || m_FirstStateCountdown <= 0)
		{
			m_PreviousState = PreviousComboState_Nothing;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)) &&
			(((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && !m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && m_ModulePlayerDhalsim->IsLookingRight())))
		{
			m_PreviousState = PreviousComboState_DownBack;
			m_PreviousStateCountdown = 15;
		}
		else if (((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && !m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && m_ModulePlayerDhalsim->IsLookingRight()))
		{
			m_PreviousStateCountdown = 15;
		}
		break;

	case PreviousComboState_DownBack:
		if (m_PreviousStateCountdown <= 0 || m_FirstStateCountdown <= 0)
		{
			m_PreviousState = PreviousComboState_Nothing;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)) &&
			(((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && !m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && m_ModulePlayerDhalsim->IsLookingRight())))
		{
			m_PreviousStateCountdown = 15;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)))
		{
			m_PreviousState = PreviousComboState_Down;
			m_PreviousStateCountdown = 15;
		}
		break;

	case PreviousComboState_Down:
		if (m_PreviousStateCountdown <= 0 || m_FirstStateCountdown <= 0)
		{
			m_PreviousState = PreviousComboState_Nothing;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)) &&
			(((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && !m_ModulePlayerDhalsim->IsLookingRight())))
		{
			m_PreviousState = PreviousComboState_DownFront;
			m_PreviousStateCountdown = 15;
		}
		else if (m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down))
		{
			m_PreviousStateCountdown = 15;
			m_FirstStateCountdown = 75;
		}
		break;

	case PreviousComboState_DownFront:
		if (m_PreviousStateCountdown <= 0 || m_FirstStateCountdown <= 0)
		{
			m_PreviousState = PreviousComboState_Nothing;
		}
		else if (m_IsAirCombo && (m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping) &&
			(m_ModulePlayerDhalsim->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(InputType_LPunch)) ||
			(App->player_two->GetPlayerInput(InputType_MPunch)) ||
			(App->player_two->GetPlayerInput(InputType_HPunch))))
		{
			m_IsAirCombo = false;
			m_PreviousState = PreviousComboState_Nothing;
			m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_AerialComboPunch);
			m_PreviousStateCountdown = 0;
			m_FirstStateCountdown = 0;
		}
		else if (m_IsAirCombo && (m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping) &&
			(m_ModulePlayerDhalsim->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(InputType_LPunch)) ||
			(App->player_two->GetPlayerInput(InputType_MPunch)) ||
			(App->player_two->GetPlayerInput(InputType_HPunch))))
		{
			m_IsAirCombo = false;
			m_PreviousState = PreviousComboState_Nothing;
			m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_AerialComboKick);
			m_PreviousStateCountdown = 0;
			m_FirstStateCountdown = 0;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Down)) &&
			(((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && !m_ModulePlayerDhalsim->IsLookingRight())))
		{
			m_PreviousStateCountdown = 15;
		}
		else if (((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && !m_ModulePlayerDhalsim->IsLookingRight()))
		{
			m_PreviousState = PreviousComboState_Front;
			m_PreviousStateCountdown = 15;
		}
		break;

	case PreviousComboState_Front:

		if (m_PreviousStateCountdown <= 0 || m_FirstStateCountdown <= 0)
		{
			m_PreviousState = PreviousComboState_Nothing;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_LPunch)) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_MPunch)) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_HPunch)))
		{
			if ((m_ModulePlayerDhalsim->GetDistanceJumped() > 20) && m_IsAirCombo && m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping)
			{
				m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_AerialComboPunch);
			}
			else if (m_TypeOfCombo == ComboTypes_ComboYogaFire)
			{
				m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_ComboYogaFire);
			}
			else if (m_TypeOfCombo == ComboTypes_ComboYogaFlame)
			{
				m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_ComboYogaFlame);
			}
			m_IsAirCombo = false;
			m_PreviousState = PreviousComboState_Nothing;
			m_PreviousStateCountdown = 0;
			m_FirstStateCountdown = 0;
		}
		else if ((m_ModulePlayerDhalsim->GetPlayerInput(InputType_LKick)) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_MKick)) ||
			(m_ModulePlayerDhalsim->GetPlayerInput(InputType_HKick)))
		{
			if ((m_ModulePlayerDhalsim->GetDistanceJumped() > 20) && m_IsAirCombo && m_ModulePlayerDhalsim->GetPlayerState() == PlayerState_Jumping)
			{
				m_ModulePlayerDhalsim->SetStartingCombo(ComboTypes_AerialComboKick);
			}
			m_IsAirCombo = false;
			m_PreviousState = PreviousComboState_Nothing;
			m_PreviousStateCountdown = 0;
			m_FirstStateCountdown = 0;
		}
		else if (((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Right)) && m_ModulePlayerDhalsim->IsLookingRight()) ||
			((m_ModulePlayerDhalsim->GetPlayerInput(InputType_Left)) && !m_ModulePlayerDhalsim->IsLookingRight()))
		{
			m_PreviousStateCountdown = 15;
		}
		break;
	}
}
