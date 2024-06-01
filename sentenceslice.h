//
// Created by 24115 on 2024/5/26.
//

#ifndef CODE_SENTENCESLICE_H
#define CODE_SENTENCESLICE_H
#include <sstream>
#include <iostream>
#include "myvector.h"
#include <vector>
std::vector<std::string> readtokens(const std::string& str)//把读入的语句按空格切片
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token)
    {
        result.push_back(token);
    }
    return result;
}

std::vector<std::string> cutstring(const std::string& str)//把字符串按照|切片
{
    std::vector<std::string> subStrings;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, '|'))
    {
        subStrings.push_back(token);
    }
    return subStrings;
}
std::string integerToString(int n)
{
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

int stringToInteger(std::string str)
{
    int n=0;
    int a=1;
    for(int i=str.length()-1;i>=0;i--)
    {
        n+=(str[i]-'0')*a;
        a*=10;
    }
    return n;
}

int timetrans(std::string str)
{
    int h,m;
    h= stringToInteger(str.substr(0,2));
    m= stringToInteger(str.substr(3,2));
    return h*60+m;
}

int daytrans(std::string str)
{
    int ans=0;
    if(str[1]=='6')
    {
        ans+=(str[3]-'0')*10+str[4]-'0';
    }
    else if(str[1]=='7')
    {
        ans+=30;
        ans+=(str[3]-'0')*10+str[4]-'0';
    }
    else if(str[1]=='8')
    {
        ans+=61;
        ans+=(str[3]-'0')*10+str[4]-'0';
    }
    else if(str[1]=='9')
    {
        ans+=92;
        ans+=(str[3]-'0')*10+str[4]-'0';
    }
    else
    {
        ans=0;
    }
    return ans;
}

std::string dayshow(int d)
{
    std::string s="";
    if(d>=1&&d<=30)
    {
        int x=d/10,y=d%10;
        s+="06-";
        s+='0'+x;
        s+='0'+y;
    }
    else if(d>=31&&d<=61)
    {
        d-=30;
        int x=d/10,y=d%10;
        s+="07-";
        s+='0'+x;
        s+='0'+y;
    }
    else if(d>=62&&d<=92)
    {
        d-=61;
        int x=d/10,y=d%10;
        s+="08-";
        s+='0'+x;
        s+='0'+y;
    }
    else
    {
        d-=92;
        int x=d/10,y=d%10;
        s+="09-";
        s+='0'+x;
        s+='0'+y;
    }
    return s;
}

std::string timeshow(int t)
{
    std::string s="";
    int h=t/60,m=t%60;
    s+='0'+h/10;
    s+='0'+h%10;
    s+=':';
    s+='0'+m/10;
    s+='0'+m%10;
    return s;
}

int stamp(std::string& s)//读取时间戳内时间
{
    std::string tmp=s.substr(1,s.size()-2);
    return stringToInteger(tmp);
}
#endif //CODE_SENTENCESLICE_H
