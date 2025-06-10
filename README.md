# CMakeLearn - SFML and ImGui Template

A modern C++ project template using CMake, SFML 3.0, and ImGui integration.

## Features

- Modern CMake (3.28+) configuration
- SFML 3.0.1 integration with automatic dependency management
- Dear ImGui integration with SFML
- Cross-platform support (Windows, macOS, Linux)
- Basic example demonstrating window creation, event handling, and UI

## Requirements

- CMake 3.28 or higher
- C++17 compatible compiler
- Git

## Quick Start

```bash
# Clone this template
git clone https://github.com/yourusername/CMakeLearn.git
cd CMakeLearn

# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./src/CMakeLearn
```

## Project Structure

```
CMakeLearn/
├── .github/            # GitHub-specific files (workflows, etc.)
├── cmake/              # CMake modules and utilities
├── dependencies/       # External dependencies configuration
├── src/                # Source code
│   ├── main.cpp        # Application entry point
│   ├── Shape.h         # Example shape class
│   └── Shape.cpp       # Implementation
├── .gitignore          # Git ignore rules
├── CMakeLists.txt      # Main CMake configuration
├── LICENSE             # License information
└── README.md           # This file
```

## Customizing the Template

After using this template:

1. Update the project name in the root `CMakeLists.txt`
2. Modify the application in `src/main.cpp`
3. Add your own classes and files to the `src` directory
4. Update this README.md with your project information

## License

This template is available under the MIT License. See the LICENSE file for details.
