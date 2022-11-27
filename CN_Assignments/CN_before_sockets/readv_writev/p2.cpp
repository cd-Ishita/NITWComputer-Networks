#include<bits/stdc++.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<strings.h>
#include<sys/uio.h>
using namespace std;
int main()
{
        int fd;
        char myfifo=(char)"/tmp/myfifo";
        mkfifo(myfifo,0666);
        char buff[]={(char*)"hii\n",(char*)"nit\n",(char*)"warangal"};
        struct iovec iov[3];
        //while(1)
        //{
                fd=open(myfifo,O_WRONLY);
                cout<<fd<<endl;
                for(int i=0;i<3;i++)
                {
                        iov[i].iov_base=buff[i];
                        iov[i].iov_len=100;
                }
                writev(fd,iov,3);
        //}
//      close(fd);
}
