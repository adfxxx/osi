#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum errors{
    bin = 3,
    txt = 2, 
    success = 1,
    wrong_input = -1,
    not_open = -2,
    error = -3,
};

int copy(FILE *file, FILE *out_file);
int check_file(char *name, int length);

int main (int argc, char *argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments");
        return wrong_input;
    }
    int ext = 0;
    int length = strlen(argv[1]);
    ext = check_file(argv[1], length);
    FILE *file_txt;
    FILE *out_file_txt;
    FILE *file_bin;
    FILE *out_file_bin;  
    file_txt = fopen(argv[1], "rb");
    out_file_txt = fopen(argv[2], "wb");
    if(!file_txt || !out_file_txt){
        printf("File is not open");
        return not_open;
    }
    if(copy(file_txt, out_file_txt)){
        printf("Success");
    }
    fclose(file_txt);
    fclose(out_file_txt);
    return 0;
    // switch (ext){
    //     case txt:
    //         file_txt = fopen(argv[1], "r");
    //         out_file_txt = fopen(argv[2], "w");
    //         if(!file_txt || !out_file_txt){
    //             printf("File is not open");
    //             return not_open;
    //         }
    //         if(copy(file_txt, out_file_txt)){
    //             printf("Success");
    //         }
    //         fclose(file_txt);
    //         fclose(out_file_txt);
    //         return 0;
    //     case bin:
    //         file_bin = fopen(argv[1], "rb");
    //         out_file_bin = fopen(argv[2], "wb");
    //         if(!file_bin || !out_file_bin){
    //             printf("File is not open");
    //             return not_open;
    //         }
    //         if(copy(file_bin, out_file_bin)){
    //             printf("Success");
    //         }
    //         fclose(file_bin);
    //         fclose(out_file_bin);
    //         return 0;
    // }
}

int check_file(char *name, int length){
    char ext[4];
    int j = 0;
    for (int i = length - 4; i < length; i++){
        ext[j] = name[i];
        j++;
    }
    if (strcmp(ext, ".txt")){
        return txt;
    }
    if (strcmp(ext, ".bin")){
        return bin;
    }
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