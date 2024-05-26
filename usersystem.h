//
// Created by 24115 on 2024/5/25.
//

#ifndef CODE_USERSYSTEM_H
#define CODE_USERSYSTEM_H

struct User
{
    char username[25]{};
    char password[35]{};
    char name[20]{};//一个汉字占三个字节
    char mailAdder[35]{};
    int privilege;
};

struct Order
{
    char trainID[25]{};
    char start[35]{};
    char end[35]{};
    int lday,aday;//出发日期和到达日期
    int ltime,atime;//出发时间和到达时间
    int price;
    int number;
    int state;//订单时间戳
    int timestamp;//下单时间戳
};

class Userinf
{
private:
    BPTree<25,User>userlist;//以username为关键字，内容为User的所有信息
    BPTree<25,Order>orderlist;//以username为关键字，内容为订单的所有信息
public:
    Userinf()
    {
        userlist.initial("userinfo");
    }

    void adduser(char c[],char u[],char p[],char n[],char m[],int g)
    {

    }

    void login(char u[],char p[])
    {

    }

    void logout(char u[])
    {

    }

    void queryprofile(char c[],char u[])
    {

    }

    void modifyprofile(char c[],char u[],char p[],char n[],char m[],int g)
    {

    }

    void queryorder(char u[])
    {

    }

    void refund(char u[],int n=1)
    {

    }
};
#endif //CODE_USERSYSTEM_H
