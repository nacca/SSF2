compile:
	g++ Main.cpp -o Main -lSDL2 -lSDL2_image -lSDL2_mixer `pkg-config --cflags --libs sdl2`
