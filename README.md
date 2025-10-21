# Proto Pong

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A modern C++20 implementation of the classic Pong game, built from the ground up with SDL2, OpenGL, and a focus on best
practices in software development.

This project is a complete modernization of an older personal project from 2016, created for educational purposes. It
serves as a practical demonstration of modern C++20 features and robust software design patterns in the context of a
real-time application.

## Screenshots

<img src="docs/screenshot_menu.jpg?raw=true" alt="Main Menu Screenshot" width="45%"> <img src="docs/screenshot_game.jpg?raw=true" alt="Gameplay Screenshot" width="45%">

## Detailed Description

**Proto Pong** is not just another Pong clone. It is a software engineering exercise focused on writing idiomatic,
modern C++ (C++20). The entire project, from the main loop to the management of GPU resources, has been designed to be
robust, maintainable, and efficient, following principles common in the game development industry.

### Key Features

*   **Modern Tech Stack:** Written in **C++20** and built with **CMake**.
*   **Cross-Platform Rendering:** Uses **SDL2** for window and event management, and **OpenGL 3.3+** for 
    hardware-accelerated 2D rendering.
*   **Robust Object-Oriented Design:** A clean, entity-based architecture with a clear separation of concerns.
*   **Challenging Paddle AI:** An AI controller that predicts the ball's trajectory, complete with reaction timing and
    intentional error for more human-like behavior.
*   **Fixed-Timestep Game Loop:** A classic main loop implementation with a fixed physics timestep and variable
    rendering for deterministic behavior and smooth animation.
*   **RAII Resource Management:** No manual `new`/`delete` for game resources. Everything (OpenGL objects, SDL devices,
    game entities, etc.) is automatically managed by smart pointers (`std::unique_ptr`) and custom RAII handles,
    eliminating memory leaks.

## Project Architecture

The design of Proto Pong is based on several key software principles and design patterns:

*   **State Machine (`Game`):** The `Game` class acts as a finite state machine that orchestrates the game flow,
    transitioning between states like the Main Menu, Match, Win Screen, etc.
*   **Entity System:** All game objects (`Ball`, `Paddle`, `Label`, `Table`) inherit from a base `Entity` class,
    providing a common interface for updating and rendering. The `Scene` class manages the ownership and lifecycle of
    these entities.
*   **Strategy Pattern (`Controller`):** The behavior of the paddles is decoupled from the `Paddle` entity itself. The
    `Controller` interface allows different "brains" to be injected, such as a `ControllerHuman` (which responds to
    keyboard input) or a `ControllerAI` (which implements the AI logic).
*   **Batched Rendering System:** The rendering architecture is decoupled through an abstract `Renderer` interface, with 
    a concrete `RendererGL3` implementation for OpenGL 3.3+. To maximize performance, it uses a batched approach: during
    the game's draw phase, entities queue their geometry (quads) into a buffer. At the end of the frame, the renderer
    issues a minimal number of draw calls to the GPU to render everything at once, significantly reducing API overhead.

## Building from Source

### Prerequisites

You will need the following tools to compile the project:

*   A C++20 compliant compiler (GCC 10+, Clang 10+, MSVC v142+).
*   CMake (version 3.15 or higher).
*   Git.
*   vcpkg: For Windows builds, the vcpkg package manager is recommended. Please install it and set the VCPKG_ROOT
    environment variable as per the official instructions.

### Dependencies

*   **For Windows:**
    *   **`SDL2` and `glm`** are managed automatically by **vcpkg**. When you configure the project, vcpkg will read the
        `vcpkg.json` manifest file and install these libraries for you. No manual installation is needed.
*   **For Linux / macOS:**
    *   **`SDL2`:** Must be installed on your system.
        *   **Ubuntu/Debian:** `sudo apt-get install libsdl2-dev`
        *   **macOS (with Homebrew):** `brew install sdl2`
    *   **`glm`** is managed by CMake's `FetchContent`.
*   **OpenGL:**
    *   Typically installed with your graphics card drivers.
*   **Internal Dependencies:**
    *   **`glad` and `utf8d`** are included directly in the repository (`thirdparty/`) and are handled automatically by
        CMake.

### Compilation Steps

#### Windows (via IDE - Recommended for Debugging)

This project is configured with a `CMakePresets.json` file to provide a simple, one-click setup in an IDE that supports presets (Visual Studio 2022, VS Code with the CMake Tools extension, CLion).

1.  **Clone the repository.**
2.  **Open the project folder** in your IDE.
3.  The IDE should automatically detect the `CMakePresets.json` file. **Select the `windows-vs` configure 
    preset.**
4.  **Build and Run** the `protopong` target from your IDE. CMake and vcpkg will handle the rest.


#### Windows (via Command-Line)

Ensure you are in the root directory of the project.

1.  **Configure the project using the preset:**
    ```bash
    # This reads CMakePresets.json and configures the project in the 'build' directory
    cmake --preset windows-vs
    ```

2.  **Compile the project:**
    ```bash
    # Build the Release configuration. You can also use 'Debug' or 'RelWithDebInfo'.
    cmake --build build --config Release
    ```
    The executable will be available at `build/src/Release/protopong.exe`.

3.  **(Optional) Create a distributable package:**
    ```bash
    # This will create a clean, self-contained package in the 'install' directory
    cmake --install build --config Release --prefix ./install
    ```
    You can then run the game from `install/bin/protopong.exe`.

#### Linux / macOS / Command-Line

For non-Windows platforms or for a manual command-line build:

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/gonrogon/protopong.git
    cd protopong
    ```

2.  **Configure the project:**
    ```bash
    cmake -S . -B build
    ```

3.  **Compile the project:**
    ```bash
    cmake --build build
    ```

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for details.