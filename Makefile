INC := `sdl2-config --cflags`
INC += -Iinclude

LIB := `sdl2-config --libs`
LIB += -lSDL2_image -lSDL2_mixer
LIB += -ljsoncpp

compile:
	g++ include/*.h src/*.cpp $(INC) $(LIB) -o SSF2   

dep:
	sudo apt install g++ make cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libjsoncpp-dev indent

dep-mac:
	brew install make cmake sdl2 sdl2_image sdl2_mixer jsoncpp

run:
	./SSF2

format:

	find src/ -name '*.cpp' -exec indent {} \;
	find include/ -name '*.h' -exec indent {} \;

clean:
	rm -rf **/*.o **/*~ SSF2 SSF2.app

.PHONY: build
build:
	rm -rf build && mkdir -p build && cd build && cmake .. && make