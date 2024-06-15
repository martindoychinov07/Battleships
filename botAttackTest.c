#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10

void botAttack(char playerBoard[BOARD_SIZE][BOARD_SIZE]) {
    int x = rand() % BOARD_SIZE;
    int y = rand() % BOARD_SIZE;

    while (playerBoard[y][x] == 'H' || playerBoard[y][x] == 'M') {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
    }

    if (playerBoard[y][x] == '-') {
        playerBoard[y][x] = 'H';
    }
    else {
        playerBoard[y][x] = 'M';
    }
}

int main() {
    srand(time(NULL));

    char board[BOARD_SIZE][BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }

    board[0][0] = 'H';
    board[7][8] = 'M';

    botAttack(board);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }

    return 0;
}
