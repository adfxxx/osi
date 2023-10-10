#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors{
    success = 1,
    wrong_input = -1,
    not_open = -2,
    error = -3
};

void print(FILE *file);

int main (int argc, char *argv[]){
    if (argc != 2){
        printf("Input = file");
        return wrong_input;
    }
    FILE *file;
    file = fopen(argv[1], "wb");
    if (!file){
        printf("\nFile is not open");
        return not_open;
    }

    unsigned char numbers[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int size = 0;
    size = fwrite(numbers, sizeof(unsigned char), 11, file);
    if(size < 11){
        printf("\nError");
        return error;
    }
    fclose(file);

    file = fopen(argv[1], "rb");
    if (!file){
        printf("File is not open");
        return not_open;
    }
    unsigned char buff;
    while(fread(&buff, sizeof(unsigned char), 1, file)){
        printf("%u ", buff);
        print(file);
    }
    fclose(file);
    file = fopen(argv[1], "rb");
    fseek(file, 3, SEEK_SET);
    unsigned char buff2[4];
    fread(buff2, sizeof(unsigned char), 4, file);
    for (int i = 0; i < 4; ++i){
        printf("%d ", buff2[i]);
    }
    fclose(file);
    return 0;
}

void print(FILE *file){
    char *base_ptr = file->_base;
    int length = strlen(base_ptr);
    printf("size of buff %d, ", file->_bufsiz);
    printf("charbuff %d, ", file->_charbuf);
    printf("cnt %d, ", file->_cnt);
    printf("descriptor %d, ", file->_file);
    printf("flag %d, ", file->_flag);
    printf("pointer %u\n", (*file->_ptr));
}