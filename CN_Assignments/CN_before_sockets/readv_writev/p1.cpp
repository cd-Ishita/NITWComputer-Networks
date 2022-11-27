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
        struct iovec iov[3];
        char buff1[100];
        char buff2[100];
        char buff3[100];
        iov[0].iov_base=buff1;
        iov[0].iov_len=sizeof(buff1);

         iov[1].iov_base=buff2;
        iov[1].iov_len=sizeof(buff2);

        iov[2].iov_base=buff3;
        iov[2].iov_len=sizeof(buff3);
        //while(1)
        //{
                fd=open(myfifo,O_RDONLY);
                cout<<fd<<endl;
                /*for(int i=0;i<3;i++)
                {
                        iovec[i].iov_base=buff[0];
                        iovec[i].iov_len=strlen(buff[i]);
                }*/
                readv(fd,iov,3);
                cout<<(char*)iov[0].iov_base<<endl;
                cout<<(char*)iov[1].iov_base<<endl;
                cout<<buff3<<endl;
        //}

        close(fd);
}
