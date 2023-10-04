# Tetris
Exploring Tetris Implementation in C++: A Practical Exercise

## Project preview
* ![image](https://github.com/Ting-Xiao-Miaw/Tetris/blob/main/.resource/preview.png)

## Source file
1. wincursor.obj - provides the implementation of the class Screen and String.
2. wincursor.h - declares the prototype of Screen and String. Your main program must include this file.
3. cursor.h - this is required by wincursor.h, which implements the cursor manipulation with ncurses.
4. pdcurses.lib - the library of ncurses to be linked with your program.
5. pdcurses.dll - run-time library for your executable file.
6. tetris.h - self-defined class CTetromino.
7. tetris-8.cpp - main program.

## Build up on Windows
Compile the program. 
```
CL /c tetris-8.cpp /I.
LINK tetris-8.obj wincursor.obj pdcurses.lib
```
It generates an executable file "tetris-8.exe"
