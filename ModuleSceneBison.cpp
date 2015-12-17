#include "Globals.h"
#include "Application.h"
#include "ModuleSceneBison.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include "ModulePlayerDhalsim.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneBison::ModuleSceneBison(bool start_enabled) : Module(start_enabled)
{
	// Ground
	ground.x = 0;
	ground.y = 160;
	ground.w = 512;
	ground.h = 56;

	// Background
	background.x = 48;
	background.y = 44;
	background.w = 416;
	background.h = 116;

	// Air
	air.x = 108;
	air.y = 503;
	air.w = 404;
	air.h = 49;

	// Ceiling
	ceiling.x = 32;
	ceiling.y = 0;
	ceiling.w = 438;
	ceiling.h = 32;

	// Black surface
	black_surface.x = 110;
	black_surface.y = 590;
	black_surface.w = 384;
	black_surface.h = 22;

	// Black surface
	black_surface2.x = 110;
	black_surface2.y = 590;
	black_surface2.w = 384;
	black_surface2.h = 22;

	// Bell
	bell.x = 221;
	bell.y = 234;
	bell.w = 70;
	bell.h = 133;

	// Column 1
	column1.x = 82;
	column1.y = 248;
	column1.w = 20;
	column1.h = 134;

	// Column2
	column2.x = 410;
	column2.y = 248;
	column2.w = 20;
	column2.h = 134;

	// Couple with child
	couple_with_child.x = 104;
	couple_with_child.y = 337;
	couple_with_child.w = 40;
	couple_with_child.h = 55;
	
	// Two boys
	two_boys.x = 296;
	two_boys.y = 334;
	two_boys.w = 20;
	two_boys.h = 58;

	two_mans_one_ground.frames.push_back({ 144, 336, 24, 56 });
	two_mans_one_ground.frames.push_back({ 481, 432, 24, 56 });
	two_mans_one_ground.speed = 0.1f;

	praying_man_ground.frames.push_back({ 177, 333, 30, 63 });
	praying_man_ground.frames.push_back({ 374, 432, 30, 63 });
	praying_man_ground.speed = 0.1f;

	praying_man_up.frames.push_back({ 368, 329, 32, 62 });
	praying_man_up.frames.push_back({ 229, 432, 32, 62 });
	praying_man_up.speed = 0.1f;

	three_man.frames.push_back({ 320, 336, 39, 56 });
	three_man.frames.push_back({ 275, 432, 39, 56 });
	three_man.speed = 0.1f;
	

}

ModuleSceneBison::~ModuleSceneBison()
{}

// Load assets
bool ModuleSceneBison::Start()
{
	LOG("Loading bison scene");
	
	graphics = App->textures->Load("bison_stage_v2.png");

	if (!App->player_dhalsim->Enable())
		return false;

	//App->audio->PlayMusic("bison.wav");
	
	return true;
}

// UnLoad assets
bool ModuleSceneBison::CleanUp()
{
	LOG("Unloading bison scene");

	App->textures->Unload(graphics);
	App->player_dhalsim->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneBison::Update()
{
	// Draw everything --------------------------------------
	App->renderer->Blit(graphics, -28, 159, &ground, 1.0f); // Ground
	App->renderer->Blit(graphics, 0, 31, &air, 0.0f); // Air
	App->renderer->Blit(graphics, -28, 43, &background, 0.55f); // Background
	App->renderer->Blit(graphics, -16, -1, &ceiling, 0.8f); // Ceiling

	App->renderer->Blit(graphics, 221 - 28, 17, &bell, 1.0f); // Bell
	App->renderer->Blit(graphics, 82 - 28, 31, &column1, 1.0f); // Column1
	App->renderer->Blit(graphics, 410 - 28, 31, &column2, 1.0f); // Column2
	
	App->renderer->Blit(graphics, 296 - 28, 31 + 86, &two_boys, 1.0f); // Two boys
	App->renderer->Blit(graphics, 104 - 28, 31 + 89, &couple_with_child, 1.0f); // Couple with boy

	App->renderer->Blit(graphics, 144 - 28, 31 + 88, &(two_mans_one_ground.GetCurrentFrame()), 1.0f); //
	App->renderer->Blit(graphics, 177 - 28, 31 + 85, &(praying_man_ground.GetCurrentFrame()), 1.0f); //
	App->renderer->Blit(graphics, 369 - 28, 31 + 81, &(praying_man_up.GetCurrentFrame()), 1.0f); //
	App->renderer->Blit(graphics, 320 - 28, 31 + 88, &(three_man.GetCurrentFrame()), 1.0f); //

	App->renderer->Blit(graphics, 0, -(int)(App->renderer->camera.y / SCREEN_SIZE), &black_surface, 0.0f); // Black Surface
	App->renderer->Blit(graphics, 0, 214 - (int)(App->renderer->camera.y / SCREEN_SIZE), &black_surface2, 0.0f); // Black Surface

	return UPDATE_CONTINUE;
}