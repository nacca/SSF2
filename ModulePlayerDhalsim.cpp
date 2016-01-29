#include "ModulePlayerDhalsim.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include "ModuleCollisions.h"
#include "ModuleParticleSystem.h"
#include "ModuleSceneBison.h"
#include "ModuleAudio.h"

// Basic Module Operations

// Constructors
ModulePlayerDhalsim::ModulePlayerDhalsim(int playerNum, bool start_enabled) : Module(start_enabled)
{
	numPlayer = playerNum;

	// idle animation
	idle.frames.push_back({ { 3, 23, 52, 87 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 59, 24, 49, 86 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 113, 29, 55, 81 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 174, 24, 52, 86 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 231, 23, 49, 87 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 284, 28, 54, 82 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 343, 24, 52, 86 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 399, 25, 49, 85 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	idle.frames.push_back({ { 454, 28, 54, 82 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });

	// forward animation
	forward.frames.push_back({ { 516, 25, 51, 85 }, 25, { { -12, -77, 26, 72 }, { 0, -78, 12, 13 }, { -6, -65, 17, 18 }, { -18, -46, 36, 45 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 572, 22, 50, 88 }, 25, { { -12, -77, 26, 72 }, { -1, -80, 12, 13 }, { -9, -67, 15, 18 }, { -18, -47, 34, 46 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 626, 20, 52, 90 }, 25, { { -12, -77, 26, 72 }, { -1, -82, 12, 13 }, { -9, -69, 17, 18 }, { -10, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 683, 22, 53, 88 }, 25, { { -12, -77, 26, 72 }, { 1, -80, 12, 13 }, { -8, -67, 23, 17 }, { -8, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 742, 23, 49, 87 }, 25, { { -12, -77, 26, 72 }, { -1, -78, 12, 13 }, { -11, -66, 23, 18 }, { -21, -48, 34, 47 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 797, 22, 52, 88 }, 25, { { -12, -77, 26, 72 }, { 2, -80, 12, 13 }, { -7, -67, 22, 18 }, { -7, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 853, 20, 52, 90 }, 25, { { -12, -77, 26, 72 }, { -1, -82, 12, 13 }, { -9, -69, 21, 18 }, { -10, -52, 19, 51 }, { 0, 0, 0, 0 } }, 10 });
	forward.frames.push_back({ { 909, 22, 50, 88 }, 25, { { -12, -77, 26, 72 }, { -1, -80, 12, 13 }, { -8, -67, 19, 17 }, { -13, -50, 28, 48 }, { 0, 0, 0, 0 } }, 10 });

	// backward animation
	backward.frames.push_back({ { 909, 22, 50, 88 }, 25, { { -12, -77, 26, 72 }, { -1, -80, 12, 13 }, { -8, -67, 19, 17 }, { -13, -50, 28, 48 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 853, 20, 52, 90 }, 25, { { -12, -77, 26, 72 }, { -1, -82, 12, 13 }, { -9, -69, 21, 18 }, { -10, -52, 19, 51 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 797, 22, 52, 88 }, 25, { { -12, -77, 26, 72 }, { 2, -80, 12, 13 }, { -7, -67, 22, 18 }, { -7, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 742, 23, 49, 87 }, 25, { { -12, -77, 26, 72 }, { -1, -78, 12, 13 }, { -11, -66, 23, 18 }, { -21, -48, 34, 47 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 683, 22, 53, 88 }, 25, { { -12, -77, 26, 72 }, { 1, -80, 12, 13 }, { -8, -67, 23, 17 }, { -8, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 626, 20, 52, 90 }, 25, { { -12, -77, 26, 72 }, { -1, -82, 12, 13 }, { -9, -69, 17, 18 }, { -10, -50, 19, 49 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 572, 22, 50, 88 }, 25, { { -12, -77, 26, 72 }, { -1, -80, 12, 13 }, { -9, -67, 15, 18 }, { -18, -47, 34, 46 }, { 0, 0, 0, 0 } }, 10 });
	backward.frames.push_back({ { 516, 25, 51, 85 }, 25, { { -12, -77, 26, 72 }, { 0, -78, 12, 13 }, { -6, -65, 17, 18 }, { -18, -46, 36, 45 }, { 0, 0, 0, 0 } }, 10 });

	// crouching animation
	crouching.frames.push_back({ { 1144, 42, 45, 68 }, 25, { { -8, -62, 26, 60 }, { 4, -67, 11, 13 }, { -8, -54, 22, 17 }, { -7, -36, 30, 35 }, { 0, 0, 0, 0 } }, 10 });
	crouching.frames.push_back({ { 1194, 52, 45, 58 }, 25, { { -8, -52, 26, 50 }, { 4, -57, 11, 13 }, { -8, -44, 24, 17 }, { -15, -27, 35, 26 }, { 0, 0, 0, 0 } }, 10});
	crouching.loop = false;

	block.frames.push_back({ { 1250, 25, 49, 85 }, 25, { { -23, -73, 26, 70 }, { -8, -81, 14, 15 }, { -20, -68, 28, 32 }, { -20, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	block.loop = false;

	crouch_block.frames.push_back({ { 1305, 49, 45, 61 }, 23, { { -8, -52, 26, 50 }, { 4, -57, 11, 13 }, { -8, -44, 24, 17 }, { -15, -27, 35, 26 }, { 0, 0, 0, 0 } }, 10 });
	crouch_block.loop = false;

	// jump animation
	jump.frames.push_back({ { 971, 42, 45, 68 }, 25, { { -12, -58, 26, 55 }, { 3, -68, 12, 14 }, { -11, -55, 26, 26 }, { -11, -40, 26, 37 }, { 0, 0, 0, 0 } }, 1 });
	jump.frames.push_back({ { 1023, 14, 34, 96 }, 8, { { -7, -85, 26, 70 }, { 5, -96, 12, 14 }, { -7, -89, 27, 31 }, { -7, -69, 27, 69 }, { 0, 0, 0, 0 } }, 22 });
	jump.frames.push_back({ { 1063, 24, 35, 86 }, 8, { { -5, -76, 26, 51 }, { 7, -86, 12, 14 }, { -5, -79, 23, 25 }, { -5, -59, 27, 34 }, { 0, 0, 0, 0 } }, 16 });
	jump.frames.push_back({ { 1101, 14, 34, 96 }, 8, { { -7, -85, 26, 70 }, { 5, -96, 12, 14 }, { -7, -89, 27, 31 }, { -7, -69, 27, 69 }, { 0, 0, 0, 0 } }, 22 });
	jump.loop = false;

	// Far attacks
	// L_punch animation
	L_punch.frames.push_back({ { 2, 170, 61, 78 }, 25, { { -13, -70, 26, 68 }, { 13, -77, 14, 14 }, { -2, -68, 29, 33 }, { -2, -44, 29, 42 }, { 0, 0, 0, 0 } }, 5 });
	L_punch.frames.push_back({ { 68, 175, 91, 73 }, 25, { { -13, -70, 26, 68 }, { 13, -77, 14, 14 }, { -2, -68, 29, 33 }, { -2, -44, 29, 42 }, { 13, -72, 44, 16 } }, 6 });
	L_punch.frames.push_back({ { 166, 170, 61, 78 }, 25, { { -13, -70, 26, 68 }, { 13, -77, 14, 14 }, { -2, -68, 29, 33 }, { -2, -44, 29, 42 }, { 0, 0, 0, 0 } }, 5});
	L_punch.loop = false;

	// M_punch animation
	M_punch.frames.push_back({ { 235, 178, 62, 70 }, 25, { { -12, -73, 26, 70 }, { 25, -62, 16, 19 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 0, 0, 0, 0 } }, 7 });
	M_punch.frames.push_back({ { 302, 205, 97, 43 }, 25, { { -12, -73, 26, 70 }, { 20, -43, 52, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 50, -43, 28, 12 } }, 4 });
	M_punch.frames.push_back({ { 403, 205, 144, 43 }, 25, { { -12, -73, 26, 70 }, { 22, -43, 92, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 95, -43, 28, 12 } }, 6 });
	M_punch.frames.push_back({ { 302, 205, 97, 43 }, 25, { { -12, -73, 26, 70 }, { 20, -43, 52, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 50, -43, 28, 12 } }, 4 });
	M_punch.frames.push_back({ { 235, 178, 62, 70 }, 25, { { -12, -73, 26, 70 }, { 25, -62, 16, 19 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 0, 0, 0, 0 } }, 5 });
	M_punch.loop = false;
	
	// H_punch animation
	H_punch.frames.push_back({ { 554, 178, 62, 70 }, 25, { { -12, -73, 26, 70 }, { 25, -62, 16, 19 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 0, 0, 0, 0 } }, 7 });
	H_punch.frames.push_back({ { 621, 205, 101, 43 }, 25, { { -12, -73, 26, 70 }, { 20, -43, 52, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 50, -43, 28, 12 } }, 4 });
	H_punch.frames.push_back({ { 727, 205, 153, 43 }, 25, { { -12, -73, 26, 70 }, { 22, -43, 92, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 95, -43, 28, 12 } }, 6 });
	H_punch.frames.push_back({ { 621, 205, 101, 43 }, 25, { { -12, -73, 26, 70 }, { 20, -43, 52, 12 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 50, -43, 28, 12 } }, 10 });
	H_punch.frames.push_back({ { 554, 178, 62, 70 }, 25, { { -12, -73, 26, 70 }, { 25, -62, 16, 19 }, { 6, -43, 25, 39 }, { -10, -43, 22, 39 }, { 0, 0, 0, 0 } }, 9 });
	H_punch.loop = false;

	// L_kick animation
	L_kick.frames.push_back({ { 2, 283, 51, 84 }, 25, { { -11, -74, 26, 72 }, { 16, -82, 13, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 2 });
	L_kick.frames.push_back({ { 59, 269, 88, 98 }, 25, { { -11, -74, 26, 72 }, { 8, -82, 45, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 3 });
	L_kick.frames.push_back({ { 153, 269, 116, 98 }, 25, { { -11, -74, 26, 72 }, { -3, -90, 93, 26 }, { 15, -82, 62, 28 }, { -5, -67, 29, 65 }, { 48, -90, 49, 20 } }, 8 });
	L_kick.frames.push_back({ { 59, 269, 88, 98 }, 25, { { -11, -74, 26, 72 }, { 8, -82, 45, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 4 });
	L_kick.frames.push_back({ { 2, 283, 51, 84 }, 25, { { -11, -74, 26, 72 }, { 16, -82, 13, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 5 });
	L_kick.loop = false;

	// M_kick animation
	M_kick.frames.push_back({ { 277, 283, 52, 84 }, 25, { { -11, -74, 26, 72 }, { 16, -82, 13, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 2 });
	M_kick.frames.push_back({ { 335, 281, 92, 86 }, 25, { { -11, -74, 26, 72 }, { 8, -82, 45, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 3 });
	M_kick.frames.push_back({ { 432, 264, 128, 103 }, 25, { { -11, -74, 26, 72 }, { -3, -90, 93, 26 }, { 15, -82, 62, 28 }, { -5, -67, 29, 65 }, { 69, -96, 33, 20 } }, 8 });
	M_kick.frames.push_back({ { 335, 281, 92, 86 }, 25, { { -11, -74, 26, 72 }, { 8, -82, 45, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 4 });
	M_kick.frames.push_back({ { 277, 283, 52, 84 }, 25, { { -11, -74, 26, 72 }, { 16, -82, 13, 16 }, { 0, -67, 29, 37 }, { 0, -46, 29, 44 }, { 0, 0, 0, 0 } }, 5 });
	M_kick.loop = false;

	// H_kick animation
	H_kick.frames.push_back({ { 569, 279, 58, 88 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 3 });
	H_kick.frames.push_back({ { 632, 272, 62, 95 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 3 });
	H_kick.frames.push_back({ { 698, 274, 91, 93 }, 25, { { -12, -73, 26, 70 }, { -7, -83, 44, 18 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 4 });
	H_kick.frames.push_back({ { 793, 270, 136, 97 }, 25, { { -12, -73, 26, 70 }, { 13, -83, 67, 29 }, { 13, -87, 94, 22 }, { -10, -68, 28, 66 }, { 68, -98, 42, 28 } }, 6 });
	H_kick.frames.push_back({ { 933, 274, 91, 93 }, 25, { { -12, -73, 26, 70 }, { -7, -83, 44, 18 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 10 });
	H_kick.frames.push_back({ { 1028, 284, 72, 83 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 9 });
	H_kick.loop = false;

	// Near attacks

	// F_L_punch animation
	F_L_punch.frames.push_back({ { 886, 147, 49, 101 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 2, -92, 32, 21 } }, 3 });
	F_L_punch.frames.push_back({ { 940, 178, 72, 70 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 6, -77, 41, 8 } }, 6 });
	F_L_punch.frames.push_back({ { 1016, 147, 49, 101 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 2, -92, 32, 21 } }, 5 });
	F_L_punch.loop = false;

	// F_M_punch animation
	F_M_punch.frames.push_back({ { 1073, 163, 59, 85 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 17, -59, 22, 25 } }, 5 });
	F_M_punch.frames.push_back({ { 1137, 129, 60, 119 }, 25, { { -12, -73, 26, 70 }, { 9, -96, 10, 33 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 21, -113, 5, 44 } }, 6 });
	F_M_punch.frames.push_back({ { 1201, 163, 59, 85 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 5 });
	F_M_punch.loop = false;

	// F_H_punch animation
	F_H_punch.frames.push_back({ { 1267, 164, 49, 84 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 7 });
	F_H_punch.frames.push_back({ { 1321, 161, 49, 87 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 4 });
	F_H_punch.frames.push_back({ { 1375, 167, 63, 81 }, 25, { { -12, -73, 26, 70 }, { 24, -67, 14, 19 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 12, -67, 29, 25 } }, 6 });
	F_H_punch.frames.push_back({ { 1442, 165, 65, 83 }, 25, { { -12, -73, 26, 70 }, { 24, -67, 14, 19 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 11, -65, 44, 31 } }, 10 });
	F_H_punch.frames.push_back({ { 1512, 167, 63, 81 }, 25, { { -12, -73, 26, 70 }, { 24, -67, 14, 19 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 9 });
	F_H_punch.frames.push_back({ { 1581, 161, 49, 87 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 9 });
	F_H_punch.loop = false;

	// F_L_kick animation
	F_L_kick.frames.push_back({ { 1107, 283, 52, 84 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 3 });
	F_L_kick.frames.push_back({ { 1165, 283, 89, 84 }, 25, { { -12, -73, 26, 70 }, { 5, -82, 48, 18 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 17, -87, 49, 24 } }, 8 });
	F_L_kick.frames.push_back({ { 1354, 283, 52, 84 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 5 });
	F_L_kick.loop = false;

	// F_M_kick animation
	F_M_kick.frames.push_back({ { 1107, 283, 52, 84 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 3 });
	F_M_kick.frames.push_back({ { 1259, 274, 91, 93 }, 25, { { -12, -73, 26, 70 }, { 5, -82, 48, 18 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 17, -87, 49, 24 } }, 8 });
	F_M_kick.frames.push_back({ { 1354, 283, 52, 84 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 5 });
	F_M_kick.loop = false;

	// F_H_kick animation
	F_H_kick.frames.push_back({ { 1414, 299, 49, 68 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 5 });
	F_H_kick.frames.push_back({ { 1468, 273, 55, 94 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 2 });
	F_H_kick.frames.push_back({ { 1526, 259, 66, 108 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 9, -80, 45, 33 } }, 8 });
	F_H_kick.frames.push_back({ { 1596, 273, 55, 94 }, 25, { { -12, -73, 26, 70 }, { 4, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 3 });
	F_H_kick.frames.push_back({ { 1657, 299, 49, 68 }, 25, { { -12, -73, 26, 70 }, { 2, -81, 14, 15 }, { -10, -68, 28, 32 }, { -10, -51, 28, 49 }, { 0, 0, 0, 0 } }, 5 });
	F_H_kick.loop = false;

	// Especial position attacks

	// crouch_punch animation
	crouch_punch.frames.push_back({ { 5, 410, 51, 56 }, 24, { { -13, -46, 28, 45 }, { 7, -57, 14, 15 }, { -8, -46, 27, 23 }, { -8, -26, 27, 25 }, { 0, 0, 0, 0 } }, 5 });
	crouch_punch.frames.push_back({ { 61, 434, 87, 32 }, 23, { { -13, -46, 28, 45 }, { 7, -31, 24, 29 }, { 21, -27, 42, 19 }, { -6, -33, 24, 29 }, { 0, 0, 0, 0 } }, 3 });
	crouch_punch.frames.push_back({ { 153, 434, 134, 32 }, 23, { { -13, -46, 28, 45 }, { 7, -31, 24, 29 }, { 30, -30, 83, 22 }, { -6, -33, 24, 29 }, { 79, -28, 32, 15 } }, 6 });
	crouch_punch.frames.push_back({ { 293, 434, 87, 32 }, 23, { { -13, -46, 28, 45 }, { 7, -31, 24, 29 }, { 21, -27, 42, 19 }, { -6, -33, 24, 29 }, { 0, 0, 0, 0 } }, 4 });
	crouch_punch.frames.push_back({ { 385, 410, 51, 56 }, 24, { { -13, -46, 28, 45 }, { 7, -57, 14, 15 }, { -8, -46, 27, 23 }, { -8, -26, 27, 25 }, { 0, 0, 0, 0 } }, 5 });
	crouch_punch.loop = false;

	// crouch_kick animation
	crouch_kick.frames.push_back({ { 444, 409, 47, 57 }, 23, { { -13, -46, 28, 45 }, { 7, -57, 14, 15 }, { -8, -46, 27, 23 }, { -8, -26, 27, 25 }, { 0, 0, 0, 0 } }, 2 });
	crouch_kick.frames.push_back({ { 496, 427, 86, 39 }, 43, { { -14, -26, 25, 23 }, { -35, -38, 14, 15 }, { -36, -33, 35, 31 }, { -1, -33, 40, 31 }, { -8, -29, 43, 24 } }, 14 });
	crouch_kick.frames.push_back({ { 586, 409, 47, 57 }, 23, { { -13, -46, 28, 45 }, { 7, -57, 14, 15 }, { -8, -46, 27, 23 }, { -8, -26, 27, 25 }, { 0, 0, 0, 0 } }, 5 });
	crouch_kick.loop = false;

	// jump_punch animation
	jump_punch.frames.push_back({ { 643, 393, 66, 74 }, 8, { { -3, -63, 26, 36 }, { 1, -68, 27, 29 }, { 5, -52, 43, 22 }, { 1, -43, 27, 19 }, { 0, 0, 0, 0 } }, 7 });
	jump_punch.frames.push_back({ { 714, 393, 102, 74 }, 8, { { -3, -63, 26, 36 }, { 1, -68, 27, 44 }, { 22, -56, 37, 32 }, { 59, -33, 32, 29 }, { 58, -30, 31, 23 } }, 10 });
	jump_punch.frames.push_back({ { 822, 393, 66, 74 }, 8, { { -3, -63, 26, 36 }, { 1, -68, 27, 29 }, { 5, -52, 43, 22 }, { 1, -43, 27, 19 }, { 0, 0, 0, 0 } }, 7 });
	jump_punch.loop = false;

	// jump_kick animation
	jump_kick.frames.push_back({ { 897, 394, 78, 73 }, 8, { { -4, -61, 27, 40 }, { 0, -63, 26, 30 }, { 0, -38, 26, 19 }, { 3, -47, 42, 22 }, { 0, 0, 0, 0 } }, 10 });
	jump_kick.frames.push_back({ { 978, 394, 117, 73 }, 8, { { -4, -61, 27, 40 }, { 0, -63, 26, 34 }, { 35, -36, 37, 20 }, { 56, -28, 51, 25 }, { 73, -26, 26, 19 } }, 12 });
	jump_kick.frames.push_back({ { 1098, 394, 78, 73 }, 8, { { -4, -61, 27, 40 }, { 0, -63, 26, 30 }, { 0, -38, 26, 19 }, { 3, -47, 42, 22 }, { 0, 0, 0, 0 } }, 10 });
	jump_kick.loop = false;

	// hit animation
	hit.frames.push_back({ { 7, 731, 49, 81 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	hit.frames.push_back({ { 64, 725, 49, 87 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	hit.frames.push_back({ { 117, 723, 49, 89 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	hit.loop = false;

	// face_hit animation
	face_hit.frames.push_back({ { 175, 728, 49, 84 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	face_hit.frames.push_back({ { 227, 725, 51, 87 }, 27, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	face_hit.frames.push_back({ { 282, 725, 61, 87 }, 36, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	face_hit.loop = false;

	// crouch_hit animation
	crouch_hit.frames.push_back({ { 350, 749, 45, 63 }, 22, { { -8, -62, 26, 60 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 30 });
	crouch_hit.loop = false;

	// air_hit animation
	air_hit.frames.push_back({ { 932, 755, 74, 57 }, 58, { { -59, -29, 76, 29 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 15 });
	air_hit.loop = false;

	// ko animation
	ko.frames.push_back({ { 932, 755, 74, 57 }, 58, { { -59, -29, 76, 29 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 30 });
	ko.frames.push_back({ { 1011, 783, 83, 29 }, 58, { { -59, -29, 76, 29 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 30 });
	ko.frames.push_back({ { 1098, 755, 74, 57 }, 58, { { -59, -29, 76, 29 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 30 });
	ko.frames.push_back({ { 1177, 783, 83, 29 }, 58, { { -59, -29, 76, 29 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 30 });
	ko.loop = false;

	// victory1 animation
	victory1.frames.push_back({ { 5, 843, 60, 96 }, 29, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 71, 843, 62, 96 }, 30, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 137, 843, 62, 96 }, 30, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 203, 858, 74, 81 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 280, 858, 74, 81 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 358, 843, 62, 96 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 426, 843, 62, 96 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 497, 843, 60, 96 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 426, 843, 62, 96 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 358, 843, 62, 96 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 280, 858, 74, 81 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 203, 858, 74, 81 }, 28, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 137, 843, 62, 96 }, 30, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 71, 843, 62, 96 }, 30, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.frames.push_back({ { 5, 843, 60, 96 }, 29, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory1.loop = true;

	// victory2 animation
	victory2.frames.push_back({ { 563, 871, 45, 68 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory2.frames.push_back({ { 613, 838, 49, 101 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory2.frames.push_back({ { 667, 837, 49, 102 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory2.frames.push_back({ { 722, 833, 57, 106 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	victory2.loop = false;

	// time_out animation
	time_out.frames.push_back({ { 785, 854, 59, 85 }, 25, { { -12, -73, 26, 70 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }, 10 });
	time_out.loop = true;

	// yoga fire animation
	yoga_fire.frames.push_back({ { 301, 492, 49, 84 }, 25, { { -12, -73, 26, 70 }, { -3, -82, 14, 15 }, { -12, -79, 29, 35 }, { -12, -35, 29, 33 }, { 0, 0, 0, 0 } }, 4 });
	yoga_fire.frames.push_back({ { 353, 489, 61, 87 }, 37, { { -12, -73, 26, 70 }, { -34, -91, 14, 15 }, { -28, -76, 29, 39 }, { -12, -35, 29, 33 }, { 0, 0, 0, 0 } }, 7 });
	yoga_fire.frames.push_back({ { 418, 495, 62, 81 }, 25, { { -12, -73, 26, 70 }, { 25, -70, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 3 });
	yoga_fire.frames.push_back({ { 485, 493, 68, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 6 });
	yoga_fire.frames.push_back({ { 557, 495, 62, 81 }, 25, { { -12, -73, 26, 70 }, { 25, -70, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 9 });
	yoga_fire.loop = false;

	// yoga flame animation
	yoga_flame.frames.push_back({ { 739, 492, 49, 84 }, 25, { { -12, -73, 26, 70 }, { -3, -82, 14, 15 }, { -12, -79, 29, 35 }, { -12, -35, 29, 33 }, { 0, 0, 0, 0 } }, 4 });
	yoga_flame.frames.push_back({ { 791, 489, 61, 87 }, 37, { { -12, -73, 26, 70 }, { -34, -91, 14, 15 }, { -28, -76, 29, 39 }, { -12, -35, 29, 33 }, { 0, 0, 0, 0 } }, 7 });
	yoga_flame.frames.push_back({ { 855, 495, 81, 81 }, 25, { { -12, -73, 26, 70 }, { 25, -70, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 7 });
	yoga_flame.frames.push_back({ { 940, 493, 87, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 42, -54, 20, 15 } }, 3 });
	yoga_flame.frames.push_back({ { 940, 493, 67, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 42, -54, 20, 15 } }, 1 });
	yoga_flame.frames.push_back({ { 940, 493, 87, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 42, -54, 20, 15 } }, 3 });
	yoga_flame.frames.push_back({ { 940, 493, 67, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 42, -54, 20, 15 } }, 1 });
	yoga_flame.frames.push_back({ { 1031, 493, 105, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 3 });
	yoga_flame.frames.push_back({ { 1031, 493, 85, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 1 });
	yoga_flame.frames.push_back({ { 1031, 493, 105, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 3 });
	yoga_flame.frames.push_back({ { 1031, 493, 85, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 1 });
	yoga_flame.frames.push_back({ { 1031, 493, 105, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 3 });
	yoga_flame.frames.push_back({ { 1031, 493, 85, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 1 });
	yoga_flame.frames.push_back({ { 1031, 493, 105, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 3 });
	yoga_flame.frames.push_back({ { 1031, 493, 85, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 48, -62, 32, 26 } }, 1 });
	yoga_flame.frames.push_back({ { 1140, 493, 108, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 3 });
	yoga_flame.frames.push_back({ { 1140, 493, 88, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 1 });
	yoga_flame.frames.push_back({ { 1253, 493, 102, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 3 });
	yoga_flame.frames.push_back({ { 1253, 493, 82, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 1 });
	yoga_flame.frames.push_back({ { 1359, 493, 101, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 3 });
	yoga_flame.frames.push_back({ { 1359, 493, 81, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 1 });
	yoga_flame.frames.push_back({ { 1465, 493, 68, 83 }, 25, { { -12, -73, 26, 70 }, { 31, -60, 14, 15 }, { -3, -78, 28, 38 }, { -10, -34, 28, 32 }, { 0, 0, 0, 0 } }, 5 });
	yoga_flame.loop = false;

	// yoga mummy animation
	yoga_mummy.frames.push_back({ { 1183, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1279, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1376, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1468, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1183, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1279, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1376, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1468, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1183, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1279, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1376, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.frames.push_back({ { 1468, 425, 89, 38 }, 45, { { -12, -46, 26, 27 }, { 30, -27, 14, 15 }, { -1, -38, 36, 38 }, { -33, -38, 32, 38 }, { 13, -38, 31, 38 } }, 5 });
	yoga_mummy.loop = false;

	// yoga spear animation
	yoga_spear.frames.push_back({ { 5, 501, 70, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 79, 501, 60, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 143, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 220, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 5, 501, 70, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 79, 501, 60, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 143, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 220, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 5, 501, 70, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 79, 501, 60, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 143, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.frames.push_back({ { 220, 501, 73, 75 }, 35, { { -12, -46, 26, 27 }, { -35, -75, 14, 15 }, { -30, -65, 29, 25 }, { -2, -41, 25, 22 }, { 7, -37, 33, 35 } }, 5 });
	yoga_spear.loop = false;

	// particula animation
	particula.frames.push_back({ { 627, 519, 27, 17 }, 13, { -4, -10, 8, 10 }, 6 });
	particula.frames.push_back({ { 627, 519, 0, 0 }, 13, { -4, -10, 8, 10 }, 1 });
	particula.frames.push_back({ { 663, 519, 26, 17 }, 13, { -4, -10, 8, 10 }, 6 });
	particula.frames.push_back({ { 663, 519, 0, 0 }, 13, { -4, -10, 8, 10 }, 1 });
	particula.frames.push_back({ { 698, 520, 26, 14 }, 13, { -4, -10, 8, 10 }, 6 });
	particula.frames.push_back({ { 698, 520, 0, 0 }, 13, { -4, -10, 8, 10 }, 1 });
	particula.loop = true;

	// destroy particula animation
	destroy_particula.frames.push_back({ { 636, 552, 16, 16 }, 8, { 0, 0, 0, 0 }, 4 });
	destroy_particula.frames.push_back({ { 660, 549, 8, 22 }, 4, { 0, 0, 0, 0 }, 4 });
	destroy_particula.frames.push_back({ { 674, 547, 16, 26 }, 8, { 0, 0, 0, 0 }, 4 });
	destroy_particula.frames.push_back({ { 696, 546, 22, 28 }, 11, { 0, 0, 0, 0 }, 4 });
	destroy_particula.loop = false;

	if (numPlayer == 1)
	{
		position.x = 150;
		position.y = 200;

		player_collider.type = COLLIDER_BODY_PLAYER_ONE;
		player_collider.module = this;

		collider_head.type = COLLIDER_PLAYER_ONE;
		collider_head.module = this;

		collider_body.type = COLLIDER_PLAYER_ONE;
		collider_body.module = this;

		collider_legs.type = COLLIDER_PLAYER_ONE;
		collider_legs.module = this;

		collider_attack.type = COLLIDER_ATTACK_PLAYER_ONE;
		collider_attack.module = this;
	}
	else
	{
		position.x = 300;
		position.y = 200;

		player_collider.type = COLLIDER_BODY_PLAYER_TWO;
		player_collider.module = this;

		collider_head.type = COLLIDER_PLAYER_TWO;
		collider_head.module = this;

		collider_body.type = COLLIDER_PLAYER_TWO;
		collider_body.module = this;

		collider_legs.type = COLLIDER_PLAYER_TWO;
		collider_legs.module = this;

		collider_attack.type = COLLIDER_ATTACK_PLAYER_TWO;
		collider_attack.module = this;
	}

	playerState = PLAYER_IDLE;
	colliding_players = false;
	jumping = false;
	hitted = false;
	head_hitted = false;
	leg_hitted = false;
	life = 200;
	wins = 0;
	win = false;
	dead = false;
	time_0 = false;
	starting_combo = COMBO_NOTHING;
	damageType = NONE_DAMAGE;
	distance_jumped = 0;
	jump_attacked = false;
	already_hitted = false;
	lastShotTimer = SDL_GetTicks();
}

// Destructor
ModulePlayerDhalsim::~ModulePlayerDhalsim()
{
}

// Load assets
bool ModulePlayerDhalsim::Start()
{
	LOG("Loading Dhalsim");

	if (numPlayer == 1)
	{
		graphics = App->textures->Load("dhalsim.png");
		otherPlayer = App->player_two;
	}
	else
	{
		graphics = App->textures->Load("dhalsim_recolor.png");
		otherPlayer = App->player_one;
	}

	App->collisions->AddCollider(&player_collider);
	App->collisions->AddCollider(&collider_head);
	App->collisions->AddCollider(&collider_body);
	App->collisions->AddCollider(&collider_legs);
	App->collisions->AddCollider(&collider_attack);

	audio_id_yoga_fire = App->audio->LoadFx("yoga_fire.wav");
	audio_id_yoga_flame = App->audio->LoadFx("yoga_flame.wav");
	audio_id_dead = App->audio->LoadFx("dhalsim_dead.wav");
	audio_id_L_attack = App->audio->LoadFx("SF2_hit_1.wav");;
	audio_id_M_attack = App->audio->LoadFx("SF2_hit_2.wav");;
	audio_id_H_attack = App->audio->LoadFx("SF2_hit_3.wav");;
	audio_id_L_impact = App->audio->LoadFx("SF2_impact_1.wav");;
	audio_id_M_impact = App->audio->LoadFx("SF2_impact_2.wav");;
	audio_id_H_impact = App->audio->LoadFx("SF2_impact_3.wav");;

	if (otherPlayer->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	return true;
}

// Unload assets
bool ModulePlayerDhalsim::CleanUp()
{
	LOG("Unloading Base player");

	App->textures->Unload(graphics);

	return true;
}

// PreUpdate
update_status ModulePlayerDhalsim::PreUpdate()
{
	bool near = false;

	if (otherPlayer->getPosition().x > position.x)
		looking_right = true;
	else
		looking_right = false;

	if ((otherPlayer->getPosition().x - position.x) < 50 &&
		(otherPlayer->getPosition().x - position.x) > -50)
		near = true;

	if (dead)
	{
		if (playerState != PLAYER_KO)
		{
			playerState = PLAYER_KO;
			otherPlayer->SetWin(true);

		}
		return UPDATE_CONTINUE;
	}
	else if (time_0)
	{
		playerState = PLAYER_TIME_OUT;
	}
	else if (win &&
		(playerState == PLAYER_IDLE ||
		playerState == PLAYER_CROUCHING ||
		playerState == PLAYER_WALKING_FORWARD ||
		playerState == PLAYER_WALKING_BACKWARD))
	{
		if (playerState != PLAYER_WIN_1 && playerState != PLAYER_WIN_2)
		{
			if (wins == 0)
			{
				++wins;
				playerState = PLAYER_WIN_1;
			}
			else
			{
				++wins;
				playerState = PLAYER_WIN_2;
			}
		}
		return UPDATE_CONTINUE;
	}
	else if (playerState == PLAYER_BLOCKING_HITTED || playerState == PLAYER_CROUCH_BLOCKING_HITTED);
	else if (leg_hitted && playerState == PLAYER_BLOCKING)
	{
		playerState = PLAYER_HIT;
	}
	else if (hitted && playerState == PLAYER_BLOCKING)
	{
		playerState = PLAYER_BLOCKING_HITTED;
		block.RestartFrames();
	}
	else if (hitted && playerState == PLAYER_CROUCH_BLOCKING)
	{
		playerState = PLAYER_CROUCH_BLOCKING_HITTED;
		crouch_block.RestartFrames();
	}
	else if ((hitted || head_hitted) && (playerState == PLAYER_JUMPING || playerState == PLAYER_JUMP_PUNCH || playerState == PLAYER_JUMP_KICK || playerState == PLAYER_AIR_HITTED))
	{
		playerState = PLAYER_AIR_HITTED;
	}
	else if ((hitted || head_hitted) && (playerState == PLAYER_CROUCHING || playerState == PLAYER_CROUCH_PUNCH || playerState == PLAYER_CROUCH_KICK))
	{
		playerState = PLAYER_CROUCH_HIT;
	}
	else if (head_hitted)
	{
		playerState = PLAYER_FACE_HIT;
	}
	else if (hitted)
	{
		playerState = PLAYER_HIT;
	}

	switch (playerState)
	{

	case PLAYER_IDLE:

		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			looking_right)
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			looking_right && otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			looking_right)
		{
			playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else if (GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_STATIC;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		break;

	case PLAYER_WALKING_FORWARD:

		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (starting_combo == AERIAL_COMBO_PUNCH)
		{
			playerState = PLAYER_YOGA_MUMMY;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (!looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else if (otherPlayer->IsAttacking())
				playerState = PLAYER_BLOCKING;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else
			playerState = PLAYER_IDLE;

		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		break;

	case PLAYER_WALKING_BACKWARD:

		if (starting_combo == COMBO_YOGA_FIRE)
		{
			App->audio->PlayFx(audio_id_yoga_fire);
			playerState = PLAYER_YOGA_FIRE;
		}
		else if (starting_combo == COMBO_YOGA_FLAME)
		{
			App->audio->PlayFx(audio_id_yoga_flame);
			playerState = PLAYER_YOGA_FLAME;
		}
		else if (otherPlayer->IsAttacking())
		{
			playerState = PLAYER_BLOCKING;
			break;
		}
		else if (GetPlayerInput(INPUT_RIGHT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_RIGHT;
		}
		else if (GetPlayerInput(INPUT_LEFT) &&
			GetPlayerInput(INPUT_UP))
		{
			distance_jumped = 0;
			going_up = true;
			playerState = PLAYER_JUMPING;
			directionJump = JUMP_LEFT;
		}
		else if (GetPlayerInput(INPUT_RIGHT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_FORWARD;
			else
				playerState = PLAYER_WALKING_BACKWARD;
		}
		else if (GetPlayerInput(INPUT_LEFT))
		{
			if (looking_right)
				playerState = PLAYER_WALKING_BACKWARD;
			else
				playerState = PLAYER_WALKING_FORWARD;
		}
		else if (GetPlayerInput(INPUT_DOWN))
		{
			playerState = PLAYER_CROUCHING;
		}
		else
		{
			playerState = PLAYER_IDLE;
		}
		if (near && starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_FORWARD_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_FORWARD_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_FORWARD_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_FORWARD_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_FORWARD_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_FORWARD_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}
		else if (starting_combo == COMBO_NOTHING)
		{
			if (GetPlayerInput(INPUT_L_PUNCH))
			{
				playerState = PLAYER_LOW_PUNCH;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_L_KICK))
			{
				playerState = PLAYER_LOW_KICK;
				App->audio->PlayFx(audio_id_L_attack);
			}
			else if (GetPlayerInput(INPUT_M_PUNCH))
			{
				playerState = PLAYER_MEDIUM_PUNCH;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_M_KICK))
			{
				playerState = PLAYER_MEDIUM_KICK;
				App->audio->PlayFx(audio_id_M_attack);
			}
			else if (GetPlayerInput(INPUT_H_PUNCH))
			{
				playerState = PLAYER_HIGH_PUNCH;
				App->audio->PlayFx(audio_id_H_attack);
			}
			else if (GetPlayerInput(INPUT_H_KICK))
			{
				playerState = PLAYER_HIGH_KICK;
				App->audio->PlayFx(audio_id_H_attack);
			}
		}

		break;

	case PLAYER_JUMPING:

		if (distance_jumped == 0 && !going_up)
		{
			already_hitted = false;
			playerState = PLAYER_IDLE;
			jump_attacked = false;
			jump.RestartFrames();
		}
		else if (starting_combo == AERIAL_COMBO_PUNCH && !jump_attacked)
		{
			already_hitted = false;
			jump_attacked = true;
			playerState = PLAYER_YOGA_MUMMY;
			if (looking_right)
				directionMummy = JUMP_RIGHT;
			else
				directionMummy = JUMP_LEFT;
		}
		else if (starting_combo == AERIAL_COMBO_KICK && !jump_attacked)
		{
			already_hitted = false;
			jump_attacked = true;
			playerState = PLAYER_YOGA_SPEAR;
			if (looking_right)
				directionMummy = JUMP_RIGHT;
			else
				directionMummy = JUMP_LEFT;
		}
		else
		{
			if (((GetPlayerInput(INPUT_L_PUNCH)) ||
				(GetPlayerInput(INPUT_M_PUNCH)) ||
				(GetPlayerInput(INPUT_H_PUNCH))) &&
				!jump_attacked)
			{
				jump_attacked = true;
				playerState = PLAYER_JUMP_PUNCH;
			}

			else if (((GetPlayerInput(INPUT_L_KICK)) ||
				(GetPlayerInput(INPUT_M_KICK)) ||
				(GetPlayerInput(INPUT_H_KICK))) &&
				!jump_attacked)
			{
				jump_attacked = true;
				playerState = PLAYER_JUMP_KICK;
			}
		}
		break;


	case PLAYER_JUMP_PUNCH:

		if (distance_jumped == 0)
		{
			already_hitted = false;
			jump_attacked = false;
			playerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_punch.RestartFrames();
		}
		else if (jump_punch.IsEnded())
		{
			jump_punch.RestartFrames();
			playerState = PLAYER_JUMPING;
		}
		break;

	case PLAYER_JUMP_KICK:
		if (distance_jumped == 0)
		{
			already_hitted = false;
			jump_attacked = false;
			playerState = PLAYER_IDLE;
			jump.RestartFrames();
			jump_kick.RestartFrames();
		}
		else if (jump_kick.IsEnded())
		{
			jump_kick.RestartFrames();
			playerState = PLAYER_JUMPING;
		}
		break;


	case PLAYER_CROUCHING:

		if ((GetPlayerInput(INPUT_RIGHT))
			&& otherPlayer->IsAttacking() && !looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_LEFT))
			&& otherPlayer->IsAttacking() && looking_right)
			playerState = PLAYER_CROUCH_BLOCKING;
		else if ((GetPlayerInput(INPUT_L_PUNCH)) ||
			(GetPlayerInput(INPUT_M_PUNCH)) ||
			(GetPlayerInput(INPUT_H_PUNCH)))
			playerState = PLAYER_CROUCH_PUNCH;

		else if ((GetPlayerInput(INPUT_L_KICK)) ||
			(GetPlayerInput(INPUT_M_KICK)) ||
			(GetPlayerInput(INPUT_H_KICK)))
			playerState = PLAYER_CROUCH_KICK;

		else if (!(GetPlayerInput(INPUT_DOWN)))
		{
			playerState = PLAYER_IDLE;
			crouching.RestartFrames();
		}
		break;

	case PLAYER_LOW_PUNCH:

		if (L_punch.IsEnded())
		{
			L_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_LOW_KICK:

		if (L_kick.IsEnded())
		{
			L_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_PUNCH:

		if (M_punch.IsEnded())
		{
			M_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_MEDIUM_KICK:

		if (M_kick.IsEnded())
		{
			M_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_PUNCH:

		if (H_punch.IsEnded())
		{
			H_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIGH_KICK:

		if (H_kick.IsEnded())
		{
			H_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_CROUCH_PUNCH:

		if (crouch_punch.IsEnded())
		{
			crouch_punch.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_CROUCH_KICK:

		if (crouch_kick.IsEnded())
		{
			crouch_kick.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		break;

	case PLAYER_FORWARD_LOW_PUNCH:

		if (F_L_punch.IsEnded())
		{
			F_L_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_LOW_KICK:

		if (F_L_kick.IsEnded())
		{
			F_L_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:

		if (F_M_punch.IsEnded())
		{
			F_M_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:

		if (F_M_kick.IsEnded())
		{
			F_M_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:

		if (F_H_punch.IsEnded())
		{
			F_H_punch.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_FORWARD_HIGH_KICK:

		if (F_H_kick.IsEnded())
		{
			F_H_kick.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_HIT:

		if (hit.IsEnded())
		{
			hit.RestartFrames();
			if (distance_jumped > 0){
				going_up = false;
				playerState = PLAYER_JUMPING;
			}
			else
				playerState = PLAYER_IDLE;
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_CROUCH_HIT:

		if (crouch_hit.IsEnded())
		{
			crouch_hit.RestartFrames();
			playerState = PLAYER_CROUCHING;
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_FACE_HIT:

		if (face_hit.IsEnded())
		{
			face_hit.RestartFrames();
			playerState = PLAYER_IDLE;
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
		}
		break;

	case PLAYER_YOGA_FIRE:
		if (yoga_fire.GetCurrentFrameNumber() == 3 && SDL_GetTicks() - lastShotTimer > 500)
		{
			if (looking_right)
			{
				iPoint particlePosition(position.x + 42 + 14, position.y - 54 + 10);
				App->particles->newParticle(particlePosition, graphics, particula, destroy_particula, 2);
			}
			else
			{
				iPoint particlePosition(position.x - 42 - 14, position.y - 54 + 10);
				App->particles->newParticle(particlePosition, graphics, particula, destroy_particula, -2);
			}
			lastShotTimer = SDL_GetTicks();
		}

		if (yoga_fire.IsEnded())
		{
			yoga_fire.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		break;

	case PLAYER_YOGA_FLAME:

		if (yoga_flame.IsEnded())
		{
			yoga_flame.RestartFrames();
			playerState = PLAYER_IDLE;

		}
		break;

	case PLAYER_YOGA_MUMMY:

		going_up = false;
		if (distance_jumped == 0)
		{
			already_hitted = false;
			yoga_mummy.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		else if (yoga_mummy.IsEnded() && distance_jumped > 0)
		{
			yoga_mummy.RestartFrames();
			playerState = PLAYER_JUMPING;
			jump_attacked = true;
		}
		else if (yoga_mummy.IsEnded())
		{
			yoga_mummy.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		break;

	case PLAYER_YOGA_SPEAR:

		going_up = false;
		if (distance_jumped == 0)
		{
			already_hitted = false;
			yoga_spear.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		else if (yoga_spear.IsEnded() && distance_jumped > 0)
		{
			yoga_spear.RestartFrames();
			playerState = PLAYER_JUMPING;
			jump_attacked = true;
		}
		else if (yoga_spear.IsEnded())
		{
			yoga_spear.RestartFrames();
			playerState = PLAYER_IDLE;
			jump_attacked = false;
		}
		break;

	case PLAYER_BLOCKING:
		if (!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT))))
		{
			block.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		else if (otherPlayer->IsAttacking() &&
			(GetPlayerInput(INPUT_DOWN)))
		{
			block.RestartFrames();
			playerState = PLAYER_CROUCH_BLOCKING;
		}
		else if (block.IsEnded())
			block.RestartFrames();
		break;

	case PLAYER_CROUCH_BLOCKING:
		if ((!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT))))
			&& (GetPlayerInput(INPUT_DOWN)))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCHING;
		}
		else if (!(GetPlayerInput(INPUT_DOWN)) && (!otherPlayer->IsAttacking() ||
			(looking_right && !(GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && !(GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_IDLE;
		}
		else if (otherPlayer->IsAttacking() && !(GetPlayerInput(INPUT_DOWN)) &&
			((looking_right && (GetPlayerInput(INPUT_LEFT))) ||
			(!looking_right && (GetPlayerInput(INPUT_RIGHT)))))
		{
			crouch_block.RestartFrames();
			playerState = PLAYER_BLOCKING;
		}
		else if (crouch_block.IsEnded())
			crouch_block.RestartFrames();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
			block.RestartFrames();
			playerState = PLAYER_BLOCKING;
		}
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (crouch_block.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			already_hitted = false;
			crouch_block.RestartFrames();
			playerState = PLAYER_CROUCH_BLOCKING;
		}
		break;

	case PLAYER_AIR_HITTED:
		if (air_hit.IsEnded())
		{
			hitted = false;
			head_hitted = false;
			leg_hitted = false;
			air_hit.RestartFrames();
			going_up = false;
			playerState = PLAYER_JUMPING;
		}
		break;
	}

	starting_combo = COMBO_NOTHING;

	return UPDATE_CONTINUE;
}

// Update
update_status ModulePlayerDhalsim::Update()
{
	SDL_Rect aux;
	int pivot;
	Collider_player_structure cps;

	if (otherPlayer->playerInCameraLimit() &&
		((App->renderer->ScreenLeftLimit() && otherPlayer->GetLooking_right()) ||
		(App->renderer->ScreenRightLimit() && !otherPlayer->GetLooking_right())) &&
		((otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED) ||
		(otherPlayer->GetPlayerState() == PLAYER_CROUCH_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_FACE_HIT) ||
		(otherPlayer->GetPlayerState() == PLAYER_BLOCKING_HITTED) ||
		(otherPlayer->GetPlayerState() == PLAYER_CROUCH_BLOCKING_HITTED)) &&
		(playerState != PLAYER_JUMP_KICK) && (playerState != PLAYER_JUMP_PUNCH) && 
		(playerState != PLAYER_JUMPING) && (playerState != PLAYER_AIR_HITTED))
	{
		if (looking_right && (otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED))
			MovePlayer(-3);
		else if (looking_right)
			MovePlayer(-1);
		else if (otherPlayer->GetPlayerState() == PLAYER_AIR_HITTED)
			MovePlayer(3);
		else
			MovePlayer(1);
	}

	switch (playerState)
	{
	case PLAYER_IDLE:
		aux = idle.GetCurrentFrame();
		pivot = idle.GetCurrentPivot();
		cps = idle.GetCurrentCollider();
		idle.NextFrame();
		break;

	case PLAYER_WALKING_FORWARD:
		if (!colliding_players)
		{
			if (looking_right)
				MovePlayer(2);
			else
				MovePlayer(-2);
		}
		aux = forward.GetCurrentFrame();
		pivot = forward.GetCurrentPivot();
		cps = forward.GetCurrentCollider();
		forward.NextFrame();
		break;

	case PLAYER_WALKING_BACKWARD:
		if (looking_right)
			MovePlayer(-1);
		else
			MovePlayer(1);
		aux = backward.GetCurrentFrame();
		pivot = backward.GetCurrentPivot();
		cps = backward.GetCurrentCollider();
		backward.NextFrame();
		break;

	case PLAYER_CROUCHING:
		aux = crouching.GetCurrentFrame();
		pivot = crouching.GetCurrentPivot();
		cps = crouching.GetCurrentCollider();
		crouching.NextFrame();
		break;

	case PLAYER_JUMPING:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		if (distance_jumped > 90)
		{
			aux = jump.frames[2].frame;
			pivot = jump.frames[2].pivot;
			cps = jump.frames[2].colliders;
		}
		else
		{
			aux = jump.frames[1].frame;
			pivot = jump.frames[1].pivot;
			cps = jump.frames[1].colliders;
		}

		break;


	case PLAYER_LOW_PUNCH:
		aux = L_punch.GetCurrentFrame();
		pivot = L_punch.GetCurrentPivot();
		cps = L_punch.GetCurrentCollider();
		L_punch.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 12;
		break;

	case PLAYER_LOW_KICK:
		aux = L_kick.GetCurrentFrame();
		pivot = L_kick.GetCurrentPivot();
		cps = L_kick.GetCurrentCollider();
		L_kick.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 12;
		break;

	case PLAYER_MEDIUM_PUNCH:
		aux = M_punch.GetCurrentFrame();
		pivot = M_punch.GetCurrentPivot();
		cps = M_punch.GetCurrentCollider();
		M_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_MEDIUM_KICK:
		aux = M_kick.GetCurrentFrame();
		pivot = M_kick.GetCurrentPivot();
		cps = M_kick.GetCurrentCollider();
		M_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_HIGH_PUNCH:
		aux = H_punch.GetCurrentFrame();
		pivot = H_punch.GetCurrentPivot();
		cps = H_punch.GetCurrentCollider();
		H_punch.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 20;
		break;

	case PLAYER_HIGH_KICK:
		aux = H_kick.GetCurrentFrame();
		pivot = H_kick.GetCurrentPivot();
		cps = H_kick.GetCurrentCollider();
		H_kick.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 20;
		break;

	case PLAYER_FORWARD_LOW_PUNCH:
		aux = F_L_punch.GetCurrentFrame();
		pivot = F_L_punch.GetCurrentPivot();
		cps = F_L_punch.GetCurrentCollider();
		F_L_punch.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 14;
		break;

	case PLAYER_FORWARD_LOW_KICK:
		aux = F_L_kick.GetCurrentFrame();
		pivot = F_L_kick.GetCurrentPivot();
		cps = F_L_kick.GetCurrentCollider();
		F_L_kick.NextFrame();
		collider_attack.damageType = L_ATTACK;
		collider_attack.damage = 14;
		break;

	case PLAYER_FORWARD_MEDIUM_PUNCH:
		aux = F_M_punch.GetCurrentFrame();
		pivot = F_M_punch.GetCurrentPivot();
		cps = F_M_punch.GetCurrentCollider();
		F_M_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_FORWARD_MEDIUM_KICK:
		aux = F_M_kick.GetCurrentFrame();
		pivot = F_M_kick.GetCurrentPivot();
		cps = F_M_kick.GetCurrentCollider();
		F_M_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_FORWARD_HIGH_PUNCH:
		aux = F_H_punch.GetCurrentFrame();
		pivot = F_H_punch.GetCurrentPivot();
		cps = F_H_punch.GetCurrentCollider();
		F_H_punch.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 22;
		break;

	case PLAYER_FORWARD_HIGH_KICK:
		aux = F_H_kick.GetCurrentFrame();
		pivot = F_H_kick.GetCurrentPivot();
		cps = F_H_kick.GetCurrentCollider();
		F_H_kick.NextFrame();
		collider_attack.damageType = H_ATTACK;
		collider_attack.damage = 22;
		break;

	case PLAYER_CROUCH_PUNCH:
		aux = crouch_punch.GetCurrentFrame();
		pivot = crouch_punch.GetCurrentPivot();
		cps = crouch_punch.GetCurrentCollider();
		crouch_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_CROUCH_KICK:
		if (looking_right)
		{
			MovePlayer(2);
		}
		else
		{
			MovePlayer(-2);
		}
		aux = crouch_kick.GetCurrentFrame();
		pivot = crouch_kick.GetCurrentPivot();
		cps = crouch_kick.GetCurrentCollider();
		crouch_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 16;
		break;

	case PLAYER_JUMP_PUNCH:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_punch.GetCurrentFrame();
		pivot = jump_punch.GetCurrentPivot();
		cps = jump_punch.GetCurrentCollider();
		jump_punch.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;


	case PLAYER_JUMP_KICK:
		if (going_up && distance_jumped < 105)
		{
			distance_jumped += 3;
		}
		else if (going_up)
			going_up = false;
		else
		{
			distance_jumped -= 3;
		}

		if (directionJump == JUMP_RIGHT)
		{
			MovePlayer(1);
		}
		else if (directionJump == JUMP_LEFT)
		{
			MovePlayer(-1);
		}

		aux = jump_kick.GetCurrentFrame();
		pivot = jump_kick.GetCurrentPivot();
		cps = jump_kick.GetCurrentCollider();
		jump_kick.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = hit.GetCurrentFrame();
		pivot = hit.GetCurrentPivot();
		cps = hit.GetCurrentCollider();
		hit.NextFrame();
		break;

	case PLAYER_CROUCH_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = crouch_hit.GetCurrentFrame();
		pivot = crouch_hit.GetCurrentPivot();
		cps = crouch_hit.GetCurrentCollider();
		crouch_hit.NextFrame();
		break;

	case PLAYER_FACE_HIT:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = face_hit.GetCurrentFrame();
		pivot = face_hit.GetCurrentPivot();
		cps = face_hit.GetCurrentCollider();
		face_hit.NextFrame();
		break;

	case PLAYER_KO:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		if (!ko.IsEnded()) {
			if (looking_right)
				MovePlayer(-1);
			else
				MovePlayer(1);
		}
		aux = ko.GetCurrentFrame();
		pivot = ko.GetCurrentPivot();
		cps = ko.GetCurrentCollider();
		ko.NextFrame();

		break;

	case PLAYER_WIN_1:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = victory1.GetCurrentFrame();
		pivot = victory1.GetCurrentPivot();
		cps = victory1.GetCurrentCollider();
		victory1.NextFrame();
		break;

	case PLAYER_WIN_2:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = victory2.GetCurrentFrame();
		pivot = victory2.GetCurrentPivot();
		cps = victory2.GetCurrentCollider();
		victory2.NextFrame();
		break;

	case PLAYER_TIME_OUT:
		if (distance_jumped > 0)
		{
			distance_jumped -= 3;
		}
		aux = time_out.GetCurrentFrame();
		pivot = time_out.GetCurrentPivot();
		cps = time_out.GetCurrentCollider();
		time_out.NextFrame();
		break;

	case PLAYER_YOGA_FIRE:
		aux = yoga_fire.GetCurrentFrame();
		pivot = yoga_fire.GetCurrentPivot();
		cps = yoga_fire.GetCurrentCollider();
		yoga_fire.NextFrame();
		break;

	case PLAYER_YOGA_FLAME:
		aux = yoga_flame.GetCurrentFrame();
		pivot = yoga_flame.GetCurrentPivot();
		cps = yoga_flame.GetCurrentCollider();
		yoga_flame.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 28;
		break;

	case PLAYER_YOGA_MUMMY:

		distance_jumped -= 3;
		if (directionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (directionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_mummy.GetCurrentFrame();
		pivot = yoga_mummy.GetCurrentPivot();
		cps = yoga_mummy.GetCurrentCollider();
		yoga_mummy.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_YOGA_SPEAR:

		distance_jumped -= 3;
		if (directionMummy == JUMP_RIGHT)
		{
			MovePlayer(2);
		}
		else if (directionMummy == JUMP_LEFT)
		{
			MovePlayer(-2);
		}

		aux = yoga_spear.GetCurrentFrame();
		pivot = yoga_spear.GetCurrentPivot();
		cps = yoga_spear.GetCurrentCollider();
		yoga_spear.NextFrame();
		collider_attack.damageType = M_ATTACK;
		collider_attack.damage = 18;
		break;

	case PLAYER_BLOCKING:
		aux = block.GetCurrentFrame();
		pivot = block.GetCurrentPivot();
		cps = block.GetCurrentCollider();
		block.NextFrame();
		break;

	case PLAYER_CROUCH_BLOCKING:
		aux = crouch_block.GetCurrentFrame();
		pivot = crouch_block.GetCurrentPivot();
		cps = crouch_block.GetCurrentCollider();
		crouch_block.NextFrame();
		break;

	case PLAYER_BLOCKING_HITTED:
		if (!looking_right)
		{
			MovePlayer(2);
		}
		else
		{
			MovePlayer(-2);
		}
		aux = block.GetCurrentFrame();
		pivot = block.GetCurrentPivot();
		cps = block.GetCurrentCollider();
		block.NextFrame();
		break;

	case PLAYER_CROUCH_BLOCKING_HITTED:
		if (!looking_right)
		{
			MovePlayer(1);
		}
		else
		{
			MovePlayer(-1);
		}
		aux = crouch_block.GetCurrentFrame();
		pivot = crouch_block.GetCurrentPivot();
		cps = crouch_block.GetCurrentCollider();
		crouch_block.NextFrame();
		break;

	case PLAYER_AIR_HITTED:
		if (!looking_right)
		{
			MovePlayer(2);
			directionJump = JUMP_RIGHT;
		}
		else
		{
			MovePlayer(-2);
			directionJump = JUMP_LEFT;
		}
		distance_jumped += 3;
		going_up = true;
		aux = air_hit.GetCurrentFrame();
		pivot = air_hit.GetCurrentPivot();
		cps = air_hit.GetCurrentCollider();
		air_hit.NextFrame();
		break;

	}

	player_collider.rect = cps.Position_collider;
	collider_head.rect = cps.Collider_head;
	collider_body.rect = cps.Collider_body;
	collider_legs.rect = cps.Collider_legs;
	collider_attack.rect = cps.Collider_attack;

	if (looking_right)
	{
		player_collider.rect.x += position.x;
		player_collider.rect.y += position.y - distance_jumped;
		collider_head.rect.x += position.x;
		collider_head.rect.y += position.y - distance_jumped;
		collider_body.rect.x += position.x;
		collider_body.rect.y += position.y - distance_jumped;
		collider_legs.rect.x += position.x;
		collider_legs.rect.y += position.y - distance_jumped;
		collider_attack.rect.x += position.x;
		collider_attack.rect.y += position.y - distance_jumped;
		App->renderer->Blit(graphics, position.x - pivot, position.y - aux.h - distance_jumped, &(aux), 1.0f, SDL_FLIP_NONE);
	}
	else
	{
		player_collider.rect.x = position.x - player_collider.rect.x - player_collider.rect.w;
		player_collider.rect.y += position.y - distance_jumped;
		collider_head.rect.x = position.x - collider_head.rect.x - collider_head.rect.w;
		collider_head.rect.y += position.y - distance_jumped;
		collider_body.rect.x = position.x - collider_body.rect.x - collider_body.rect.w;
		collider_body.rect.y += position.y - distance_jumped;
		collider_legs.rect.x = position.x - collider_legs.rect.x - collider_legs.rect.w;
		collider_legs.rect.y += position.y - distance_jumped;
		collider_attack.rect.x = position.x - collider_attack.rect.x - collider_attack.rect.w;
		collider_attack.rect.y += position.y - distance_jumped;
		App->renderer->Blit(graphics, position.x - aux.w + pivot, position.y - aux.h - distance_jumped, &(aux), 1.0f, SDL_FLIP_HORIZONTAL);
	}
	
	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModulePlayerDhalsim::PostUpdate()
{
	colliding_players = false;
	return UPDATE_CONTINUE;
}

// On Collision
void ModulePlayerDhalsim::OnCollision(Collider* c1, Collider* c2)
{
	if (!win && !dead && !time_0){
		if (c1->type == COLLIDER_BODY_PLAYER_ONE && c2->type == COLLIDER_BODY_PLAYER_TWO)
			colliding_players = true;
		else if (c1->type == COLLIDER_BODY_PLAYER_TWO && c2->type == COLLIDER_BODY_PLAYER_ONE)
			colliding_players = true;
		else if (c1->type == COLLIDER_PLAYER_ONE && c2->type == COLLIDER_ATTACK_PLAYER_TWO && c2->rect.w * c2->rect.h != 0 && !already_hitted)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
			}
			else if ((otherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || otherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
				leg_hitted = true;
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}
			else if (!hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(audio_id_L_impact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(audio_id_M_impact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(audio_id_H_impact);
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if (c1->type == COLLIDER_PLAYER_TWO && c2->type == COLLIDER_ATTACK_PLAYER_ONE  && c2->rect.w * c2->rect.h != 0 && !already_hitted)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
			}
			else if ((otherPlayer->GetPlayerState() == PLAYER_CROUCH_PUNCH || otherPlayer->GetPlayerState() == PLAYER_CROUCH_KICK) && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= c2->damage;
				already_hitted = true;
				leg_hitted = true;
			}


			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}
			else if (!hitted)
			{
				if (c2->damageType == L_ATTACK)
					App->audio->PlayFx(audio_id_L_impact);
				else if (c2->damageType == M_ATTACK)
					App->audio->PlayFx(audio_id_M_impact);
				else if (c2->damageType == H_ATTACK)
					App->audio->PlayFx(audio_id_H_impact);
			}


			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_ONE || c1->type == COLLIDER_ATTACK_PLAYER_ONE) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= 18;
				App->audio->PlayFx(audio_id_L_impact);
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
		else if ((c1->type == COLLIDER_PLAYER_TWO || c1->type == COLLIDER_ATTACK_PLAYER_TWO) && c2->type == COLLIDER_PARTICLES && c2->rect.w * c2->rect.h != 0)
		{
			if (!hitted && playerState != PLAYER_BLOCKING && playerState != PLAYER_CROUCH_BLOCKING && playerState != PLAYER_BLOCKING_HITTED && playerState != PLAYER_CROUCH_BLOCKING_HITTED)
			{
				life -= 18;
				App->audio->PlayFx(audio_id_L_impact);
			}

			if (life < 0)
			{
				App->scene_bison->RestartScene(otherPlayer->wins + 1);
				App->audio->PlayFx(audio_id_dead);
				dead = true;
				life = 0;
			}

			hitted = true;
			if (c1 == &collider_head)
				head_hitted = true;
		}
	}
}

// Returns true if the player is the limit of the camera
bool ModulePlayerDhalsim::playerInCameraLimit() const
{
	if (App->renderer->camera.x <= -((player_collider.rect.x - player_collider.rect.w)*SCREEN_SIZE))
		return true;
	if (App->renderer->camera.x - App->renderer->camera.w >= -((player_collider.rect.x + player_collider.rect.w * 2)*SCREEN_SIZE))
		return true;
	return false;
}

// Moves horizontaly the player, it can move the camera
void ModulePlayerDhalsim::MovePlayer(int distance)
{
	if (distance > 0)
	{
		if (!App->renderer->ScreenRightLimit())
		{
			if (!otherPlayer->playerInCameraLimit())
			{
				position.x += distance;
				if (App->renderer->camera.x - App->renderer->camera.w >= -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
					App->renderer->camera.x -= SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x - App->renderer->camera.w < -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
					position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x - App->renderer->camera.w < -((player_collider.rect.x + player_collider.rect.w*2)*SCREEN_SIZE))
				position.x += distance;
		}
	}
	else
	{
		if (!App->renderer->ScreenLeftLimit())
		{
			if (!otherPlayer->playerInCameraLimit())
			{
				position.x += distance;
				if (App->renderer->camera.x <= -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
					App->renderer->camera.x += SCREEN_SIZE;
			}
			else
			{
				if (App->renderer->camera.x > -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
					position.x += distance;
			}
		}
		else
		{
			if (App->renderer->camera.x > -(player_collider.rect.x*SCREEN_SIZE) + (player_collider.rect.w*SCREEN_SIZE))
				position.x += distance;
		}
	}
}

// Resets the player states
void ModulePlayerDhalsim::restartPlayer(bool everything)
{

	if (numPlayer == 1)
	{
		position.x = 150;
		position.y = 200;
	}
	else
	{
		position.x = 300;
		position.y = 200;
	}

	if (everything)
		wins = 0;

	playerState = PLAYER_IDLE;
	colliding_players = false;
	jumping = false;
	hitted = false;
	head_hitted = false;
	life = 200;
	win = false;
	dead = false;
	time_0 = false;
	starting_combo = COMBO_NOTHING;

	ko.RestartFrames();
	victory1.RestartFrames();
	victory2.RestartFrames();

	distance_jumped = 0;
	jump_attacked = false;
	leg_hitted = false;
	already_hitted = false;
}

// Get and Set variables

iPoint ModulePlayerDhalsim::getPosition() const
{
	return position;
}

bool ModulePlayerDhalsim::IsAttacking() const
{
	if (playerState == PLAYER_LOW_PUNCH ||
		playerState == PLAYER_LOW_KICK ||
		playerState == PLAYER_MEDIUM_PUNCH ||
		playerState == PLAYER_MEDIUM_KICK ||
		playerState == PLAYER_HIGH_PUNCH ||
		playerState == PLAYER_HIGH_KICK ||
		playerState == PLAYER_CROUCH_PUNCH ||
		playerState == PLAYER_CROUCH_KICK ||
		playerState == PLAYER_JUMP_PUNCH ||
		playerState == PLAYER_JUMP_KICK ||
		playerState == PLAYER_FORWARD_LOW_PUNCH ||
		playerState == PLAYER_FORWARD_LOW_KICK ||
		playerState == PLAYER_FORWARD_MEDIUM_PUNCH ||
		playerState == PLAYER_FORWARD_MEDIUM_KICK ||
		playerState == PLAYER_FORWARD_HIGH_PUNCH ||
		playerState == PLAYER_FORWARD_HIGH_KICK ||
		playerState == PLAYER_YOGA_FIRE ||
		playerState == PLAYER_YOGA_FLAME ||
		playerState == PLAYER_YOGA_MUMMY ||
		playerState == PLAYER_YOGA_SPEAR)
	{
		return true;
	}
	else if (App->particles->GetNumberParticles() > 0)
	{
		if (otherPlayer->looking_right)
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x > otherPlayer->position.x && App->particles->GetParticleSpeed(i) < 0)
					return true;
			}
		}
		else
		{
			for (int i = 0; i < App->particles->GetNumberParticles(); ++i)
			{
				if (App->particles->GetParticlePosition(i).x < otherPlayer->position.x && App->particles->GetParticleSpeed(i) > 0)
					return true;
			}
		}
	}
	return false;
}

int ModulePlayerDhalsim::GetWins() const
{
	return wins;
}

void ModulePlayerDhalsim::SetWins(int wins)
{
	this->wins = wins;
}

int ModulePlayerDhalsim::GetLife() const
{
	return life;
}

void ModulePlayerDhalsim::SetLife(int life)
{
	this->life = life;
}

void ModulePlayerDhalsim::DecreseLife(int life)
{
	this->life -= life;
}

bool ModulePlayerDhalsim::GetWin() const
{
	return win;
}

void ModulePlayerDhalsim::SetWin(bool win)
{
	this->win = win;
}

bool ModulePlayerDhalsim::GetDead() const
{
	return dead;
}

void ModulePlayerDhalsim::SetDead(bool dead)
{
	this->dead = dead;
}

bool ModulePlayerDhalsim::GetTime_0() const
{
	return time_0;
}

void ModulePlayerDhalsim::SetTime_0(bool time_0)
{
	this->time_0 = time_0;
}

bool ModulePlayerDhalsim::GetJumping() const
{
	return jumping;
}

void ModulePlayerDhalsim::SetJumping(bool jumping)
{
	this->jumping = jumping;
}

bool ModulePlayerDhalsim::GetLooking_right() const
{
	return looking_right;
}

void ModulePlayerDhalsim::SetLooking_right(bool looking_right)
{
	this->looking_right = looking_right;
}

player_state ModulePlayerDhalsim::GetPlayerState() const
{
	return playerState;
}

void ModulePlayerDhalsim::SetPlayerState(player_state playerState)
{
	this->playerState = playerState;
}

combo_types ModulePlayerDhalsim::GetStartingCombo() const
{
	return starting_combo;
}

void ModulePlayerDhalsim::SetStartingCombo(combo_types starting_combo)
{
	this->starting_combo = starting_combo;
}

int ModulePlayerDhalsim::GetDistanceJumped() const
{
	return distance_jumped;
}

bool ModulePlayerDhalsim::GetPlayerInput(input_type actionKey)
{
	if (numPlayer == 1)
	{
		switch (actionKey)
		{
		case INPUT_UP:
			return App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT;
			break;
		case INPUT_DOWN:
			return App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT;
			break;
		case INPUT_LEFT:
			return App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT;
			break;
		case INPUT_RIGHT:
			return App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT;
			break;
		case INPUT_L_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
			break;
		case INPUT_L_KICK:
			return App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN;
			break;
		case INPUT_M_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
			break;
		case INPUT_M_KICK:
			return App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
			break;
		case INPUT_H_PUNCH:
			return App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN;
			break;
		case INPUT_H_KICK:
			return App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN;
			break;
		}
	}
	else
	{
		switch (actionKey)
		{
		case INPUT_UP:
			return (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTX) == KEY_REPEAT || App->input->yDir == -1 || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT);
			break;
		case INPUT_DOWN:
			return (App->input->GetButton(SDL_CONTROLLER_AXIS_LEFTY) == KEY_REPEAT || App->input->yDir == 1 || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT);
			break;
		case INPUT_LEFT:
			return (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTX) == KEY_REPEAT || App->input->xDir == -1 || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT);
			break;
		case INPUT_RIGHT:
			return (App->input->GetButton(SDL_CONTROLLER_AXIS_RIGHTY) == KEY_REPEAT || App->input->xDir == 1 || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT);
			break;
		case INPUT_L_PUNCH:
			return (App->input->GetButton(12) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_DOWN);
			break;
		case INPUT_L_KICK:
			return (App->input->GetButton(10) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_7) == KEY_DOWN);
			break;
		case INPUT_M_PUNCH:
			return (App->input->GetButton(11) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_DOWN);
			break;
		case INPUT_M_KICK:
			return (App->input->GetButton(13) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_DOWN);
			break;
		case INPUT_H_PUNCH:
			return (App->input->GetButton(8) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_DOWN);
			break;
		case INPUT_H_KICK:
			return (App->input->GetButton(9) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_KP_9) == KEY_DOWN);
			break;
		}
	}
	return false;
}