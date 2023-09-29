INC := -Iinclude
INC += `pkg-config --cflags sdl2 sdl2_image sdl2_mixer`
INC += `pkg-config --cflags jsoncpp`

LIB := `pkg-config --libs sdl2 sdl2_image sdl2_mixer`
LIB += `pkg-config --libs jsoncpp`

CXX = g++
CXXFLAGS = -std=c++17 -Wswitch

all:
	${CXX} ${CXXFLAGS} src/*.cpp $(INC) $(LIB) -o SSF2   

obj:
	${CXX} ${CXXFLAGS} -c src/*.cpp $(INC) $(LIB)

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
