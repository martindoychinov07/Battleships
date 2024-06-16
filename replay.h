#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 10

typedef struct {
    char p1Board[BOARD_SIZE][BOARD_SIZE];
    char p2Board[BOARD_SIZE][BOARD_SIZE];
}readInfo;

readInfo readStep(char* filename, int moveNumber);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void playReplay(char* filename);