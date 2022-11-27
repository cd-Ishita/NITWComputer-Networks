#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int fd=open("/dev/ig",O_RDWR,0666);
    printf("%d\n",fd);
    char buff[100],input[100],send[100];
    int attempts = 0;
    read(fd,&buff,100);
    int actual_number = atoi(buff);
    
    while(1){
        printf("Enter number you want to guess: \n");
        scanf("%s",input);
        
        int guess = atoi(input);
        
        //printf("Value = %d, input = %d", actual_number, guess);
        if(actual_number > guess){
            printf("Guess higher!\n");
        }
        else if(actual_number < guess){
            printf("Guess lower!\n");
        }
        else{
            printf("YOU HAVE GUESSED CORRECTLY!!\n Number was %d\n\n", guess);
            break;
        }
        attempts++;

        if(attempts > 10){
            printf("You have exceed number of attempts, the number was %d", actual_number);
            break;
        }
    }

    return 0;
}