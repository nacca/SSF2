#pragma once
#include <vector>
#include <iostream>
using namespace std;

struct Collider_player_structure {
	SDL_Rect Position_collider;
	SDL_Rect Collider_head;
	SDL_Rect Collider_body;
	SDL_Rect Collider_legs;
	SDL_Rect Collider_attack;
};

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;
	vector<int> pivots;
	vector<Collider_player_structure> colliders;
	bool loop;
	bool endReached;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f), loop(true), endReached(false)
	{}

	~Animation()
	{
		frames.clear();
		pivots.clear();
		colliders.clear();
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= frames.size())
		{
			if (loop)
				current_frame = 0.0f;
			else
			{
				current_frame = (float)frames.size() - 1.0f;
				endReached = true;
			}
		}
		return frames[(int)current_frame];
	}

	int& GetCurrentPivot()
	{
		return pivots[(int)current_frame];
	}

	Collider_player_structure GetCurrentCollider()
	{
		return colliders[(int)current_frame];
	}

	bool IsEnded()
	{
		return endReached;
	}

	void RestartFrames()
	{
		endReached = false;
		current_frame = 0.0f;
	}
};