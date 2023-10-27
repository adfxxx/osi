#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum errors{
    success = 1,
    wrong_input = -1,
    is_not_open = -2,
    fork_error = -3,
    memory_error = -4,
    fail = -5
};

void print(int state);
int search(char *name, char *line);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }
    char *f_n = argv[1];
    FILE *file_names = fopen(f_n, "r");
    if(file_names == NULL){
        print(is_not_open);
        return is_not_open;
    }

    char symbol;
    int length = 1;
    char *name = (char*)malloc(length*sizeof(char));
    if(name == NULL){
        fclose(file_names);
        print(memory_error);
        return memory_error;
    }
    int i = 0;
    int result = 0;
    char *line = "abc";
    char *temp_name;
    int flag = 0;
    while((symbol = fgetc(file_names)) != EOF){
        if(symbol != ' ' && symbol != '\t' && symbol != '\n'){
            name[i] = symbol;
            i++;
        }
        else{
            name[i] = '\0';
            i = 0;
            pid_t pid = fork();
            if(pid == -1){
                free(name);
                fclose(file_names);
                print(fork_error);
                return fork_error;
            }
            else if(pid == 0){
                result = search(name, line);
                if(result == is_not_open){
                    printf("File %s is not open\n", name);
                }
                else if (result != fail){
                    printf("Line is in string %d in %s\n", result, name);
                    flag = 1;
                }
                free(name);
                exit(EXIT_SUCCESS);
            }
            else{
                free(name);
                length = 1;
                name = (char*)malloc(length*sizeof(char));
                if(name == NULL){
                    fclose(file_names);
                    print(memory_error);
                    return memory_error;
                }
                i = 0;
            }
        }
        if(length <= i){
            length *= 2;
            temp_name = (char*)realloc(name, length*sizeof(char));
            if(temp_name == NULL){
                free(name);
                print(memory_error);
                return memory_error;
            }
            name = temp_name;
        }
    }
    if(flag = 0){
        printf("Line is not in files");
    }
    fclose(file_names);
    int status;
    while(wait(&status) > 0){}

    free(name);
    
    return success;
}

int search(char *name, char *line){
    FILE *file = fopen(name, "r");
    if(file == NULL){
        return is_not_open;
    }
    char *str = NULL;
    size_t str_length = 0; 
    int count = 0;
    while(getline(&str, &str_length, file) != -1){
        count++;
        if(strstr(str, line) != NULL){
            free(str);
            fclose(file);
            return count;
        }
    }
    free(str);
    fclose(file);
    return fail;
}

void print(int state){
    if(state == wrong_input){
        printf("Input is wrong");
    }
    else if(state == fork_error){
        printf("Error with fork()");
    }
    else if(state == memory_error){
        printf("Memory error");
    }
}