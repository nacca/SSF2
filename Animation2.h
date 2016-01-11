#pragma once
#include <vector>
#include <iostream>
using namespace std;

struct AnimationStructure
{
	SDL_Rect frame;
	int pivot;
	Collider_player_structure colliders;
	int duration;

	AnimationStructure(SDL_Rect frame, int pivot,
		Collider_player_structure colliders, int duration)
		: frame(frame), pivot(pivot), colliders(colliders), duration(duration)
	{
	}

};

class Animation2
{
public:
	vector<AnimationStructure> frames;
	bool loop;
	bool endReached;

private:
	int current_frame;
	int frames_in_frame;

public:
	Animation2() : frames(), current_frame(0), frames_in_frame(0), loop(true), endReached(false)
	{}

	~Animation2()
	{
		frames.clear();
	}

	int GetCurrentFrameNumber()
	{
		return current_frame;
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[current_frame].frame;
	}

	int GetCurrentPivot()
	{
		return frames[current_frame].pivot;
	}

	Collider_player_structure GetCurrentCollider()
	{
		return frames[current_frame].colliders;
	}

	void NextFrame()
	{
		++frames_in_frame;
		if (frames[current_frame].duration == frames_in_frame)
		{
			frames_in_frame = 0;
			++current_frame;
		}
		if (current_frame == frames.size() && !loop){
			endReached = true;
			--current_frame;
		}
		else if (current_frame == frames.size())
		{
			frames_in_frame = 0;
			current_frame = 0;
		}
	}

	bool IsEnded()
	{
		return endReached;
	}

	void RestartFrames()
	{
		endReached = false;
		current_frame = 0;
		frames_in_frame = 0;
	}
};