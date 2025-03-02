Raycasting FPS

Overview

This project implements a basic raycasting engine using SDL2 to render a 3D-like environment. It features player movement similar to early FPS games like Wolfenstein 3D.

Features

Raycasting rendering for a pseudo-3D effect

Player movement with forward, backward, rotation controls

Basic collision detection to prevent walking through walls

SDL2-based graphics

Controls

W - Move forward

S - Move backward

A - Rotate left

D - Rotate right

ESC - Quit the application

Dependencies

SDL2

Compilation & Execution

To compile and run the project, ensure you have SDL2 installed, then use:

gcc -o raycaster raycasting.c -lSDL2 -lm
./raycaster

Future Improvements

Implementing texturing for walls

Adding floor and ceiling rendering

Implementing enemy AI and interactive objects

Acknowledgments

This project is inspired by classic raycasting techniques used in early 3D games