#include "game.h"
#include "replay.h"
#include "encryption.h"

int main() {
    srand(time(NULL));
    char mode;

    printf("To play multiplayer type(m)\nTo play singleplayer type(s)\nTo view replay type(r)\n");
    scanf("%c", &mode);

    while(
        mode != 'm'&&
        mode != 'M' &&
        mode !='s' &&
        mode !='S' &&
        mode !='r' &&
        mode!='R') {
        printf("Wrong choice! Please try again!");
        printf("To play multiplayer type(m)\nTo play singleplayer type(s)\nTo view replay type(r)\n");
        scanf("%c", &mode);
    }

    if(mode == 'm' || mode == 'M') {
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

        char replayFile[100];
        memcpy(replayFile, initializeReplay(), sizeof(replayFile));

        play_game(&player1, &player2, replayFile);
    }
    else if(mode == 's' || mode == 'S') {
        Player player, bot;

        initialize_board(&player);
        initialize_board(&bot);

        char choice;
        printf("Player: Do you want to load a configuration from a file? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y') {
            char filename[100];
            printf("Enter filename: ");
            scanf("%s", filename);
            load_configuration(&player, filename);
        } else {
            manual_configuration(&player);
        }

        randomShips(bot.own_display);

        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                bot.board[i][j] = bot.own_display[i][j];
            }
        }

        char replayFile[100];
        memcpy(replayFile, initializeReplay(), sizeof(replayFile));

        play_game_vs_bot(&player, &bot, replayFile);
    }else{
        char filename[100];
        char password[100];
        printf("Enter file name: ");
        scanf("%s", filename);
        printf("Enter password: ");
        scanf("%s", password);
        decrypt(filename, password);
        playReplay(filename);
        encrypt(filename, password);
    }

    return 0;
}