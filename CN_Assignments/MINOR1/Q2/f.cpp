#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include<bits/stdc++.h>
#include <semaphore.h>
using namespace std;

const char* sem_1="s1"; 
const char* sem_4="f";
const char* sem_2="s2"; 
const char* sem_3="s3";
const char* sem_5="s4";


int main()
{
	   
        	sem_unlink(sem_1);
        	sem_unlink(sem_2);
        	sem_unlink(sem_3);
        	sem_unlink(sem_4);
        	sem_unlink(sem_5);
        	
	sem_t *s1 = sem_open(sem_1,O_CREAT|O_EXCL,0666,0);
	sem_t *s = sem_open(sem_4,O_CREAT|O_EXCL,0666,0);
	sem_t *s2 = sem_open(sem_2,O_CREAT|O_EXCL,0666,0);
	sem_t *s3 = sem_open(sem_3,O_CREAT|O_EXCL,0666,0);
	sem_t *s4 = sem_open(sem_5,O_CREAT|O_EXCL,0666,0);
	
	
	sem_wait(s);
	sem_post(s2);
	sem_wait(s);
	sem_post(s3);
	sem_wait(s);
	sem_post(s4);
	sem_wait(s);
	sem_post(s1);
	sem_wait(s);
	sem_post(s1);
	sem_post(s2);
	sem_post(s3);
	sem_post(s4);
	
	

}
