#include<bits/stdc++.h>
using namespace std;
char hamcode(int pos,int len,string temp)
{
    int i=pos-1;
    int cnt=0;
    while(i<len)
    {
        for(int j=i;j<i+pos;j++)
        {
            if(temp[j]=='1'&&j!=pos-1)
            cnt++;
        }
        i=i+ (2*pos);
    }
    //cout<<cnt<<endl;
    if(cnt%2==0)
    return '0';
    else
    return '1';
}
void check(string s)
{
    int i=sqrt(s.length());
    while(s.length()>(pow(2,i)-(i+1)))
    i++;
    int cnt=i;
    string temp;
    int j=0,k=0;
    for(int i=0;i<s.length()+cnt;i++)
    {
        if(i==pow(2,k)-1)
        {
            temp+='0';
            k++;
        }
        else
        {
            temp+=s[j];
            j++;
        }
    }
    for(int i=0;i<cnt;i++)
    {
        int pos=pow(2,i);
        temp[pos-1]=hamcode(pos,s.length()+cnt,temp);
        //cout<<pos<<" "<<temp[pos-1]<<endl;
    }
    cout<<"the hamming bit stream is : "<<temp<<endl;
}
void validate(string s,string t)
{
    int cnt=sqrt(t.length())+1;
    int sum=0;
    //cout<<cnt<<" "<<t.length()<<endl;
    for(int i=0;i<cnt;i++)
    {
        int pos=pow(2,i);
        if(hamcode(pos,t.length(),t)==t[pos-1])
        continue;
        else
        sum+=pos;
        //cout<<pos<<" "<<t[pos-1]<<" "<<hamcode(pos,t.length(),t)<<endl;
    }
    if(sum==0)
    cout<<"No error"<<endl;
    else
    cout<<"Error at "<<sum<<" position"<<endl;
}
int main()
{
    string s;
    cout<<"enter the bit stream : ";
    cin>>s;
    check(s);
    string t;
    cout<<"enter reveciver side bit stream : ";
    cin>>t;
    validate(s,t);
}
