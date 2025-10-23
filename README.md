# Minesweeper (C++ + NASM + Raylib)

A classic **Minesweeper** implementation built using **C++**, **NASM Assembly**, and **Raylib**.  This project was completed with three other teammates as part of course in University of Costa Rica.
This project combines high-level game logic in C++ with low-level performance routines written in Assembly.

<img width="825" height="545" alt="image" src="https://github.com/user-attachments/assets/000f9e38-b7f0-45d0-991e-8552d04c7317" />

---

## 🚀 Features
- 🕹️ Fully playable Minesweeper with GUI built using Raylib  
- ⚙️ Game logic and matrix operations written in **C++**  
- 🧮 Optimized matrix use in **NASM Assembly**  
- 🏁 build using **CMake**
- 🏆 Multiple Difficulties: Beginner, Intermediate, and Expert levels
- ⏱️ Game Timer: Measure your performance
- 📊 Leaderboard System: Track and persist high score with a .csv file.
- 🎨 Dynamic graphics, assets.

## 🛠️ Technology Stack
- C++: Core game logic and architecture
- NASM Assembly: Performance-critical matrix operations
- Raylib: Cross-platform graphics and input handling
- CMake: Modern build system configuration
- Linux: Primary development and deployment platform

## 📋 Prerequisites
Required Dependencies: <br>
CMake (version 4.1.2) <br>
Git

## 🛠️Compile the project 
- Create and enter build directory
mkdir build && cd build

- Configure with CMake
cmake ..

- Compile
make
make -j$(nproc) (recommended: uses all CPU cores)

- Alternative: compile with verbose output for debugging
make VERBOSE=1

- Run the game
./buscaminas

## 🧹Clean Build
rm -rf build

## How to play

Left Click: Reveal a cell

Right Click: Place/remove a flag

## Game Rules
Click on cells to reveal them

Numbers indicate how many mines are adjacent

Right-click to mark suspected mines with flags

Clear all non-mine cells to win

## 🙏 Acknowledgments
Raylib community for the excellent graphics library
NASM developers for the robust assembler
Classic Microsoft Minesweeper for inspiration
Contributors and testers who helped improve the game

## ✅ Future features / Pending actions
- Implement propagation (flood fill) — currently, clicking on a cell only reveals that single cell, but it should automatically reveal adjacent empty cells.
- Translate all code comments from english to spanish.

# Enjoy the game and happy coding! 🎮💻


