#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum errors{
    success = 1,
    wrong_input = -1,
    not_open = -2,
    error = -3
};

int copy(FILE *file, FILE *out_file);

int main (int argc, char *argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments");
        return wrong_input;
    }
    FILE *file = fopen(argv[1], "r");
    FILE *out_file = fopen(argv[2], "w");
    if(!file || !out_file){
        printf("File is not open");
        return not_open;
    }
    if(copy(file, out_file)){
        printf("Success");
    }
    fclose(file);
    fclose(out_file);
    return 0;
}

int copy(FILE *file, FILE *out_file){
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char symbol;
    for(int i = 0; i < length; i++){
        symbol = fgetc(file);
        if(symbol != EOF){
            fprintf(out_file, "%c", symbol);
        }
        else {break;}
    }
    return success;
}