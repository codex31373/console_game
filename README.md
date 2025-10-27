# Console Game

A 2D physics-based console game built with C++, SDL2, and Box2D.

## Features

- **SDL2 Graphics**: 2D rendering with SDL2
- **Box2D Physics**: Realistic physics simulation
- **Dynamic Objects**: Interactive physics objects that respond to gravity and collisions
- **Real-time Physics**: Fixed timestep physics simulation for stable gameplay

## Requirements

- C++17 compatible compiler
- SDL2 development libraries
- SDL2_image development libraries
- Box2D development libraries
- CMake 3.16 or higher

### Ubuntu/Debian Dependencies

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libsdl2-dev libsdl2-image-dev
sudo apt install libbox2d-dev
```

## Building

### Option 1: Using the build script (recommended)
```bash
./build.sh
```

### Option 2: Manual build
```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./bin/ConsoleGame
```

## Controls

- **ESC**: Exit the game
- **SPACE**: Add a new random physics object
- **Window Close**: Exit the game

## Game Features

- **Ground Platform**: Static green platform at the bottom
- **Falling Boxes**: Dynamic physics objects that fall and bounce
- **Realistic Physics**: Gravity, friction, and collision detection
- **Colorful Objects**: Each new object has a random color

## Project Structure

```
console_game/
├── CMakeLists.txt      # Build configuration
├── include/            # Header files
│   ├── Game.hpp
│   ├── PhysicsWorld.hpp
│   └── GameObject.hpp
├── src/               # Source files
│   ├── main.cpp
│   ├── Game.cpp
│   ├── PhysicsWorld.cpp
│   └── GameObject.cpp
└── README.md          # This file
```

## Architecture

The game uses a modular architecture with three main components:

1. **Game**: Main game loop, SDL initialization, and event handling
2. **PhysicsWorld**: Box2D physics simulation management
3. **GameObject**: Base class for physics-enabled game objects

The game runs at 60 FPS with a fixed physics timestep for stable simulation.
