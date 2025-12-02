# Snake Game (C + ncurses)
Team: Jowel Martin & Kelly Ruiz

This project is a terminal-based implementation of the classic Snake game written in C using the ncurses library.
The game runs entirely in a Codespace terminal:
-A bordered snake pit (minimum 20×20)
-A snake that starts with length 3
-Randomly generated food
-Snake growth when eating
-Wall and self collision lead to losing
-Win/loss conditions
-Arrow-key movement
-“Game Over / You Win” screen
-Can restart with space
-All visuals are rendered using ncurses.

# Team Members & Contributions
Kelly Ruiz:
-Created repository
-Implemented play_game()
-Implemented place_food()

Jowel Martin:
-Initialized and set up the game
-Set up the Main()

# Controls
↑ - Moves up
↓  - Moves Down
← - Moves Left
→ - Moves Right
Space - restarts game
Control-c - ends game

# How to Compile
First: 
gcc snake1.c -o snake1 -lncurses
Then: 
./snake1
