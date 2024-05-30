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

std::vector<std::string> curstring(const std::string& str)//把字符串按照|切片
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

int starttime(std::string& str)
{
    int h,m;
    h= stringToInteger(str.substr(0,2));
    m= stringToInteger(str.substr(2,2));
    return h*60+m;
}

int daytrans(std::string& str)
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
    else
    {
        ans+=61;
        ans+=(str[3]-'0')*10+str[4]-'0';
    }
    return ans;
}
#endif //CODE_SENTENCESLICE_H
