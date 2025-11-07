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

### Windows Dependencies

**Option 1: Using vcpkg (Recommended)**

```powershell
# Clone vcpkg (if not already installed)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies
.\vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows box2d:x64-windows

# Integrate vcpkg with Visual Studio
.\vcpkg integrate install
```

**Option 2: Manual Installation**

1. Install Visual Studio Community with C++ development tools
2. Download SDL2 development libraries from https://github.com/libsdl-org/SDL/releases
3. Download SDL2_image from https://github.com/libsdl-org/SDL_image/releases
4. Download Box2D from https://github.com/erincatto/box2d/releases
5. Extract to a known location and set environment variables or update CMakeLists.txt with include/lib paths


### Ubuntu/Debian Dependencies

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
sudo apt install libbox2d-dev
```

## Building

### Windows

**Step 1: Install vcpkg packages**

```powershell
vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows box2d:x64-windows
```

**Step 2: Find your vcpkg installation path**

Run this to find where vcpkg is installed:
```powershell
(Get-Command vcpkg).Source | Split-Path -Parent
```

This will output something like `C:\Users\YourName\vcpkg` or `C:\vcpkg`.

**Step 3: Configure and build (replace `VCPKG_PATH` with your actual path from Step 2)**

```powershell
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="VCPKG_PATH\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

**Example (if vcpkg is at `C:\vcpkg`):**
```powershell
cmake -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows -G "Visual Studio 16 2019" -A x64 ..
```

### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

## Running

### Windows

```powershell
# From build directory
.\bin\Release\ConsoleGame.exe

# Or if built with Visual Studio directly
.\Release\ConsoleGame.exe
```

### Linux/macOS

```bash
./bin/ConsoleGame
```

## Controls

- **ESC**: Toggle pause menu
- **ENTER**: Select menu option / Confirm action
- **SPACE**: Jump (when playing) / Add a new random physics object (in debug mode)
- **Arrow Keys**: Move player character
- **Window Close**: Exit the game

## Troubleshooting

### Windows Issues

**CMake Error: Could not find toolchain file**
- You used a placeholder path instead of your actual vcpkg location
- **Solution:**
  1. Find your vcpkg installation directory:
     ```powershell
     (Get-Command vcpkg).Source | Split-Path -Parent
     ```
  2. Replace `VCPKG_PATH` in the cmake command with the actual path
  3. Example if your vcpkg is at `C:\vcpkg`:
     ```powershell
     cmake -DCMAKE_TOOLCHAIN_FILE="C:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows -G "Visual Studio 16 2019" -A x64 ..
     ```

**CMake can't find SDL2, SDL2_ttf, or Box2D**
- Ensure all packages are installed: `vcpkg install sdl2:x64-windows sdl2-image:x64-windows sdl2-ttf:x64-windows box2d:x64-windows`
- Verify the toolchain file path is correct
- Clear build cache: `Remove-Item build -Recurse` and reconfigure

**DLL not found errors at runtime**
- Add the vcpkg bin directory to PATH: `C:\path\to\vcpkg\installed\x64-windows\bin`
- Or copy DLLs to the same directory as the executable

**Visual Studio build fails with "missing dependencies"**
- Reinstall vcpkg libraries: `vcpkg remove sdl2:x64-windows sdl2-image:x64-windows box2d:x64-windows`
- Then reinstall: `vcpkg install sdl2:x64-windows sdl2-image:x64-windows box2d:x64-windows --recurse`

**CMake configuration fails**
- Clear cmake cache: `rm -r build` (PowerShell: `Remove-Item build -Recurse`)
- Ensure Visual Studio build tools are installed
- Try using Ninja if Visual Studio generator fails

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
