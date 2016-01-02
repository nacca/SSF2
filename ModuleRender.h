#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_image\include\SDL_image.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;
//typedef enum SDL_RendererFlip;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool ScreenLeftLimit() const;
	bool ScreenRightLimit() const;

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed = 1.0f, SDL_RendererFlip flip_texture = SDL_FLIP_NONE);
	bool DrawRect(SDL_Rect* rec);
	bool DrawStaticRect(SDL_Rect* rec);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};

#endif // __MODULERENDER_H__