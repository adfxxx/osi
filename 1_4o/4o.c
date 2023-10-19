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
int xor_32(FILE* file, unsigned char *result_4);
int mask_hex(FILE* file, int mask);

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
        fclose(file);
        return wrong_input;
    }
    int flag = check_flag(argv[2]);
    int result = 0;
    unsigned char result_4[4] = {0};
    int mask_hx = 0x37333732;
    switch(flag){
        case xor8:
            result = xor_8(file);
            if (result == fail){
                printf("Error");
                fclose(file);
                return fail;
            }
            printf("Result of xor8: %d\n", result);
            fclose(file);
            return success;                   
        case xor32:
            result = xor_32(file, result_4);
            if (result == fail){
                printf("Error");
                fclose(file);
                return fail;
            }
            printf("Result of xor32: ");
            for (int i = 0; i < 4; i++){
                printf("%u ", result_4[i]);
            }
            fclose(file);
            return success;
        case mask:
            result = mask_hex(file, mask_hx);
            printf("Result of mask <hex>: %d", result);
            fclose(file);
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
        if (buff == 0){
            continue;
        }
        sum = sum ^ buff;
    }
    return sum;
}

int xor_32(FILE* file, unsigned char *result){
    unsigned char buff [4] = {0};
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
            for (int j = 0; j < 4; j++){
                result[j] ^= buff[j];
            }
            i = 0;
        }
        else {i++;}
    }
    for (int j = i; j < 4; j++){
        buff[j] = 0;
    }
    for (int j = 0; j < 4; j++){
        result[j] ^= buff[j];
    }
    return success;
}

int mask_hex(FILE* file, int mask){
    unsigned char buff [4] = {0};
    int count = 0;
    while(fread(buff, sizeof(unsigned char), 4, file) == 4){
        if(!memcmp(buff, &mask, 4)){
            count++;
        }
    }
    return count;
}