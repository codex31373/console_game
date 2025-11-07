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
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
sudo apt install libbox2d-dev
```

## Building

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

- **ESC**: Toggle pause menu
- **ENTER**: Select menu option / Confirm action
- **SPACE**: Jump (when playing) / Add a new random physics object (in debug mode)
- **Arrow Keys**: Move player character
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
├── assets/             # Game assets
│   └── fonts/          # Font files
├── include/            # Core header files
│   ├── Game.hpp
│   ├── PhysicsWorld.hpp
│   └── GameObject.hpp
├── src/                # Source files
│   ├── Bird.cpp/hpp    # Bird game object implementation
│   ├── Common.hpp      # Common definitions and includes
│   ├── Game.cpp        # Main game logic
│   ├── GameObject.cpp  # Base game object class
│   ├── PhysicsWorld.cpp # Physics simulation
│   ├── Platform.cpp/hpp # Platform game object
│   ├── Player.cpp/hpp  # Player character implementation
│   ├── Stair.cpp/hpp   # Stair game object
│   └── main.cpp        # Entry point
└── README.md           # This file
```

## Architecture

The game follows a component-based architecture with these key components:

1. **Game (Game.cpp/hpp)**
   - Manages the main game loop and timing
   - Handles SDL initialization and window management
   - Processes input events and updates game state
   - Manages the rendering pipeline
   - Coordinates between different game systems

2. **PhysicsWorld (PhysicsWorld.cpp/hpp)**
   - Wraps Box2D physics simulation
   - Manages physics bodies and their interactions
   - Handles collision detection and response
   - Provides an interface for creating and managing physics objects

3. **GameObject (GameObject.cpp/hpp)**
   - Base class for all game entities
   - Implements common functionality like position, rotation, and scale
   - Provides virtual methods for updating and rendering
   - Manages object lifetime and cleanup

4. **Game Objects**
   - **Player (Player.cpp/hpp)**: Represents the player character with movement and interaction logic
   - **Bird (Bird.cpp/hpp)**: Implements bird behavior and AI
   - **Platform (Platform.cpp/hpp)**: Static or moving platforms in the game world
   - **Stair (Stair.cpp/hpp)**: Stair objects for level navigation

5. **Common (Common.hpp)**
   - Shared constants and type definitions
   - Common includes and utility functions
   - Global configuration parameters

The game runs at 60 FPS with a fixed physics timestep for stable simulation. The architecture is designed to be extensible, making it easy to add new game objects and features.
