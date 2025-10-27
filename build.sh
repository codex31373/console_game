#!/bin/bash

# Build script for Console Game

echo "Building Console Game..."

# Create build directory if it doesn't exist
mkdir -p build

# Configure with CMake
cd build
cmake ..

# Build the project
make

echo "Build complete!"
echo "Run the game with: ./bin/ConsoleGame"
