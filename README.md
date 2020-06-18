# GamesEngCourseWork
Group project repository for the module Games Egineering 2017/2018 SET09121 - Potato's Destiny  

# Aim
Implement 2D game taking advantage of the provided engine and system physics.

# Build
The project has been built taking advantage of SFML to get access to windowing, graphics and audio. Box2D has been used for the game Physics engine.

Entity component has been used to give similar and different features to the games entities

[![Build status](https://ci.appveyor.com/api/projects/status/s56gtqedqdl563d8?svg=true)](https://ci.appveyor.com/project/Willyees/games-cw)

![potatosdestiny](https://github.com/Willyees/Games_cw/blob/assets/assets/potatosdestiny.png)


How to build it:

1: Clone repo

2: git submodule update --init --recursive

3: Create visual studio project with CMake (using the CMakeList.txt)

4: Set pd as startup project

5: Build it

# Features
- Game can be paused, saved, reloaded and restarted. Collision detection is performed to reduce lives of main character. 
![features](https://github.com/Willyees/Games_cw/blob/assets/assets/features.png)

- Photato sprite switched based on the type of movement (same is for the enemies). 
![sprite](https://github.com/Willyees/Games_cw/blob/assets/assets/sprite.gif)

- One powerup implemented. 

![powerup](https://github.com/Willyees/Games_cw/blob/assets/assets/powerup.png)

- Multiple resolution available in full screen mode
![resolutions](https://github.com/Willyees/Games_cw/blob/assets/assets/resolution.png)

- Enemies AI targeting and shooting at the character position
![shooting](https://github.com/Willyees/Games_cw/blob/assets/assets/sprite_shooting.gif)


# How to play
Simple 1 level platform game. The aim of the game is to find the key and use it to open the end portal to unlock the next level (it is not yet implemented) and complete the game.
Comands: Use the arrows to move ← ↑ → ↓

3 total lives which are lost at each harmful collision. Enemies are invicible, so is best to avoid them. Coins can be collected for future shop items.
