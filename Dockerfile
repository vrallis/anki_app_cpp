
FROM ubuntu:22.04

WORKDIR /usr/src/app

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libsdl2-dev \
    libsdl2-mixer-dev \
    libsqlite3-dev \
    libexpat1-dev \
    figlet \
    && rm -rf /var/lib/apt/lists/*

COPY . .

RUN rm -rf build && mkdir build

WORKDIR /usr/src/app/build
RUN cmake -DENABLE_TESTS=OFF .. && make

COPY ../config.json .

ENV SDL_AUDIODRIVER=dummy

CMD ["./anki_app_cpp"]