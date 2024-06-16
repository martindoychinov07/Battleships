#include "replay.h"

readInfo readStep(char* filename, int moveNumber){
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error opening file");
        exit(1);
    }

    int tmp;
    while(fscanf(f, "%d", &tmp)){
        if(tmp == moveNumber){
            fseek(f, 2, SEEK_CUR);
            readInfo rI;
            for(int i = 0; i < BOARD_SIZE; i++){
                for(int j = 0; j < BOARD_SIZE; j++){
                    fscanf(f, "%c", &rI.p1Board[i][j]);
                }
                fseek(f, 2 ,SEEK_CUR);
            }
            for(int i = 0; i < BOARD_SIZE; i++){
                for(int j = 0; j < BOARD_SIZE; j++){
                    fscanf(f, "%c", &rI.p2Board[i][j]);
                }
                fseek(f, 2 ,SEEK_CUR);
            }
            fclose(f);
            return rI;
        }else{
            fseek(f, 1 + (BOARD_SIZE + 2) * BOARD_SIZE * 2, SEEK_CUR);
        }
        
    }
    fclose(f);
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]){
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
            printf("%d ", i + 1);
        }
        printf("\n");

        for (int i = 0; i < BOARD_SIZE; i++) {
            printf("%d ", i + 1);
            for (int j = 0; j < BOARD_SIZE; j++) {
                printf("%c ", board[i][j]);
            }
            printf("\n");
        }
        printf("\n");
}

void playReplay(char* filename){
    int numberOfSteps, currentSelectedStep = 1;
    FILE *f = fopen(filename, "r");
    while(fscanf(f, "%d", &numberOfSteps) && !feof(f)){
        fseek(f, 1 + (BOARD_SIZE + 2) * BOARD_SIZE * 2, SEEK_CUR);
    }

    readInfo info;
    while(1){
        info = readStep(filename, currentSelectedStep);
        printf("\nShowing step %d out of %d\n", currentSelectedStep, numberOfSteps);
        printf("Player 1\n");
        printBoard(info.p1Board);
        printf("Player 2\n");
        printBoard(info.p2Board);

        printf("Select shown step (input invalid number to exit): ");
        scanf("%d", &currentSelectedStep);
        if(currentSelectedStep <= 0 || currentSelectedStep > numberOfSteps)break;
    }

    fclose(f);
}