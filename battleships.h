#ifndef BATTLESHIP_H
#define BATTLESHIP_H

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
} Ship;

typedef struct {
    int num_ships;
    Ship ships[NUM_SHIPS];
    int board[BOARD_SIZE][BOARD_SIZE];
    char display[BOARD_SIZE][BOARD_SIZE];
} Player;

void initialize_board(Player *player);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
bool place_ship(Player *player, int size, char orientation, Coordinate start);
bool is_valid_position(Player *player, int size, char orientation, Coordinate start);
bool contains(Ship ship, Coordinate location);
bool destroyed(Player *player, Ship ship);
void fill_board(Player *player, Ship ship);
void load_configuration(Player *player, const char *filename);
void manual_configuration(Player *player);
void play_game(Player *player1, Player *player2);
bool player_turn(Player *current, Player *opponent);
void edit_ship(Player *player);
void view_board(Player *player);

#endif
