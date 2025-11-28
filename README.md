Denix Game Engine

A high-performance custom 3D Game Engine built from first principles in Modern C++ (C++20). This project demonstrates a deep understanding of core game engine architecture, including memory management, physics integration, multithreading, and rendering pipelines.

Status: 🚧 Active Development 🚧
Engineered to explore the limits of custom game technology without reliance on commercial middleware.

🎯 Engineering Goals

The Denix Engine was architected to solve specific technical challenges inherent in game development:

Performance: Implementing a custom memory management system and experimental job system to maximise CPU utilisation.

Modularity: Utilising a hybrid OOP/ECS architecture to balance flexibility with cache-friendly data layouts.

Tooling: Creating a robust editor environment with runtime shader editing and integrated physics debugging.

🛠️ Technology Stack

Core Language: C++ (C++20 standards)

Rendering: OpenGL (Core Profile) via GLEW

Physics: Nvidia PhysX 4.1 (Full rigid body integration)

Platform: SDL3 (Windowing & Input)

Audio: OpenAL Soft

Build System: CMake

🏛️ System Architecture

1. Hybrid Architecture (OOP + ECS)

The engine utilises a hybrid approach, leveraging Object-Oriented Programming for high-level game logic (Actors) while employing Entity-Component-System (ECS) principles for performance-critical subsystems like rendering and physics.

2. Experimental Job System

Inspired by the architecture of engines like REDengine (Cyberpunk 2077), the Denix Engine features a custom job system that moves beyond simple threading.

Features: Worker thread pool, priority-based task scheduling, and lock-free queues.

Result: Efficient distribution of tasks across available CPU cores, preventing main-thread bottlenecks.

3. Integrated Physics Debugging

Full integration with the PhysX Visual Debugger (PVD) allows for real-time visualisation of collision geometry, constraints, and rigid body dynamics, essential for debugging complex physics interactions.

🎮 The Tech Demo: "Kill the Color!"

Included is a testbed application designed to stress-test the engine's physics and rendering capabilities.

Controls: Standard WASD + Mouse camera movement.

Interaction: Space to fire projectiles and interact with the physics-enabled environment.

UI: Custom experimental UI system for menu navigation (Up/Down/Space).

🧰 Developer Tools

The engine includes a suite of developer-facing tools to aid in content creation and profiling:

Runtime Shader Editor: Modify GLSL shaders on the fly via the Render Component widget without restarting the engine.

Game Slow Motion: Debug fast-moving physics interactions by modifying the global time dilation.

Performance Profiler: Real-time metrics for frame time, render thread usage, and physics simulation steps.

🚀 Building the Project

Prerequisites

CMake (3.20+)

C++ Compiler supporting C++20 (MSVC recommended for Windows)

Build Instructions

Clone the repository.

Generate project files:

mkdir build
cd build
cmake ..


Open the solution and set DevProject as the startup project.

Running & Debugging

Executable: Located in DevProject/bin.

PhysX Debugging: Launch the PhysX Visual Debugger from DevProject/PVD3 before running the engine to capture the session.

Documentation: Full API documentation is available in Doxygen/html/index.html.

Developed by Denis Pickering
First-Class Honours Graduate in Games Software Engineering
