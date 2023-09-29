DOCKER_IMAGE=cpp-compiler

OS=$(shell uname -s)
ifeq ($(OS),Darwin)        # Mac OS X
    SED = gsed
endif
UID=$(shell id -u)
GID=$(shell id -g)


.PHONY: build


INC := -Iinclude
INC += `pkg-config --cflags sdl2 sdl2_image sdl2_mixer`
INC += `pkg-config --cflags jsoncpp`

LIB := `pkg-config --libs sdl2 sdl2_image sdl2_mixer`
LIB += `pkg-config --libs jsoncpp`

CXX = g++
CXXFLAGS = -std=c++17 -Wswitch

compile-native:
	${CXX} ${CXXFLAGS} src/*.cpp $(INC) $(LIB) -o SSF2   

obj:
	${CXX} ${CXXFLAGS} -c src/*.cpp $(INC) $(LIB)

dep:	dep-debian

dep-debian:
	sudo apt install g++ make cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libjsoncpp-dev indent

dep-arch:
	sudo yay -S g++ make cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libjsoncpp-dev indent --noconfirm sdl2_mixer --noconfirm

dep-mac:
	brew install make cmake sdl2 sdl2_image sdl2_mixer jsoncpp

run:
	./SSF2

format:

	find src/ -name '*.cpp' -exec indent {} \;
	find include/ -name '*.h' -exec indent {} \;

clean:
	rm -rf **/*.o **/*~ SSF2 SSF2.app

build:
	rm -rf build && mkdir -p build && cd build && cmake .. && make

build-docker:
	docker build . -t ${DOCKER_IMAGE}

compile-linux:	build-docker
	docker run -it --rm --user ${UID}:${GID} -v "${PWD}":/workdir -w /workdir ${DOCKER_IMAGE} make build

shell:
	docker run -it --rm --user ${UID}:${GID} -v "${PWD}":/workdir -w /workdir --entrypoint=/bin/bash ${DOCKER_IMAGE}
