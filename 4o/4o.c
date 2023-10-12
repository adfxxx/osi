#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

enum errors{
    success = 1,
    wrong_input = -1,
    not_open = -2,
    fail = -3
};

enum flags{
    xor8 = 1,
    xor32 = 2,
    mask = 3
};

int check_flag(char *flag);
int xor_8(FILE* file);
int xor_32(FILE* file);
int to_10_base(char **mask);
int mask_hex(FILE* file, char *mask);

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("Wrong amount of arguments");
        return wrong_input;
    }

    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("File is not open");
        fclose(file);
        return not_open;
    }

    if(check_flag(argv[2]) == fail){
        printf("Flag is wrong");
        return wrong_input;
    }
    int flag = check_flag(argv[2]);
    int result = 0;
    switch(flag){
        case xor8:
            result = xor_8(file);
            if (result == fail){
                printf("Error");
                return fail;
            }
            printf("Result of xor8: %d\n", result);
            fclose(file);
            return success;                   
        case xor32:
            result = xor_32(file);
            if (result == fail){
                printf("Error");
                return fail;
            }
            printf("Result of xor32: %d\n", result);
            fclose(file);
            return success;
        case mask:
            printf("Input a mask: ");
            char *mask = (char*)malloc(sizeof(char));
            int length = to_10_base(&mask); 
            if (length == fail || length == wrong_input){
                fclose(file);
                free(mask);
                return fail;
            }
            result = mask_hex(file, mask);
            printf("Result of mask <hex>: %d", result);
            fclose(file);
            free(mask);
            return success;        
    }

}

int check_flag(char *flag){
    if(!strcmp(flag, "xor8")){
        return xor8;
    }
    if(!strcmp(flag, "xor32")){
        return xor32;
    }
    if(!strcmp(flag, "mask")){
        return mask;
    }
    return fail;
}

int xor_8(FILE* file){
    unsigned char buff;
    int sum = 0;
    while(fread(&buff, sizeof(unsigned char), 1, file)){
        if (ferror(file)){
            return fail;
        }
        if (buff == ' '){
            continue;
        }
        sum = sum ^ buff;
    }
    return sum;
}

int xor_32(FILE* file){
    unsigned char buff [4] = {0};
    int sum = 0;
    int result = 0;
    int i = 0;
    int count = 0;
    while(!feof(file)){
        if (ferror(file)){
            return fail;
        }
        count = fread(&buff[i], sizeof(unsigned char), 1, file);
        if(count == 0){
            break;
        }
        if (buff[i] == ' '){
            continue;
        }
        if (i == 3){
            sum = buff[0] ^ buff[1] ^ buff[2] ^ buff[3];
            result = result ^ sum;
            i = 0;
        }
        else {i++;}
    }
    for (int j = i; j < 4; j++){
        buff[j] = 0;
    }
    sum = buff[0] ^ buff[1] ^ buff[2] ^ buff[3];
    result = result ^ sum;
    return result;
}

int to_10_base(char **mask){
    int i = 0;
    char symbol;
    int count = 0;
    while(scanf("%c", &symbol) && symbol != '\n'){
        if(!isdigit(symbol) && !isalpha(symbol)){
            printf("Wrong input");
            return wrong_input;
        }
        (*mask)[count] = symbol;
        count++;
        (*mask) = (char*)realloc(*mask, sizeof(char)*count);
        if(*mask == NULL){
            printf("Error");
            return fail;
        }
    }
    
    int length = strlen((*mask));
    int temp = 0;
    int number = 0;
    for(int i = 0; i < length; ++i){
        if(isdigit((*mask)[i])){
            temp = (*mask)[i] - '0';
            number += (temp*pow(16, length - i - 1));
        }
        else{
            temp = tolower((*mask)[i]) - 'a' + 10;
            number += (temp*pow(16, length - i - 1));
        }
    }
    sprintf(*mask, "%d", number);
    return length;
}

int mask_hex(FILE* file, char *mask){
    unsigned char buff [4] = {0};
    int size = 0;
    int count = 0;
    int i = 0;
    while(!feof(file)){
        if (ferror(file)){
            return fail;
        }
        size = fread(&buff[i], sizeof(unsigned char), 1, file);
        if(size == 0){
            break;
        }
        if (buff[i] == ' '){
            continue;
        }
        if (i == 3){
            if(buff[0] == mask[0] && buff[1] == mask[1] && buff[2] == mask[2] && buff[3] == mask[3]){
                count++;
            }
            i = 0;
        }
        else {i++;}
    }
    return count;
}