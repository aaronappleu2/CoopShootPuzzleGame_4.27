# CoopShootPuzzleGame_4.27

## Introduction

This Project is a UE4 Game Project implemented through the tutorial of  [Tom Looman](https://www.tomlooman.com/unreal-engine-cpp-survival-sample-game/). 

The project uses the UE4 public learning resource package. 

This project is for study use only.

## Game Properties

- Character movement with aim offset
- Character Health component
- Gun Weapon class
- Pick-up items that provide gains
- Multiplayer game implementation
- AI Enemies have the performance of looking for cover
- Store the highest score ever
- Minesweeper-like puzzle components  as penalties and victory conditions

### Minesweeper-like puzzle

There is a 5x5 grid with a ball in each cell. N of them are bombs and the others are normal but will show different styles depending on the number of bombs around them.

The goal of the puzzle is to find one of the balls, which has bombs in all four directions(up, down, left, and right) without going out of range.

If the ball is found, the game is won. If the bomb is detonated before it is found, a new puzzle will be refreshed and new enemies will appear.

![20220423_180643046](https://github.com/aaronappleu2/CoopShootPuzzleGame_4.27/blob/main/sample.jpg)
