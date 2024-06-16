#include "encryption.h"

void encrypt(char *input, char* password){
    FILE *f = fopen(input, "r");
    FILE *temp = fopen("temp.txt", "w");

    int ceaserShift = 0;

    for(int i=0;i<password[i]!='\0';i++){
        ceaserShift+=password[i];
    }

    size_t i = 0;
    char ch;

    while((ch=fgetc(f))!=EOF){
        ch = (ch + ceaserShift)%256;
        ch = ch ^ password[i%strlen(password)];
        fputc(ch, temp);
        i++;
    }

    fclose(f);
    fclose(temp);
    
    f = fopen(input, "w");
    temp = fopen("temp.txt", "r");

    while((ch=fgetc(temp))!=EOF){
        fputc(ch, f);
    }

    fclose(f);
    fclose(temp);

    remove("temp.txt");

}

void decrypt(char *input, char* password){
    FILE *f = fopen(input, "r");
    FILE *temp = fopen("temp.txt", "w");

    int ceaserShift = 0;

    for(int i=0;i<password[i]!='\0';i++){
        ceaserShift+=password[i];
    }

    size_t i = 0;
    char ch;

    while((ch=fgetc(f))!=EOF){
        ch = ch ^ password[i%strlen(password)];
        ch = (ch - ceaserShift + 256)%256; 
        fputc(ch, temp);
        i++;
    }

    fclose(f);
    fclose(temp);
    
    f = fopen(input, "w");
    temp = fopen("temp.txt", "r");

    while((ch=fgetc(temp))!=EOF){
        fputc(ch, f);
    }

    fclose(f);
    fclose(temp);

    remove("temp.txt");
}
