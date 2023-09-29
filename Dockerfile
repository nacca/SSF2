FROM debian
RUN apt update -y && apt install -y g++ make cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libjsoncpp-dev indent