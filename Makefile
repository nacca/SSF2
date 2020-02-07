compile:
	g++ *.cpp -o SSF2 -ljsoncpp `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_mixer `pkg-config --cflags --libs sdl2`

dep:
	sudo apt install g++ libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libjsoncpp-dev indent

run:
	./SSF2

format:

	find . -name '*.cpp' -exec indent {} \;

clean:
	rm -f *.o SSF2 *~