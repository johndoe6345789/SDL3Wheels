# SDL3Wheels

SDL3Wheels - A clone of the classic DOS game "Wacky Wheels" built with SDL3, CMake, Ninja, and Conan.

## About

Wacky Wheels was a popular kart racing game from 1994. This project recreates the core gameplay mechanics using modern C++ and SDL3, with a renderer abstraction layer that allows swapping rendering backends.

## Features

- 🏎️ Kart racing gameplay with physics
- 🤖 AI opponents
- 🏁 Lap tracking and race timing
- ⌨️ Keyboard controls (Arrow keys or WASD)
- 🎮 Menu system with pause functionality
- 📦 Modern C++ architecture with clean separation of concerns
- 🎨 Renderer abstraction layer for flexibility

## Architecture

The game features a renderer abstraction layer (`IRenderer`) that decouples the game logic from the rendering implementation. Currently, SDL3 is used via `SDL3Renderer`, but the abstraction allows easy swapping to other rendering backends (Vulkan, DirectX, etc.) without changing game code.

```
Game Logic → IRenderer (interface) → SDL3Renderer (implementation)
```

## Prerequisites

- CMake 3.21 or higher
- Ninja build system
- C++17 compatible compiler
- Conan 2.x (for dependency management)
- Python 3 (for Conan)

## Building

### Setup Conan

Install Conan via pip:

```bash
pip install conan
```

Detect your Conan profile:

```bash
conan profile detect --force
```

### Build the Game

```bash
# Install dependencies with Conan
conan install . --output-folder=build --build=missing -s build_type=Release

# Configure with CMake
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake

# Build
cmake --build build

# Run
./build/SDL3Wheels  # Linux/Mac
# or
build\SDL3Wheels.exe  # Windows
```

### Debug Build

```bash
conan install . --output-folder=build-debug --build=missing -s build_type=Debug
cmake -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=build-debug/conan_toolchain.cmake
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
- Manages dependencies via Conan
- Includes QEMU testing support
- Produces build artifacts

## Project Structure

```
SDL3Wheels/
├── src/                    # Source files
│   ├── main.cpp            # Entry point
│   ├── game.cpp            # Main game loop
│   ├── kart.cpp            # Kart physics
│   ├── track.cpp           # Track definition
│   ├── input_handler.cpp   # Input handling
│   └── sdl3_renderer.cpp   # SDL3 renderer implementation
├── include/                # Header files
│   ├── game.h
│   ├── kart.h
│   ├── track.h
│   ├── input_handler.h
│   ├── i_renderer.h        # Renderer interface
│   └── sdl3_renderer.h     # SDL3 renderer header
├── assets/                 # Game assets
├── .github/
│   └── workflows/          # CI/CD pipelines
├── CMakeLists.txt          # CMake configuration
└── conanfile.txt           # Conan dependency manifest
```

## Technologies

- **SDL3**: Graphics, input, and window management
- **CMake**: Build system configuration
- **Ninja**: Fast build execution
- **Conan**: C++ package manager
- **GitHub Actions**: CI/CD automation
- **QEMU**: Cross-platform testing

## Extending the Renderer

To add a new rendering backend:

1. Create a class that implements `IRenderer` interface
2. Implement all virtual methods
3. Update `Game::init()` to instantiate your renderer
4. No changes needed to game logic!

Example:
```cpp
class VulkanRenderer : public IRenderer {
    // Implement all IRenderer methods
};

// In game.cpp:
renderer_ = std::make_unique<VulkanRenderer>(...);
```

## License

See LICENSE file for details.

## Acknowledgments

Inspired by "Wacky Wheels" by Apogee Software (1994).

