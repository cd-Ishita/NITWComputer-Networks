#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include<string.h>
void *readthrd(void *arg)
{
int *a=(int *)arg;
char b1[100];
scanf("%s",b1);
write(*a,b1,100);
}
void *writethrd(void *arg)
{
int *a=(int *)arg;
char b1[100];
read(*a,b1,100);
printf("%s\n",b1);
}
int main()
{
pthread_t p1,p2,c1,c2;
char b1[100],b2[100];
int A[2],B[2],d;
pipe(A);
pipe(B);
d=fork();
if(d>0)
{
close(A[0]);
close(B[1]);
int t=3;
while(t--)
{
printf("enter b1 value in parent:");
pthread_create(&p1,NULL,readthrd,&A[1]);
pthread_join(p1,NULL);
printf("recieved b2 value in parent:");
pthread_create(&p2,NULL,writethrd,&B[0]);
pthread_join(p2,NULL);

}
}
else
{
close(B[0]);
close(A[1]);
int t=3;
while(t--)
{
printf("recieved b1 value in child:");
pthread_create(&c1,NULL,writethrd,&A[0]);
pthread_join(c1,NULL);
printf("enter b2 value in child:");
pthread_create(&c2,NULL,readthrd,&B[1]);
pthread_join(c2,NULL);
}
}

return 0;
}
