# Particle-Simulator

# Particle Physics Simulation

A real-time particle physics simulation featuring different materials with unique behaviors including sand, water, fire, snow, and water spray effects.

## Features

- **Multiple Materials**: Sand, Water, Fire, Snow, and Water Spray
- **Real-time Physics**: Gravity, fluid dynamics, and particle interactions
- **Interactive Controls**: Mouse-driven particle creation with keyboard material selection
- **Optimized Rendering**: Efficient OpenGL-based visualization

## Controls

- **S** - Sand particles (falls and settles)
- **W** - Water particles (flows and spreads)
- **F** - Fire particles (rises upward)
- **N** - Snow particles (powder-like behavior)
- **O** - Water spray particles (spray effect)
- **Mouse** - Hold left button and drag to create particles
- **ESC** - Exit application

## Dependencies

The project requires the following libraries:

- **OpenGL** - Graphics rendering
- **GLFW** - Window management and input handling
- **GLUT** - OpenGL utilities
- **GLEW** - OpenGL extension loading

## Building the Project

### Prerequisites

Make sure you have the following installed:

- CMake 3.16 or higher
- A C++17 compatible compiler (GCC, Clang, or MSVC)
- OpenGL development libraries
- GLFW development libraries
- GLUT development libraries
- GLEW development libraries

### Platform-Specific Setup

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install cmake build-essential
sudo apt-get install libglfw3-dev libglew-dev freeglut3-dev
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
```

#### macOS
```bash
# Using Homebrew
brew install cmake glfw glew freeglut
```

#### Windows
- Install CMake from https://cmake.org/
- Install Visual Studio with C++ development tools
- Libraries can be installed via vcpkg:
```cmd
vcpkg install glfw3 glew freeglut
```

### Build Steps

1. **Clone or extract the project files**

2. **Create a build directory**
```bash
mkdir build
cd build
```

3. **Configure the project**
```bash
cmake ..
```

On Windows with vcpkg:
```cmd
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

4. **Build the project**
```bash
cmake --build . --config Release
```

5. **Run the executable**
```bash
# On Linux/macOS
./bin/ParticleSimulation

# On Windows
.\bin\ParticleSimulation.exe
```

### Build Options

- **Debug Build**: `cmake --build . --config Debug`
- **Release Build**: `cmake --build . --config Release`
- **Install**: `cmake --install .`

## Architecture

The refactored code follows a clean, object-oriented design:

### Key Components

- **ParticleSimulation**: Main simulation class handling physics and rendering
- **Particle**: Individual particle data structure
- **MaterialType**: Enum defining different particle types
- **Application**: Main application wrapper handling window management

### Improvements Made

1. **Modular Design**: Separated concerns into distinct classes
2. **Type Safety**: Used enums and strong typing
3. **Memory Management**: Used smart pointers and RAII
4. **Performance**: Optimized grid updates and rendering
5. **Maintainability**: Clear separation of physics, rendering, and input handling
6. **Extensibility**: Easy to add new material types

### Physics Implementation

Each material type has unique physics behavior:

- **Powder Materials** (Sand, Snow): Fall with gravity, settle in piles
- **Fluid Materials** (Water): Flow and spread horizontally
- **Fire**: Rises upward with random dispersion
- **Water Spray**: Falls with lateral spray effects

## Troubleshooting

### Common Issues

1. **Missing Libraries**: Ensure all dependencies are installed
2. **CMake Not Found**: Install CMake or add it to PATH
3. **Compilation Errors**: Check compiler C++17 support
4. **Runtime Errors**: Verify graphics drivers are updated

### Performance Tips

- The simulation runs at 60 FPS by default
- Large numbers of particles may impact performance
- Adjust `SPAWN_PROBABILITY` in the header to control particle density

## License

This project is provided as-is for educational and demonstration purposes.
