#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include <iostream>

using namespace std;

ModuleRender::ModuleRender()
{
	camera.x = - (100 * SCREEN_SIZE);
	camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	// debug camera
/*	int speed = 5;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (App->renderer->camera.y < 10*SCREEN_SIZE)
			App->renderer->camera.y += speed;	
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (App->renderer->camera.y > 0)
			App->renderer->camera.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (App->renderer->camera.x < 0)
			App->renderer->camera.x += speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (App->renderer->camera.x > - (200 * SCREEN_SIZE))
			App->renderer->camera.x -= speed;
	}

	//std::cout << camera.x << " : " << camera.w << std::endl;
*/
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed, SDL_RendererFlip flip_texture)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int)(camera.y) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, 0.0f, NULL, flip_texture) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawRect(SDL_Rect* rec)
{
	cout << "REC" << endl;
	cout << rec->x << " , " << rec->y << " , " << rec->w << " , " << rec->h << endl;
	SDL_Rect rec_aux;
	rec_aux.x = rec->x * SCREEN_SIZE + App->renderer->camera.x;
	rec_aux.y = rec->y * SCREEN_SIZE + App->renderer->camera.y;
	rec_aux.w = rec->w * SCREEN_SIZE;
	rec_aux.h = rec->h * SCREEN_SIZE;
	cout << rec_aux.x << " , " << rec_aux.y << " , " << rec_aux.w << " , " << rec_aux.h << endl;

	if(SDL_RenderFillRect(renderer, &rec_aux) != 0)
		return false;
	return true;
}

bool ModuleRender::DrawStaticRect(SDL_Rect* rec)
{
	SDL_Rect rec_aux;
	rec_aux.x = rec->x * SCREEN_SIZE;
	rec_aux.y = rec->y * SCREEN_SIZE;
	rec_aux.w = rec->w * SCREEN_SIZE;
	rec_aux.h = rec->h * SCREEN_SIZE;

	if (SDL_RenderFillRect(renderer, &rec_aux) != 0)
		return false;
	return true;
}

bool ModuleRender::ScreenLeftLimit() const
{
	if (camera.x == 0)
		return true;
	return false;
}

bool ModuleRender::ScreenRightLimit() const
{
	if (camera.x == -(200 * SCREEN_SIZE))
		return true;
	return false;
}