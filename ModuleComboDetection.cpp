#include "Application.h"
#include "ModuleInput.h"
#include "ModuleComboDetection.h"
#include "ModulePlayerDhalsim.h"
#include "SDL/include/SDL.h"

ModuleComboDetection::ModuleComboDetection(bool start_enabled) : Module(start_enabled)
{
	previous_state_player_one = NOTHING;
	countdown_previous_state_player_one = 0;
	countdown_first_state_player_one = 0;
	previous_state_player_two = NOTHING;
	countdown_previous_state_player_two = 0;
	countdown_first_state_player_two = 0;
	combo_air_start_player_one = false;
	combo_air_start_player_two = false;
}

ModuleComboDetection::~ModuleComboDetection()
{

}

UpdateStatus ModuleComboDetection::PreUpdate()
{
	--countdown_previous_state_player_one;
	--countdown_first_state_player_one;
	--countdown_previous_state_player_two;
	--countdown_first_state_player_two;
	switch (previous_state_player_one)
	{
		case NOTHING:
			if (App->player_one->GetPlayerInput(InputType_Down))
			{
				if (App->player_one->GetPlayerState() == PlayerState_Jumping)
					combo_air_start_player_one = true;
				previous_state_player_one = COMBO_DOWN;
				type_of_combo_player_one = ComboTypes_ComboYogaFire;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->player_one->GetPlayerInput(InputType_Down)) &&
				(App->player_one->GetPlayerState() == PlayerState_Jumping) &&
				(App->player_one->GetPlayerInput(InputType_Left) && App->player_one->GetLookingRight()) ||
				(App->player_one->GetPlayerInput(InputType_Right) && !App->player_one->GetLookingRight()))
			{
				if (App->player_one->GetPlayerState() == PlayerState_Jumping)
					combo_air_start_player_one = true;				previous_state_player_one = COMBO_DOWN_BACK;
				type_of_combo_player_one = ComboTypes_AerialCombo;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->player_one->GetPlayerInput(InputType_Left) && App->player_one->GetLookingRight()) ||
				(App->player_one->GetPlayerInput(InputType_Right) && !App->player_one->GetLookingRight()))
			{
				if (App->player_one->GetPlayerState() == PlayerState_Jumping)
					combo_air_start_player_one = true;				previous_state_player_one = COMBO_BACK;
				type_of_combo_player_one = ComboTypes_ComboYogaFlame;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			break;

		case COMBO_BACK:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(InputType_Down)) &&
				(((App->player_one->GetPlayerInput(InputType_Right)) && !App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && App->player_one->GetLookingRight())))
			{
				previous_state_player_one = COMBO_DOWN_BACK;
				countdown_previous_state_player_one = 15;
			}
			else if (((App->player_one->GetPlayerInput(InputType_Right)) && !App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && App->player_one->GetLookingRight()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;

		case COMBO_DOWN_BACK:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(InputType_Down)) &&
				(((App->player_one->GetPlayerInput(InputType_Right)) && !App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && App->player_one->GetLookingRight())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if ((App->player_one->GetPlayerInput(InputType_Down)))
			{
				previous_state_player_one = COMBO_DOWN;
				countdown_previous_state_player_one = 15;
			}
			break;

		case COMBO_DOWN:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(InputType_Down)) &&
				(((App->player_one->GetPlayerInput(InputType_Right)) && App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && !App->player_one->GetLookingRight())))
			{
				previous_state_player_one = COMBO_DOWN_FRONT;
				countdown_previous_state_player_one = 15;
			}
			else if (App->player_one->GetPlayerInput(InputType_Down))
			{
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			break;

		case COMBO_DOWN_FRONT:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if (combo_air_start_player_one && (App->player_one->GetPlayerState() == PlayerState_Jumping) &&
				(App->player_one->GetDistanceJumped() > 20) && 
				((App->player_two->GetPlayerInput(InputType_LPunch)) ||
				(App->player_two->GetPlayerInput(InputType_MPunch)) ||
				(App->player_two->GetPlayerInput(InputType_HPunch))))
			{
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(ComboTypes_AerialComboPunch);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if (combo_air_start_player_one && (App->player_one->GetPlayerState() == PlayerState_Jumping) &&
				(App->player_one->GetDistanceJumped() > 20) &&
				((App->player_two->GetPlayerInput(InputType_LPunch)) ||
				(App->player_two->GetPlayerInput(InputType_MPunch)) ||
				(App->player_two->GetPlayerInput(InputType_HPunch))))
			{
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(ComboTypes_AerialComboKick);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->player_one->GetPlayerInput(InputType_Down)) &&
				(((App->player_one->GetPlayerInput(InputType_Right)) && App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && !App->player_one->GetLookingRight())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if (((App->player_one->GetPlayerInput(InputType_Right)) && App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && !App->player_one->GetLookingRight()))
			{
				previous_state_player_one = COMBO_FRONT;
				countdown_previous_state_player_one = 15;
			}
			break;

		case COMBO_FRONT:

			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(InputType_LPunch)) ||
				(App->player_one->GetPlayerInput(InputType_MPunch)) ||
				(App->player_one->GetPlayerInput(InputType_HPunch)))
			{
				if ((App->player_one->GetDistanceJumped() > 20) && combo_air_start_player_one && App->player_one->GetPlayerState() == PlayerState_Jumping)
				{
					App->player_one->SetStartingCombo(ComboTypes_AerialComboPunch);
				}
				else if (type_of_combo_player_one == ComboTypes_ComboYogaFire)
				{
					App->player_one->SetStartingCombo(ComboTypes_ComboYogaFire);
				}
				else if (type_of_combo_player_one == ComboTypes_ComboYogaFlame)
				{
					App->player_one->SetStartingCombo(ComboTypes_ComboYogaFlame);
				}
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->player_one->GetPlayerInput(InputType_LKick)) ||
				(App->player_one->GetPlayerInput(InputType_MKick)) ||
				(App->player_one->GetPlayerInput(InputType_HKick)))
			{
				if ((App->player_one->GetDistanceJumped() > 20) && combo_air_start_player_one && App->player_one->GetPlayerState() == PlayerState_Jumping)
				{
					App->player_one->SetStartingCombo(ComboTypes_AerialComboKick);
				}
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if (((App->player_one->GetPlayerInput(InputType_Right)) && App->player_one->GetLookingRight()) ||
				((App->player_one->GetPlayerInput(InputType_Left)) && !App->player_one->GetLookingRight()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;
	}


	switch (previous_state_player_two)
	{
	case NOTHING:
		if (App->player_two->GetPlayerInput(InputType_Down))
		{
			if (App->player_two->GetPlayerState() == PlayerState_Jumping)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_DOWN;
			type_of_combo_player_two = ComboTypes_ComboYogaFire;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if ((App->player_two->GetPlayerInput(InputType_Down)) &&
			(App->player_two->GetPlayerState() == PlayerState_Jumping) &&
			(App->player_two->GetPlayerInput(InputType_Left) && App->player_two->GetLookingRight()) ||
			(App->player_two->GetPlayerInput(InputType_Right) && !App->player_two->GetLookingRight()))
		{
			if (App->player_two->GetPlayerState() == PlayerState_Jumping)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_DOWN_BACK;
			type_of_combo_player_two = ComboTypes_AerialCombo;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if ((App->player_two->GetPlayerInput(InputType_Left) && App->player_two->GetLookingRight()) ||
			(App->player_two->GetPlayerInput(InputType_Right) && !App->player_two->GetLookingRight()))
		{
			if (App->player_two->GetPlayerState() == PlayerState_Jumping)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_BACK;
			type_of_combo_player_two = ComboTypes_ComboYogaFlame;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		break;

	case COMBO_BACK:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(InputType_Down)) &&
			(((App->player_two->GetPlayerInput(InputType_Right)) && !App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && App->player_two->GetLookingRight())))
		{
			previous_state_player_two = COMBO_DOWN_BACK;
			countdown_previous_state_player_two = 15;
		}
		else if (((App->player_two->GetPlayerInput(InputType_Right)) && !App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && App->player_two->GetLookingRight()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;

	case COMBO_DOWN_BACK:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(InputType_Down)) &&
			(((App->player_two->GetPlayerInput(InputType_Right)) && !App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && App->player_two->GetLookingRight())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if ((App->player_two->GetPlayerInput(InputType_Down)))
		{
			previous_state_player_two = COMBO_DOWN;
			countdown_previous_state_player_two = 15;
		}
		break;

	case COMBO_DOWN:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(InputType_Down)) &&
			(((App->player_two->GetPlayerInput(InputType_Right)) && App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && !App->player_two->GetLookingRight())))
		{
			previous_state_player_two = COMBO_DOWN_FRONT;
			countdown_previous_state_player_two = 15;
		}
		else if (App->player_two->GetPlayerInput(InputType_Down))
		{
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		break;

	case COMBO_DOWN_FRONT:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if (combo_air_start_player_two && (App->player_two->GetPlayerState() == PlayerState_Jumping) &&
			(App->player_two->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(InputType_LPunch)) ||
			(App->player_two->GetPlayerInput(InputType_MPunch)) ||
			(App->player_two->GetPlayerInput(InputType_HPunch))))
		{
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(ComboTypes_AerialComboPunch);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (combo_air_start_player_two && (App->player_two->GetPlayerState() == PlayerState_Jumping) &&
			(App->player_two->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(InputType_LPunch)) ||
			(App->player_two->GetPlayerInput(InputType_MPunch)) ||
			(App->player_two->GetPlayerInput(InputType_HPunch))))
		{
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(ComboTypes_AerialComboKick);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((App->player_two->GetPlayerInput(InputType_Down)) &&
			(((App->player_two->GetPlayerInput(InputType_Right)) && App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && !App->player_two->GetLookingRight())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if (((App->player_two->GetPlayerInput(InputType_Right)) && App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && !App->player_two->GetLookingRight()))
		{
			previous_state_player_two = COMBO_FRONT;
			countdown_previous_state_player_two = 15;
		}
		break;

	case COMBO_FRONT:

		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(InputType_LPunch)) ||
			(App->player_two->GetPlayerInput(InputType_MPunch)) ||
			(App->player_two->GetPlayerInput(InputType_HPunch)))
		{
			if ((App->player_two->GetDistanceJumped() > 20) && combo_air_start_player_two && App->player_two->GetPlayerState() == PlayerState_Jumping)
			{
				App->player_two->SetStartingCombo(ComboTypes_AerialComboPunch);
			}
			else if (type_of_combo_player_two == ComboTypes_ComboYogaFire)
			{
				App->player_two->SetStartingCombo(ComboTypes_ComboYogaFire);
			}
			else if (type_of_combo_player_two == ComboTypes_ComboYogaFlame)
			{
				App->player_two->SetStartingCombo(ComboTypes_ComboYogaFlame);
			}
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((App->player_two->GetPlayerInput(InputType_LKick)) ||
			(App->player_two->GetPlayerInput(InputType_MKick)) ||
			(App->player_two->GetPlayerInput(InputType_HKick)))
		{
			if ((App->player_two->GetDistanceJumped() > 20) && combo_air_start_player_two && App->player_two->GetPlayerState() == PlayerState_Jumping)
			{
				App->player_two->SetStartingCombo(ComboTypes_AerialComboKick);
			}
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (((App->player_two->GetPlayerInput(InputType_Right)) && App->player_two->GetLookingRight()) ||
			((App->player_two->GetPlayerInput(InputType_Left)) && !App->player_two->GetLookingRight()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;
	}

	return UpdateStatus_Continue;
}