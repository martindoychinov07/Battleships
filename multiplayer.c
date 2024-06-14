#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    int board[BOARD_SIZE][BOARD_SIZE];
    char display[BOARD_SIZE][BOARD_SIZE]; 
    char own_display[BOARD_SIZE][BOARD_SIZE]; 
} Player;

void initialize_board(Player *player);
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
bool place_ship(Player *player, int size, char orientation, Coordinate start);
bool is_valid_position(Player *player, int size, char orientation, Coordinate start);
bool contains(Ship ship, Coordinate location);
bool destroyed(Player *player, Ship *ship);
void fill_board(Player *player, Ship ship);
void load_configuration(Player *player, const char *filename);
void manual_configuration(Player *player);
void play_game(Player *player1, Player *player2);
bool player_turn(Player *current, Player *opponent);
void edit_ship(Player *player);
void view_board(Player *player, bool during_configuration);
bool all_ships_destroyed(Player *player);

int main() {
    Player player1, player2;
    initialize_board(&player1);
    initialize_board(&player2);

    char choice;
    printf("Player 1: Do you want to load a configuration from a file? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y') {
        char filename[100];
        printf("Enter filename: ");
        scanf("%s", filename);
        load_configuration(&player1, filename);
    } else {
        manual_configuration(&player1);
    }

    printf("Player 2: Do you want to load a configuration from a file? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y') {
        char filename[100];
        printf("Enter filename: ");
        scanf("%s", filename);
        load_configuration(&player2, filename);
    } else {
        manual_configuration(&player2);
    }

    play_game(&player1, &player2);
    return 0;
}

void initialize_board(Player *player) {
    player->num_ships = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            player->board[i][j] = 0;
            player->display[i][j] = 'O';
            player->own_display[i][j] = 'O';
        }
    }
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n  ");
    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int r = 0; r < BOARD_SIZE; r++) {
        printf("%d ", r + 1);
        for (int c = 0; c < BOARD_SIZE; c++) {
            printf("%c ", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

bool is_valid_position(Player *player, int size, char orientation, Coordinate start) {
    if (orientation == 'H') {
        if (start.col + size > BOARD_SIZE) {
            printf("Error: Ship exceeds board boundaries horizontally.\n");
            return false;
        }
        for (int i = 0; i < size; i++) {
            if (player->board[start.row][start.col + i] != 0) {
                printf("Error: Ship overlaps with another ship at (%d, %d).\n", start.row + 1, start.col + i + 1);
                return false;
            }
        }
    } else if (orientation == 'V') {
        if (start.row + size > BOARD_SIZE) {
            printf("Error: Ship exceeds board boundaries vertically.\n");
            return false;
        }
        for (int i = 0; i < size; i++) {
            if (player->board[start.row + i][start.col] != 0) {
                printf("Error: Ship overlaps with another ship at (%d, %d).\n", start.row + i + 1, start.col + 1);
                return false;
            }
        }
    } else {
        printf("Error: Invalid orientation '%c'.\n", orientation);
        return false;
    }
    return true;
}

bool place_ship(Player *player, int size, char orientation, Coordinate start) {
    if (!is_valid_position(player, size, orientation, start)) {
        return false;
    }

    Ship ship;
    ship.size = size;
    ship.orientation = orientation;
    ship.coordinates = malloc(size * sizeof(Coordinate));
    ship.destroyed = false;

    if (orientation == 'H') {
        for (int i = 0; i < size; i++) {
            ship.coordinates[i] = (Coordinate){start.row, start.col + i};
        }
    } else {
        for (int i = 0; i < size; i++) {
            ship.coordinates[i] = (Coordinate){start.row + i, start.col};
        }
    }

    player->ships[player->num_ships++] = ship;
    fill_board(player, ship);
    return true;
}

void fill_board(Player *player, Ship ship) {
    for (int i = 0; i < ship.size; i++) {
        player->board[ship.coordinates[i].row][ship.coordinates[i].col] = 1;
        player->own_display[ship.coordinates[i].row][ship.coordinates[i].col] = 'S';
    }
}

void load_configuration(Player *player, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file");
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int size;
        char orientation;
        Coordinate start;
        if (sscanf(line, "%d %c %d %d", &size, &orientation, &start.row, &start.col) == 4) {
            start.row--; 
            start.col--; 
            printf("Placing ship of size %d, orientation %c, at (%d, %d)\n", size, orientation, start.row + 1, start.col + 1);
            if (!place_ship(player, size, orientation, start)) {
                fprintf(stderr, "Error placing ship from file at (%d, %d) with size %d and orientation %c.\n", start.row + 1, start.col + 1, size, orientation);
                exit(1);
            }
        } else {
            fprintf(stderr, "Invalid line format: %s", line);
        }
    }

    fclose(file);
}

void manual_configuration(Player *player) {
    int sizes[NUM_SHIPS] = {2, 2, 2, 2, 3, 3, 3, 4, 4, 6};
    char orientation;
    Coordinate start;
    int placed_ships = 0;

    while (placed_ships < NUM_SHIPS) {
        int option;
        printf("Options:\n");
        printf("1. Place next ship\n");
        printf("2. Edit the position of one of your ships\n");
        printf("3. View the current board\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Place ship of size %d\n", sizes[placed_ships]);
                printf("Enter orientation (H/V): ");
                scanf(" %c", &orientation);
                printf("Enter start row and column: ");
                scanf("%d %d", &start.row, &start.col);
                start.row--;
                start.col--;

                if (place_ship(player, sizes[placed_ships], orientation, start)) {
                    placed_ships++;
                } else {
                    printf("Invalid position. Try again.\n");
                }
                break;
            case 2:
                edit_ship(player);
                break;
            case 3:
                view_board(player, true); 
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}

void play_game(Player *player1, Player *player2) {
    Player *current_player = player1;
    Player *opponent = player2;

    while (true) {
        bool hit = true;

        if (all_ships_destroyed(opponent)) {
            printf("Player %d wins!\n", (current_player == player1) ? 1 : 2);
            break;
        }

        while (hit) {
            printf("Player %d's turn:\n", (current_player == player1) ? 1 : 2);
            hit = player_turn(current_player, opponent);
            view_board(current_player, false);
        }

        Player *temp = current_player;
        current_player = opponent;
        opponent = temp;
    }
}

bool player_turn(Player *current, Player *opponent) {
    Coordinate guess;
    bool valid_guess = false;

    while (!valid_guess) {
        printf("Enter guess (row and column): ");
        scanf("%d %d", &guess.row, &guess.col);
        guess.row--;
        guess.col--;
        if (guess.row >= 0 && guess.row < BOARD_SIZE && guess.col >= 0 && guess.col < BOARD_SIZE) {
            valid_guess = true;
        } else {
            printf("Invalid coordinates. Try again.\n");
        }
    }

    if (opponent->board[guess.row][guess.col] == 1) {
        printf("Hit!\n");
        current->display[guess.row][guess.col] = 'X';
        opponent->own_display[guess.row][guess.col] = 'X';
        opponent->board[guess.row][guess.col] = 0;

        for (int i = 0; i < opponent->num_ships; i++) {
            if (contains(opponent->ships[i], guess)) {
                if (destroyed(opponent, &opponent->ships[i])) {
                    printf("Ship destroyed!\n");
                }
                break;
            }
        }
        return true; 
    } else {
        printf("Miss.\n");
        current->display[guess.row][guess.col] = '*';
        opponent->own_display[guess.row][guess.col] = '*';
        return false; 
    }
}

void edit_ship(Player *player) {
    view_board(player, true); 
    int ship_index;
    printf("Enter index of the ship you want to edit (1 to %d): ", player->num_ships);
    scanf("%d", &ship_index);
    ship_index--; 

    if (ship_index < 0 || ship_index >= player->num_ships) {
        printf("Invalid ship index.\n");
        return;
    }

    Ship *ship = &player->ships[ship_index];
    int size = ship->size;
    char orientation = ship->orientation;
    Coordinate start = ship->coordinates[0];

    for (int i = 0; i < size; i++) {
        player->board[ship->coordinates[i].row][ship->coordinates[i].col] = 0;
        player->own_display[ship->coordinates[i].row][ship->coordinates[i].col] = 'O';
    }

    char new_orientation;
    Coordinate new_start;
    printf("Editing ship of size %d\n", size);
    printf("Enter new orientation (H/V): ");
    scanf(" %c", &new_orientation);
    printf("Enter new start row and column: ");
    scanf("%d %d", &new_start.row, &new_start.col);
    new_start.row--;
    new_start.col--;

    if (place_ship(player, size, new_orientation, new_start)) {
        ship->orientation = new_orientation;
        ship->coordinates = malloc(size * sizeof(Coordinate));
        if (new_orientation == 'H') {
            for (int i = 0; i < size; i++) {
                ship->coordinates[i] = (Coordinate){new_start.row, new_start.col + i};
            }
        } else {
            for (int i = 0; i < size; i++) {
                ship->coordinates[i] = (Coordinate){new_start.row + i, new_start.col};
            }
        }
    } else {
        place_ship(player, size, orientation, start);
    }
}

void view_board(Player *player, bool during_configuration) {
    printf("Player's current board:\n");
    print_board(player->own_display);

    if (!during_configuration) {
        printf("Opponent's board:\n");
        print_board(player->display);
    }
}

bool contains(Ship ship, Coordinate location) {
    for (int i = 0; i < ship.size; i++) {
        if (ship.coordinates[i].row == location.row && ship.coordinates[i].col == location.col) {
            return true;
        }
    }
    return false;
}

bool destroyed(Player *player, Ship *ship) {
    if (ship->destroyed) {
        return false;
    }
    for (int i = 0; i < ship->size; i++) {
        if (player->own_display[ship->coordinates[i].row][ship->coordinates[i].col] != 'X') {
            return false;
        }
    }
    ship->destroyed = true;
    return true;
}

bool all_ships_destroyed(Player *player) {
    for (int i = 0; i < player->num_ships; i++) {
        if (!player->ships[i].destroyed) {
            return false;
        }
    }
    return true;
}

