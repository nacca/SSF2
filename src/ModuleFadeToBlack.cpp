#include <math.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "SDL.h"

ModuleFadeToBlack::ModuleFadeToBlack (bool start_enabled):
Module (start_enabled)
{
}

ModuleFadeToBlack::~ModuleFadeToBlack ()
{
}

// Load assets
bool
ModuleFadeToBlack::Start ()
{
  LOG ("Preparing Fade Screen");
  SDL_SetRenderDrawBlendMode (App->renderer->renderer, SDL_BLENDMODE_BLEND);
  return true;
}

// Update: draw background
UpdateStatus
ModuleFadeToBlack::Update ()
{
  if (start_time > 0)
    {
      Uint32 now = SDL_GetTicks () - start_time;
      float normalized = (float) now / (float) total_time;

      if (normalized > 1.0f)
	normalized = 1.0f;

      if (!fading_in)
	normalized = 1.0f - normalized;

      // Draw a screen-size balck rectangle with alpha
      SDL_SetRenderDrawColor (App->renderer->renderer, 0, 0, 0,
			      (Uint8) (normalized * 255.0f));
      SDL_RenderFillRect (App->renderer->renderer, NULL);

      if (module_off == nullptr && module_on != nullptr)
	{
	  module_on->Enable ();
	  module_on = nullptr;
	}

      if (now >= total_time)
	{
	  if (fading_in)
	    {
	      if (module_off != nullptr)
		module_off->Disable ();
	      module_on->Enable ();

	      total_time += total_time;
	      start_time = SDL_GetTicks ();
	      fading_in = false;
	    }
	  else
	    {
	      start_time = 0;
	    }
	}
    }

  return UpdateStatus_Continue;
}

// Fade to black. At mid point deactivate one module, then activate the other
void
ModuleFadeToBlack::FadeToBlack (Module * module_on, Module * module_off,
				float time)
{
  fading_in = (module_off != nullptr) ? true : false;
  start_time = SDL_GetTicks ();
  total_time = (Uint32) (time * 0.5f * 1000.0f);

  this->module_on = module_on;
  this->module_off = module_off;
}

bool
ModuleFadeToBlack::isFading () const
{
  return start_time > 0;
}
