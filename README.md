# OOP_Monopoly

A C++ implementation of the classic Monopoly board game.

## Introduction

OOP Lab. Group project.

## Features

- Multiple players support
- Basic Monopoly rules (buying land, paying rent, going to jail, etc.)
- Items and cards with different effects (like Control Dice, Rocket Card, Fate Card, Destroy Card)
- Turn-based gameplay
- Command-line interface for easy play

## Getting Started

### Prerequisites

- C++ compiler (like g++, clang++)
- Git (optional, for cloning the repo)

### How to Run

1. Clone the repository:
    ```sh
    git clone https://github.com/xuwwwww/OOP_Monopoly.git
    cd OOP_Monopoly
    ```

2. Compile the code:
    ```sh
    g++ -std=c++11 -o monopoly main.cpp
    ```

3. Run the game:
    ```sh
    ./monopoly
    ```

Depending on the folder structure, you may need to adjust the compile command to include all source files.

## File Structure

- `main.cpp`: The main entry point of the game
- `Player.h/cpp`: Player class and logic
- `Game.h/cpp`: Core game logic
- `Item.h/cpp`: Items and their effects
- `CommandHandler.h/cpp`: Command processing
- `Map.h/cpp`: Monopoly map and properties
- `README.md`: This file

## How to Play

Start the game and follow the instructions on the screen. You will enter commands to roll dice, buy land, use items, and more. Each player takes turns and tries to become the richest player, just like in real Monopoly. But in CLI.

---
