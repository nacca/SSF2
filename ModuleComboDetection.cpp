#include "Application.h"
#include "ModuleInput.h"
#include "ModuleComboDetection.h"
#include "ModulePlayerOne.h"
#include "ModulePlayerTwo.h"
#include "SDL/include/SDL.h"

ModuleComboDetection::ModuleComboDetection(bool start_enabled) : Module(start_enabled)
{
	previous_state_player_one = NOTHING;
	countdown_previous_state_player_one = 0;
	countdown_first_state_player_one = 0;
	previous_state_player_two = NOTHING;
	countdown_previous_state_player_two = 0;
	countdown_first_state_player_two = 0;
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
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				previous_state_player_one = COMBO_DOWN;
				type_of_combo_player_one = COMBO_YOGA_FIRE;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) &&
				(App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->player_one->GetLooking_right()) ||
				(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !App->player_one->GetLooking_right()))
			{
				previous_state_player_one = COMBO_DOWN_BACK;
				type_of_combo_player_one = AERIAL_COMBO;
				countdown_previous_state_player_one = 15;
				countdown_first_state_player_one = 75;
			}
			if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->player_one->GetLooking_right()) ||
				(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !App->player_one->GetLooking_right()))
			{
				previous_state_player_one = COMBO_BACK;
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
			else if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) &&
				(((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && !App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && App->player_one->GetLooking_right())))
			{
				previous_state_player_one = COMBO_DOWN_BACK;
				countdown_previous_state_player_one = 15;
			}
			else if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && !App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && App->player_one->GetLooking_right()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;

		case COMBO_DOWN_BACK:
			if (countdown_previous_state_player_one <= 0 || countdown_first_state_player_one <= 0)
			{
				previous_state_player_one = NOTHING;
			}
			else if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) &&
				(((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && !App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && App->player_one->GetLooking_right())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT))
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
			else if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) &&
				(((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && !App->player_one->GetLooking_right())))
			{
				previous_state_player_one = COMBO_DOWN_FRONT;
				countdown_previous_state_player_one = 15;
			}
			else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
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
			else if ((App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN))
			{
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(AERIAL_COMBO_PUNCH);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->player_one->GetPlayerState() == PLAYER_JUMPING) &&
				(App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
			{
				previous_state_player_one = NOTHING;
				App->player_one->SetStartingCombo(AERIAL_COMBO_KICK);
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) &&
				(((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && !App->player_one->GetLooking_right())))
			{
				countdown_previous_state_player_one = 15;
			}
			else if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && !App->player_one->GetLooking_right()))
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
			else if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN))
			{
				if (App->player_one->GetPlayerState() == PLAYER_JUMPING)
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
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if ((App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) ||
				(App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
			{
				if (App->player_one->GetPlayerState() == PLAYER_JUMPING)
				{
					App->player_one->SetStartingCombo(AERIAL_COMBO_KICK);
				}
				previous_state_player_one = NOTHING;
				countdown_previous_state_player_one = 0;
				countdown_first_state_player_one = 0;
			}
			else if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && App->player_one->GetLooking_right()) ||
				((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && !App->player_one->GetLooking_right()))
			{
				countdown_previous_state_player_one = 15;
			}
			break;
	}


	switch (previous_state_player_two)
	{
	case NOTHING:
		if ((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1))
		{
			previous_state_player_two = COMBO_DOWN;
			type_of_combo_player_two = COMBO_YOGA_FIRE;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)) &&
			(App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->player_two->GetLooking_right()) ||
			((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1) && !App->player_two->GetLooking_right()))
		{
			previous_state_player_two = COMBO_DOWN_BACK;
			type_of_combo_player_two = AERIAL_COMBO;
			countdown_previous_state_player_two = 15;
			countdown_first_state_player_two = 75;
		}
		if (((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1) && App->player_two->GetLooking_right()) ||
			((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1) && !App->player_two->GetLooking_right()))
		{
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
		else if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)) &&
			((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && !App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && App->player_two->GetLooking_right())))
		{
			previous_state_player_two = COMBO_DOWN_BACK;
			countdown_previous_state_player_two = 15;
		}
		else if ((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && !App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && App->player_two->GetLooking_right()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;

	case COMBO_DOWN_BACK:
		if (countdown_previous_state_player_two <= 0 || countdown_first_state_player_two <= 0)
		{
			previous_state_player_two = NOTHING;
		}
		else if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)) &&
			((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && !App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && App->player_two->GetLooking_right())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)))
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
		else if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)) &&
			((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && !App->player_two->GetLooking_right())))
		{
			previous_state_player_two = COMBO_DOWN_FRONT;
			countdown_previous_state_player_two = 15;
		}
		else if ((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1))
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
		else if ((App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->input->GetButton(8) == KEY_DOWN) ||
			(App->input->GetButton(11) == KEY_DOWN) ||
			(App->input->GetButton(12) == KEY_DOWN))
		{
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(AERIAL_COMBO_PUNCH);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((App->player_two->GetPlayerState() == PLAYER_JUMPING) &&
			(App->input->GetButton(9) == KEY_DOWN) ||
			(App->input->GetButton(10) == KEY_DOWN) ||
			(App->input->GetButton(13) == KEY_DOWN))
		{
			previous_state_player_two = NOTHING;
			App->player_two->SetStartingCombo(AERIAL_COMBO_KICK);
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if (((App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1)) &&
			((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && !App->player_two->GetLooking_right())))
		{
			countdown_previous_state_player_two = 15;
		}
		else if ((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && !App->player_two->GetLooking_right()))
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
		else if ((App->input->GetButton(8) == KEY_DOWN) ||
			(App->input->GetButton(11) == KEY_DOWN) ||
			(App->input->GetButton(12) == KEY_DOWN))
		{
			if (App->player_two->GetPlayerState() == PLAYER_JUMPING)
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
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((App->input->GetButton(9) == KEY_DOWN) ||
			(App->input->GetButton(10) == KEY_DOWN) ||
			(App->input->GetButton(13) == KEY_DOWN))
		{
			if (App->player_two->GetPlayerState() == PLAYER_JUMPING)
			{
				App->player_two->SetStartingCombo(AERIAL_COMBO_KICK);
			}
			previous_state_player_two = NOTHING;
			countdown_previous_state_player_two = 0;
			countdown_first_state_player_two = 0;
		}
		else if ((((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1)) && App->player_two->GetLooking_right()) ||
			(((App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1)) && !App->player_two->GetLooking_right()))
		{
			countdown_previous_state_player_two = 15;
		}
		break;
	}

	return UPDATE_CONTINUE;
}