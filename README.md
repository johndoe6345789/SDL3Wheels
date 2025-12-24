# SDL3Wheels

SDL3Wheels - A clone of the classic DOS game "Wacky Wheels" built with SDL2, CMake, Ninja, and vcpkg.

## About

Wacky Wheels was a popular kart racing game from 1994. This project recreates the core gameplay mechanics using modern C++ and SDL2.

## Features

- 🏎️ Kart racing gameplay with physics
- 🤖 AI opponents
- 🏁 Lap tracking and race timing
- ⌨️ Keyboard controls (Arrow keys or WASD)
- 🎮 Menu system with pause functionality
- 📦 Modern C++ architecture with clean separation of concerns

## Prerequisites

- CMake 3.21 or higher
- Ninja build system
- C++17 compatible compiler
- vcpkg (for dependency management)

## Building

### Setup vcpkg

If you don't have vcpkg installed:

```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # Linux/Mac
# or
bootstrap-vcpkg.bat   # Windows
```

Set the VCPKG_ROOT environment variable:

```bash
export VCPKG_ROOT=/path/to/vcpkg  # Linux/Mac
# or
set VCPKG_ROOT=C:\path\to\vcpkg   # Windows
```

### Build the Game

```bash
# Configure
cmake --preset=default

# Build
cmake --build build

# Run
./build/SDL3Wheels  # Linux/Mac
# or
build\SDL3Wheels.exe  # Windows
```

### Debug Build

```bash
cmake --preset=debug
cmake --build build-debug
```

## Controls

### Menu
- **Arrow Keys**: Navigate menu
- **Enter/Space**: Select option
- **Escape**: Exit

### Racing
- **Arrow Keys** or **WASD**: Control kart
  - Up/W: Accelerate
  - Down/S: Brake
  - Left/A: Turn left
  - Right/D: Turn right
- **Escape**: Pause/Resume
- **P**: Pause

## CI/CD

The project includes a GitHub Actions workflow that:
- Builds on Linux and Windows
- Uses Ninja build system
- Manages dependencies via vcpkg
- Includes QEMU testing support
- Produces build artifacts

## Architecture

```
SDL3Wheels/
├── src/              # Source files
│   ├── main.cpp      # Entry point
│   ├── game.cpp      # Main game loop
│   ├── kart.cpp      # Kart physics
│   ├── track.cpp     # Track definition
│   ├── input_handler.cpp  # Input handling
│   └── renderer.cpp  # Rendering logic
├── include/          # Header files
├── assets/           # Game assets
├── .github/
│   └── workflows/    # CI/CD pipelines
├── CMakeLists.txt    # CMake configuration
├── CMakePresets.json # CMake presets
└── vcpkg.json        # Dependency manifest
```

## Technologies

- **SDL2**: Graphics, input, and window management
- **CMake**: Build system configuration
- **Ninja**: Fast build execution
- **vcpkg**: C++ package manager
- **GitHub Actions**: CI/CD automation
- **QEMU**: Cross-platform testing

## License

See LICENSE file for details.

## Acknowledgments

Inspired by "Wacky Wheels" by Apogee Software (1994).

