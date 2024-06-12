#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10

struct Ship {
    int size;
    int count;
};

typedef struct Ship Ship;

Ship ships[] = {{6, 1}, {4, 2}, {3, 3}, {2, 4}};

int validPos(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return 0;
    }
    else {
        for (int xi = -1; xi <= 1; xi++) {
            for (int yi = -1; yi <= 1; yi++) {
                int tx = x + xi;
                int ty = y + yi;
                if (0 <= tx && tx < BOARD_SIZE && 0 <= ty && ty < BOARD_SIZE) {
                    if (board[ty][tx] != ' ') {
                        return 0;
                    }
                }
            }
        }
        return 1;
    }
}

int validArea(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int size, char dir) {
    for (int i = 0; i < size; i++) {
        int valid = validPos(board, x + (dir == 'L' ? -i: dir == 'R' ? i : 0), y + (dir == 'U' ? -i: dir == 'D' ? i : 0));
        if (valid == 0) {
            return 0;
        }
    }
    return 1;
}

void randomShips(char board[BOARD_SIZE][BOARD_SIZE]) {
    int count = (sizeof(ships) / sizeof(ships[0]));

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < ships[i].count; j++) {
            int x = rand() % BOARD_SIZE;
            int y = rand() % BOARD_SIZE;
            char dir = "UDLR"[rand() % 4];

            while (validArea(board, x, y, ships[i].size, dir) == 0) {
                x = rand() % BOARD_SIZE;
                y = rand() % BOARD_SIZE;
                dir = "UDLR"[rand() % 4];
            }

            for (int m = 0; m < ships[i].size; m++) {
                if (dir == 'U') {
                    board[y - m][x] = '|';
                }
                else if (dir == 'D') {
                    board[y + m][x] = '|';
                }
                else if (dir == 'L') {
                    board[y][x - m] = '-';
                }
                else if (dir == 'R') {
                    board[y][x + m] = '-';
                }
            }
        }
    }
}

int main() {
    srand(time(NULL));

    char board[BOARD_SIZE][BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }

    randomShips(board);

    printf("Board after placing ships:\n");
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }

    return 0;
}
