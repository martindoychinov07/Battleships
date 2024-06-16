#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 10

typedef struct {
    int row;
    int col;
} Coordinate;

typedef struct {
    int size;
    char orientation;
    Coordinate *coordinates;
    bool destroyed;
} Ship;

typedef struct {
    int num_ships;
    Ship ships[NUM_SHIPS];
    char board[BOARD_SIZE][BOARD_SIZE];
    char display[BOARD_SIZE][BOARD_SIZE]; 
    char own_display[BOARD_SIZE][BOARD_SIZE]; 
} Player;

struct ShipArr {
    int size;
    int count;
};

typedef struct ShipArr ShipArr;

extern ShipArr shipsArr[];

void initialize_board(Player *player);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
bool place_ship(Player *player, int size, char orientation, Coordinate start);
bool is_valid_position(Player *player, int size, char orientation, Coordinate start);
bool contains(Ship ship, Coordinate location);
bool destroyed(Player *player, Ship *ship);
void fill_board(Player *player, Ship ship);
void load_configuration(Player *player, const char *filename);
void manual_configuration(Player *player);
void play_game(Player *player1, Player *player2, char* replayFile);
void play_game_vs_bot(Player *player, Player *bot, char* replayFile);
bool player_turn(Player *current, Player *opponent);
void edit_ship(Player *player);
void view_board(Player *player, bool during_configuration);
bool all_ships_destroyed(Player *player);
bool botAttack(char playerBoard[BOARD_SIZE][BOARD_SIZE]);
int validPos(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);
int validArea(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int size, char dir);
void randomShips(char board[BOARD_SIZE][BOARD_SIZE]);

char* initializeReplay();
void recordStep(char* file, Player* p1, Player* p2, int stepNumber);