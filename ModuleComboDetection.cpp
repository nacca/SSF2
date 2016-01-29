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

update_status ModuleComboDetection::PreUpdate()
{
	--countdown_previous_state_player_one;
	--countdown_first_state_player_one;
	--countdown_previous_state_player_two;
	--countdown_first_state_player_two;
	switch (previous_state_player_one)
	{
		case NOTHING:
			if (App->player_one->GetPlayerInput(INPUT_DOWN))
			{
				if (App->player_one->GetPlayerState() == PLAYER_JUMPING)
					combo_air_start_player_one = true;
				previous_state_player_one = COMBO_DOWN;
				type_of_combo_player_one = COMBO_YOGA_FIRE;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->player_one->GetPlayerInput(INPUT_DOWN)) &&
				(App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->player_one->GetPlayerInput(INPUT_LEFT) && App->player_one->GetLooking_right()) ||
				(App->player_one->GetPlayerInput(INPUT_RIGHT) && !App->player_one->GetLooking_right()))
			{
				if (App->player_one->GetPlayerState() == PLAYER_JUMPING)
					combo_air_start_player_one = true;				previous_state_player_one = COMBO_DOWN_BACK;
				type_of_combo_player_one = AERIAL_COMBO;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->player_one->GetPlayerInput(INPUT_LEFT) && App->player_one->GetLooking_right()) ||
				(App->player_one->GetPlayerInput(INPUT_RIGHT) && !App->player_one->GetLooking_right()))
			{
				if (App->player_one->GetPlayerState() == PLAYER_JUMPING)
					combo_air_start_player_one = true;				previous_state_player_one = COMBO_BACK;
				type_of_combo_player_one = COMBO_YOGA_FLAME;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			break;

		case COMBO_BACK:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(INPUT_DOWN)) &&
				(((App->player_one->GetPlayerInput(INPUT_RIGHT)) && !App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && App->player_one->GetLooking_right())))
			{
				previous_state_player_one = COMBO_DOWN_BACK;
				countdown_previous_state_player_one = 15;
			}
			else if (((App->player_one->GetPlayerInput(INPUT_RIGHT)) && !App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && App->player_one->GetLooking_right()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;

		case COMBO_DOWN_BACK:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->player_one->GetPlayerInput(INPUT_DOWN)) &&
				(((App->player_one->GetPlayerInput(INPUT_RIGHT)) && !App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && App->player_one->GetLooking_right())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if ((App->player_one->GetPlayerInput(INPUT_DOWN)))
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
			else if ((App->player_one->GetPlayerInput(INPUT_DOWN)) &&
				(((App->player_one->GetPlayerInput(INPUT_RIGHT)) && App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && !App->player_one->GetLooking_right())))
			{
				previous_state_player_one = COMBO_DOWN_FRONT;
				countdown_previous_state_player_one = 15;
			}
			else if (App->player_one->GetPlayerInput(INPUT_DOWN))
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
			else if (combo_air_start_player_one && (App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->player_one->GetDistanceJumped() > 20) && 
				((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
				(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
				(App->player_two->GetPlayerInput(INPUT_H_PUNCH))))
			{
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(AERIAL_COMBO_PUNCH);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if (combo_air_start_player_one && (App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->player_one->GetDistanceJumped() > 20) &&
				((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
				(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
				(App->player_two->GetPlayerInput(INPUT_H_PUNCH))))
			{
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(AERIAL_COMBO_KICK);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->player_one->GetPlayerInput(INPUT_DOWN)) &&
				(((App->player_one->GetPlayerInput(INPUT_RIGHT)) && App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && !App->player_one->GetLooking_right())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if (((App->player_one->GetPlayerInput(INPUT_RIGHT)) && App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && !App->player_one->GetLooking_right()))
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
			else if ((App->player_one->GetPlayerInput(INPUT_L_PUNCH)) ||
				(App->player_one->GetPlayerInput(INPUT_M_PUNCH)) ||
				(App->player_one->GetPlayerInput(INPUT_H_PUNCH)))
			{
				if ((App->player_one->GetDistanceJumped() > 20) && combo_air_start_player_one && App->player_one->GetPlayerState() == PLAYER_JUMPING)
				{
					App->player_one->SetStartingCombo(AERIAL_COMBO_PUNCH);
				}
				else if (type_of_combo_player_one == COMBO_YOGA_FIRE)
				{
					App->player_one->SetStartingCombo(COMBO_YOGA_FIRE);
				}
				else if (type_of_combo_player_one == COMBO_YOGA_FLAME)
				{
					App->player_one->SetStartingCombo(COMBO_YOGA_FLAME);
				}
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->player_one->GetPlayerInput(INPUT_L_KICK)) ||
				(App->player_one->GetPlayerInput(INPUT_M_KICK)) ||
				(App->player_one->GetPlayerInput(INPUT_H_KICK)))
			{
				if ((App->player_one->GetDistanceJumped() > 20) && combo_air_start_player_one && App->player_one->GetPlayerState() == PLAYER_JUMPING)
				{
					App->player_one->SetStartingCombo(AERIAL_COMBO_KICK);
				}
				combo_air_start_player_one = false;
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if (((App->player_one->GetPlayerInput(INPUT_RIGHT)) && App->player_one->GetLooking_right()) ||
				((App->player_one->GetPlayerInput(INPUT_LEFT)) && !App->player_one->GetLooking_right()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;
	}


	switch (previous_state_player_two)
	{
	case NOTHING:
		if (App->player_two->GetPlayerInput(INPUT_DOWN))
		{
			if (App->player_two->GetPlayerState() == PLAYER_JUMPING)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_DOWN;
			type_of_combo_player_two = COMBO_YOGA_FIRE;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if ((App->player_two->GetPlayerInput(INPUT_DOWN)) &&
			(App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->player_two->GetPlayerInput(INPUT_LEFT) && App->player_two->GetLooking_right()) ||
			(App->player_two->GetPlayerInput(INPUT_RIGHT) && !App->player_two->GetLooking_right()))
		{
			if (App->player_two->GetPlayerState() == PLAYER_JUMPING)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_DOWN_BACK;
			type_of_combo_player_two = AERIAL_COMBO;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if ((App->player_two->GetPlayerInput(INPUT_LEFT) && App->player_two->GetLooking_right()) ||
			(App->player_two->GetPlayerInput(INPUT_RIGHT) && !App->player_two->GetLooking_right()))
		{
			if (App->player_two->GetPlayerState() == PLAYER_JUMPING)
				combo_air_start_player_two = true;
			previous_state_player_two = COMBO_BACK;
			type_of_combo_player_two = COMBO_YOGA_FLAME;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		break;

	case COMBO_BACK:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(INPUT_DOWN)) &&
			(((App->player_two->GetPlayerInput(INPUT_RIGHT)) && !App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && App->player_two->GetLooking_right())))
		{
			previous_state_player_two = COMBO_DOWN_BACK;
			countdown_previous_state_player_two = 15;
		}
		else if (((App->player_two->GetPlayerInput(INPUT_RIGHT)) && !App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && App->player_two->GetLooking_right()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;

	case COMBO_DOWN_BACK:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if ((App->player_two->GetPlayerInput(INPUT_DOWN)) &&
			(((App->player_two->GetPlayerInput(INPUT_RIGHT)) && !App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && App->player_two->GetLooking_right())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if ((App->player_two->GetPlayerInput(INPUT_DOWN)))
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
		else if ((App->player_two->GetPlayerInput(INPUT_DOWN)) &&
			(((App->player_two->GetPlayerInput(INPUT_RIGHT)) && App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && !App->player_two->GetLooking_right())))
		{
			previous_state_player_two = COMBO_DOWN_FRONT;
			countdown_previous_state_player_two = 15;
		}
		else if (App->player_two->GetPlayerInput(INPUT_DOWN))
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
		else if (combo_air_start_player_two && (App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->player_two->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_H_PUNCH))))
		{
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(AERIAL_COMBO_PUNCH);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (combo_air_start_player_two && (App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->player_two->GetDistanceJumped() > 20) &&
			((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_H_PUNCH))))
		{
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(AERIAL_COMBO_KICK);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((App->player_two->GetPlayerInput(INPUT_DOWN)) &&
			(((App->player_two->GetPlayerInput(INPUT_RIGHT)) && App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && !App->player_two->GetLooking_right())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if (((App->player_two->GetPlayerInput(INPUT_RIGHT)) && App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && !App->player_two->GetLooking_right()))
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
		else if ((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_H_PUNCH)))
		{
			if ((App->player_two->GetDistanceJumped() > 20) && combo_air_start_player_two && App->player_two->GetPlayerState() == PLAYER_JUMPING)
			{
				App->player_two->SetStartingCombo(AERIAL_COMBO_PUNCH);
			}
			else if (type_of_combo_player_two == COMBO_YOGA_FIRE)
			{
				App->player_two->SetStartingCombo(COMBO_YOGA_FIRE);
			}
			else if (type_of_combo_player_two == COMBO_YOGA_FLAME)
			{
				App->player_two->SetStartingCombo(COMBO_YOGA_FLAME);
			}
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (combo_air_start_player_two && 
			((App->player_two->GetPlayerInput(INPUT_L_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_M_PUNCH)) ||
			(App->player_two->GetPlayerInput(INPUT_H_PUNCH))))
		{
			if ((App->player_two->GetDistanceJumped() > 20) && combo_air_start_player_two && App->player_two->GetPlayerState() == PLAYER_JUMPING)
			{
				App->player_two->SetStartingCombo(AERIAL_COMBO_KICK);
			}
			combo_air_start_player_two = false;
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (((App->player_two->GetPlayerInput(INPUT_RIGHT)) && App->player_two->GetLooking_right()) ||
			((App->player_two->GetPlayerInput(INPUT_LEFT)) && !App->player_two->GetLooking_right()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;
	}

	return UPDATE_CONTINUE;
}