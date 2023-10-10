#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <math.h>

enum errors{
    success = 1,
    fail = -1,
    wrong_input = 0
};

typedef struct{
    char login[7];
    int pin_code;
    int sanct;
    int count_functions;
} user;

int check_login(char *str, int length);
void get_login(user **users, int *count);
int check_pin(char *input, int length);
void get_pin(user *users, int count);
void loggin(user **users, int *count, char **now_login);
int search(char *name, int pin, user *users, int count);\
int check_date_time(struct tm input);
int now_time();
int now_date();
int check_flag(char *flag);
int how_much_sec(struct tm input);
int how_much_min(struct tm input);
int how_much_hours(struct tm input);
int how_much_years(struct tm input);
int sanctions(user **users, int count);
int find_index(char *name, user *users, int count);

int main(){
    char state;
    user *users = NULL;
    char *now_login = NULL;
    int count = 0;
    loggin(&users, &count, &now_login);
    int function = 0;
    while (function != 6){
        printf("\nFunctions:\n 1. Time\n 2. Date\n 3. How much <time> flag\n 4. Logout\n 5. Sanctions username <number>\n 6. Exit\nInput a number: ");
        scanf("%d", &function);
        if (function == 1){
            int index = find_index(now_login, users, count);
            if(index != -1){
                if(users[index].sanct == 0 || users[index].sanct >= users[index].count_functions){
                    users[index].count_functions++;
                    if(now_time() == success){
                        continue;
                    }
                    else{break;}
                }
                else{ 
                    printf("Your limit is over. Good bye!");
                    loggin(&users, &count, &now_login);
                }
            }
        }
        if (function == 2){
            int index = find_index(now_login, users, count);
            if(index != -1){
                if(users[index].sanct == 0 || users[index].sanct > users[index].count_functions){
                    users[index].count_functions++;
                    if(now_date()){
                        continue;
                    }
                    else{break;}
                }
                else{ 
                    printf("Your limit is over. Good bye!");
                    loggin(&users, &count, &now_login);
                }
            }
        }
        if (function == 3){
            int index = find_index(now_login, users, count);
            if(index != -1){
                if(users[index].sanct == 0 || users[index].sanct > users[index].count_functions){
                    users[index].count_functions++;
                    printf("Input a date (day month year) and a flag(-s, -m, -h, -y) ");
                    struct tm input;
                    int result = 0; 
                    char flag[3];
                    do{
                        result = scanf("%d.%d.%d %d:%d:%d %s", &input.tm_mday, &input.tm_mon, &input.tm_year, &input.tm_hour, &input.tm_min, &input.tm_sec, flag);
                        if(result != 7 || check_date_time(input) == fail){
                            printf("Wrong date, try again: ");
                            while (getchar() != '\n');
                        }
                    }while(result != 7 || check_date_time(input) == fail);
                    while(!check_flag(flag)){
                        printf("Wrong flag");
                        scanf("%s", flag);
                    }
                    if (!strcmp(flag,"-s")){
                        input.tm_year -= 1900;
                        input.tm_mon--;
                        if(how_much_sec(input)){
                            continue;
                        }
                        else{break;}
                    }
                    if (!strcmp(flag,"-m")){
                        input.tm_year -= 1900;
                        input.tm_mon--;
                        if(how_much_min(input)){
                            continue;
                        }
                        else{break;}
                    }
                    if (!strcmp(flag,"-h")){
                        input.tm_year -= 1900;
                        input.tm_mon--;
                        if(how_much_hours(input)){
                            continue;
                        }
                        else{break;}
                    }
                    if (!strcmp(flag,"-y")){
                        input.tm_year -= 1900;
                        input.tm_mon--;
                        if(how_much_years(input)){
                            continue;
                        }
                        else{break;}
                    }
                }
                else{ 
                    printf("Your limit is over. Good bye!");
                    loggin(&users, &count, &now_login);
                }
            }
        }
        if (function == 4){
            free(now_login);
            now_login = NULL;
            loggin(&users, &count, &now_login);
        }
        if (function == 5){
            int index = find_index(now_login, users, count);
            if(index != -1){
                if(users[index].sanct == 0 || users[index].sanct > users[index].count_functions){
                    users[index].count_functions++;
                    if(sanctions(&users, count)){
                        printf("\nSuccess!");
                        continue;
                    }
                    else{
                        printf("\nSomething went wrong...");
                        continue;
                    }
                }
                else{ 
                    printf("Your limit is over. Good bye!");
                    loggin(&users, &count, &now_login);
                }
            }
        }
    }
    return 0;
}

int check_login(char *str, int length){
    for (int i = 0; i < length; i++){
        if(!isdigit(str[i])){
            if(!(str[i] >= 'a' && str[i] <= 'z')){
                if(!(str[i] >= 'A'&& str[i] <= 'Z')){
                    return fail;
                }
            }
        }
    }
    return success;
}

void get_login(user **users, int *count){
    printf("\nInput login (it should have <=6 symbols, digits or symbols of alphabet): ");
    char str[256];    
    scanf("%s", str);
    int length = strlen(str);
    if (length > 6 || length < 1){
        while(length < 1 || length > 6){
            printf("\nLogin should have <=6 symbols, try again: ");
            scanf("%s", str);
            length = strlen(str);
        }
    }
    if(!check_login(str, length)){
        printf("Wrong symbol in login, try again: ");
        while(!check_login(str, length)){
            scanf("%s", str);
            length = strlen(str);
        }
    }
    *users = realloc(*users, (*count + 1)*sizeof(user));
    (*users)[*count].sanct = 0;
    strcpy((*users)[*count].login,str);
    printf("\nGood login!");
    (*count)++;
}

int check_pin(char *input, int length){
    for (int i = 0; i < length; ++i){
        if (!(input[i] >= '0' && input[i] <= '9')){
            return fail;
        }
    }
    return success;
}

void get_pin(user *users, int count){
    printf("\nNow input your pin-code (0 <= number <= 100000): ");
    int pin = 0;
    int flag = 0;
    char input [256];
    int length = 0;
    while (flag == 0){
        scanf("%s", input);
        length = strlen(input);
        if (length == 0 || length > 6){
            printf("\nPin-code should be a number between 0 and 100000, try again: ");
            flag = 0;
            continue;
        }
        if(!check_pin(input, length)){
            printf("\nPin-code should be a number between 0 and 100000, try again: ");
            flag = 0;
            continue;
        }
        flag = 1;
    }
    pin = atoi(input);
    users[count - 1].pin_code = pin;
    printf("\nGood pin-code!");
}

int check_flag(char *flag){
    if (flag[0] == '-'){
        if(flag[1] == 's' || flag[1] == 'm' || flag[1] == 'h' || flag[1] == 'y'){
            return success;
        }
        else{return fail;}
    }
    else{return fail;}
}

void loggin(user **users, int *count, char **now_login){
    char state;
    if(*count == 0){
        printf("Choose right option:\n \t1. Sign in\n \t2. Log in\nInput a number (1 or 2): ");
        scanf(" %c", &state);
        getchar();
        while (state != '1'){
            printf ("\nWrong input. This is your first session, so you have to sign in (input 1)");
            printf("\nChoose right option:\n \t1. Sign in\n \t2. Log in\nInput a number (1 or 2): "); 
            scanf(" %c", &state);
        }
        get_login(users, count);
        get_pin(*users, *count);
    }
    printf("\nChoose right option:\n \t1. Sign in\n \t2. Log in\nInput a number (1 or 2): ");
    scanf(" %c", &state);
    while (state != '1' && state != '2'){
        printf ("Wrong input. Your input should be 1 or 2, try again: ");
        scanf(" %c", &state);
    }
    while (state == '1' || state == '2'){
        if (state == '1'){
            get_login(users, count);
            get_pin(*users, *count);  
            printf("\nChoose right option:\n \t1. Sign in\n \t2. Log in\nInput a number (1 or 2): ");
            scanf(" %c", &state);
            while (state != '1' && state != '2'){
                printf ("Wrong input. Your input should be 1 or 2, try again: ");
                scanf(" %c", &state);
            }
        }
        if(state == '2'){
            printf ("Input your login and your pin-code: ");
            char name[256];
            int pin;
            scanf("%s", name);
            getchar();
            scanf("%d", &pin);
            if(search(name, pin, *users, *count) == fail){
                printf("\nYour login or pin-code is not in the system. Try again or sign in");
                printf("\nChoose right option:\n \t1. Sign in\n \t2. Log in\nInput a number (1 or 2): ");
                scanf(" %c", &state);
                while (state != '1' && state != '2'){
                    printf ("Wrong input. Your input should be 1 or 2");
                    scanf(" %c", &state);
                }
            }
            else{
                printf("\nSuccess! Now you have an access to functions.");
                *now_login = malloc(strlen(name)+1);
                strcpy(*now_login, name);
                break;
            }
        } 
    }
}

int search(char *name, int pin, user *users, int count){
    for(int i = 0; i <= count; i++){
        if (!strcmp(name, users[i].login)){
            if (pin == users[i].pin_code){
                return success;
            }
        }
    }
    return fail;
}

int check_date_time(struct tm input){
    if(input.tm_year >= 0 && input.tm_year <= 2099){
        if(input.tm_mon >= 0 && input.tm_mon <= 12){
            if(input.tm_mday >= 1 && input.tm_mday <= 31){
                if(input.tm_hour >= 0 && input.tm_hour <= 23){
                    if(input.tm_min >= 0 && input.tm_min <= 59){
                        if(input.tm_sec >= 0 && input.tm_sec <= 59){
                            return success;
                        }
                    }
                }
            }
        }
    }
    return fail;
}

int now_time(){
    time_t now_time = time(NULL);
    struct tm *now = localtime(&now_time);
    printf("Time: %d:%d:%d\n", now->tm_hour, now->tm_min, now->tm_sec);
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return success;
    }
    else if (function == 2){
        return fail;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int now_date(){
    time_t now_time = time(NULL);
    struct tm *now = localtime(&now_time);  
    printf("Date: %d.%d.%d\n", now->tm_mday, now->tm_mon + 1, now->tm_year+1900);
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return 1;
    }
    else if (function == 2){
        return 0;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int how_much_sec(struct tm input){
    time_t now_time = time(NULL);
    printf("Difference in seconds: %.lf\n", difftime(now_time, mktime(&input)));
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return 1;
    }
    else if (function == 2){
        return 0;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int how_much_min(struct tm input){
    time_t now_time = time(NULL);
    printf("Difference in minutes: %.lf\n", difftime(now_time, mktime(&input))/60);
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return 1;
    }
    else if (function == 2){
        return 0;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int how_much_hours(struct tm input){
    time_t now_time = time(NULL);
    printf("Difference in hours: %.lf\n", difftime(now_time, mktime(&input))/3600);
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return 1;
    }
    else if (function == 2){
        return 0;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int how_much_years(struct tm input){
    time_t now_time = time(NULL);
    struct tm *now = localtime(&now_time);
    if(now->tm_year > input.tm_year){
        printf("Difference in years: %d\n", now->tm_year - input.tm_year);
    }
    else{
        printf("Difference in years: %d\n", input.tm_year - now->tm_year);
    }
    printf("You want to get back to menu? Choose a variant:\n 1. Back to menu\n 2. Finish program\nInput a number: ");
    int function = 0;
    scanf("%d", &function);
    if (function == 1){
        return 1;
    }
    else if (function == 2){
        return 0;
    }
    else {
        while (function != 1 && function != 2){
            printf("Wrong input, try again: "); 
            scanf("%d", &function);
        }
        if (function == 1){
            return success;
        }
        else if (function == 2){
            return fail;
        }
    }
}

int sanctions(user **users, int count){
    printf("Input user's login and number of functions for them: ");
    int number = 0 ;
    char name[256];
    int flag = 0;
    while (flag == 0){
        scanf("%s", name);
        int length = strlen(name);
        scanf("%d", &number);
        if(!check_login(name, length) || number <= 0){
            printf ("Wrong input, try again: ");
            continue;
        }
        flag = 1;
    }
    for (int i = 0; i <= count; i++){
        if (!strcmp(name, (*users)[i].login)){
            printf("\nAre you sure? If yes, printf a password: ");
            char password[5];
            scanf("%s", password);
            if(!strcmp(password, "12345")){
                (*users)[i].sanct = number-1;
                (*users)[i].count_functions = 0;
                return success;
            }
            else {printf("\nWrong password"); return fail;}
        }
    }
    printf("User is not in the system");
    return fail;
}

int find_index(char *name, user *users, int count){
    for (int i = 0; i <= count; i++){
        if(!strcmp(name, users[i].login)){
            return i;
        }
    }
    return fail;
}