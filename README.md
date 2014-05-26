Total Resistance
================

Overview
--------

Total Resistance is a MIT licensed cross-platform tower defense game written in C++11. The game formed part of my final year Computer Science project at Royal Holloway, University of London. Much of the work was focused on implementing a custom game engine library.

Engine Details
--------------

**Computer graphics**

 * Orthographic camera
 * Directional lighting
 * GLSL shader support

**User interface**

 * Simple 2D shapes
 * Textures and solid fill colours
 * Text using FreeType fonts

**Audio**

 * Music and sound playback
 * Clamped audio mixing

**Resource management**

 * Reference counted resource management
 * Multi-threaded resource loading using message-passing

**Architecture**

 * Component-oriented design
 * Typesafe publish-subscribe implementation

Building
--------

The game has been tested on both Mac OS X and Linux. Building requires CMake in order to generate platform-specific  makefiles or IDE projects. The Clang compiler has been exclusively used during development. You will need to install the following libraries before building the project:

 * GLFW3
 * GLEW
 * GLM
 * ASSIMP
 * FreeType 2
 * PortAudio
 
Building the software requires that you first `cd` to the `/build/` directory and then run one of the several shell scripts provided in the `/scripts/` directory.

Credits
-------

1. Portions of this software are copyright 2014 The FreeType Project (www.freetype.org). All rights reserved.
2. Portions of this software have been adapted from SFML (www.sfml-dev.org) and are copyright 2014 Laurent Gomila.
3. All music used in this software was created by Matthew Pablo (http://www.matthewpablo.com/).