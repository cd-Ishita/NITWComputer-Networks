#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>
using namespace std;
int main()
{
	int fd;
	mkfifo("Hero",0666);
	char buffer[120];
	int count;
	fd=open("Hero",O_RDONLY);
	int fp=open("nani.cpp",O_WRONLY | O_CREAT);
	while((count=read(fd,buffer,120))>0)
	{
		string s(buffer);
		cout<<s.substr(0,count);
		write(fp,buffer,count);
	}
	close(fp);
	close(fd);
	string str = "g++ "; 
    	str = str + "nani.cpp" + " -o nani"; 
    	const char *command = str.c_str(); 
    	cout << "Compiling file using " << command << endl; 
    	system(command);
    	string s="./nani < input.txt > sysout1.txt";
    	const char *command2 = s.c_str();
    	cout << "\nRunning file "<<endl;; 
    	system(command2); 
    	int fd1=open("output.txt",O_RDONLY);
    	int fd2=open("sysout1.txt",O_RDONLY);
    	char ch1[2],ch2[2];
    	string flag="Your Output is Matching!!!";
    	while(read(fd1,ch1,1)&&read(fd2,ch2,1))
    	{
    		if(ch1[0]==ch2[0])
    		continue;
    		else
    		{
    			cout<<ch1<<" "<<ch2<<endl;
    			flag="Your Code is wrong!";
    			break;
    		}
    	}
    	mkfifo("code",0666);
    	fd=open("code",O_WRONLY);
    	char *buff=&flag[0];
    	strcmp(buff,flag.c_str());
    	write(fd,buff,flag.length());
    	cout<<flag<<endl;
    	close(fd);
	return 0;
}
