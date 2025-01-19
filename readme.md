# Anki App

## Prerequisites

### English

- Docker (for Docker setup)
- Git
- CMake
- GCC (for compiling yourself)
- SDL2
- SDL2_mixer
- SQLite3
- Expat
- Figlet

### Ελληνικά

- Docker (για τη ρύθμιση Docker)
- Git
- CMake
- GCC (για τοπική μεταγλώττιση)
- SDL2
- SDL2_mixer
- SQLite3
- Expat
- Figlet

## Installation Instructions (Linux/WSL Only)

### English 🇬🇧

#### Using Docker

1. **Clone the repository:**
    ```bash
    git clone https://github.com/vrallis/anki_app_cpp
    cd anki_app_cpp
    ```

2. **Build the Docker image:**
    ```bash
    docker build -t anki_app_cpp .
    ```

3. **Run the Docker container:**
    ```bash
    docker run -it anki_app_cpp
    ```

#### Compiling Locally

1. **Clone the repository:**
    ```bash
    git clone https://github.com/vrallis/anki_app_cpp
    cd anki_app_cpp
    ```

2. **Install dependencies:**
    ```bash
    sudo apt-get update
    sudo apt-get install -y build-essential cmake git libsdl2-dev libsdl2-mixer-dev libsqlite3-dev libexpat1-dev figlet
    ```

3. **Build the application:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Run the application:**
    ```bash
    ./anki_app_cpp
    ```

### Ελληνικά 🇬🇷

#### Χρήση Docker

1. **Κλωνοποιήστε το repo:**
    ```bash
    git clone https://github.com/vrallis/anki_app_cpp
    cd anki_app_cpp
    ```

2. **Δημιουργήστε την εικόνα Docker:**
    ```bash
    docker build -t anki_app_cpp .
    ```

3. **Εκτελέστε το κοντέινερ Docker:**
    ```bash
    docker run -it anki_app_cpp
    ```

#### Τοπική Μεταγλώττιση

1. **Κλωνοποιήστε το repo:**
    ```bash
    git clone https://github.com/vrallis/anki_app_cpp
    cd anki_app_cpp
    ```

2. **Εγκαταστήστε τα libraries:**
    ```bash
    sudo apt-get update
    sudo apt-get install -y build-essential cmake git libsdl2-dev libsdl2-mixer-dev libsqlite3-dev libexpat1-dev figlet
    ```

3. **Compile την εφαρμογή:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. **Εκτελέστε την εφαρμογή:**
    ```bash
    ./anki_app_cpp
    ```