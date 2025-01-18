# Use an official Ubuntu as a parent image
FROM ubuntu:22.04

# Set the working directory
WORKDIR /usr/src/app

# Install dependencies
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

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Remove any existing build directory and create a new one
RUN rm -rf build && mkdir build

# Create build directory and build the application without tests
WORKDIR /usr/src/app/build
RUN cmake -DENABLE_TESTS=OFF .. && make

# Copy the config file to the build directory
COPY ../config.json .

# Set SDL to use a dummy audio driver
ENV SDL_AUDIODRIVER=dummy

# Run the application
CMD ["./anki_app_cpp"]