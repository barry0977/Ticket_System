//
// Created by 24115 on 2024/5/25.
//

#ifndef CODE_USERSYSTEM_H
#define CODE_USERSYSTEM_H
#include "mymap.h"
#include <cstring>
struct User
{
    char username[25]{};
    char password[35]{};
    char name[20]{};//一个汉字占三个字节
    char mailAdder[35]{};
    int privilege;

    ~User()=default;
    bool operator<(const User& obj)const
    {
        return strcmp(username,obj.username)<0;
    }
    bool operator>(const User& obj)const
    {
        return strcmp(username,obj.username)>0;
    }
    bool operator==(const User& obj)const
    {
        return strcmp(username,obj.username)==0;
    }

    User operator=(const User& obj)
    {
        strcpy(username,obj.username);
        strcpy(password,obj.password);
        strcpy(name,obj.name);
        strcpy(mailAdder,obj.mailAdder);
        privilege=obj.privilege;
        return *this;
    }
};

struct Order
{
    char trainID[25]{};
    long index;
    char start[35]{};
    char end[35]{};
    int lday,aday;//出发日期和到达日期
    int ltime,atime;//出发时间和到达时间
    int price;
    int number;
    int state;//订单状态，0-suceess,1-pending,2-refunded
    int timestamp;//下单时间戳
    int rank1,rank2;

    Order()=default;
    Order(char i[],char f[],char t[],int ld,int ad,int lt,int at,int p,int n,int s,int time,long ind,int r1,int r2)
    {
        strcpy(trainID,i);
        strcpy(start,f);
        strcpy(end,t);
        lday=ld;
        aday=ad;
        ltime=lt;
        atime=at;
        price=p;
        number=n;
        state=s;
        timestamp=time;
        index=ind;
        rank1=r1;
        rank2=r2;
    }

    bool operator<(const Order& obj)const
    {
        return timestamp>obj.timestamp;
    }
    bool operator>(const Order& obj)const
    {
        return timestamp<obj.timestamp;
    }
    bool operator==(const Order& obj)const
    {
        return timestamp==obj.timestamp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Order& order)
    {
        os<<order.trainID<<' '<<order.start<<' '<<dayshow(order.lday)<<' '<<timeshow(order.ltime)<<" -> "<<order.end<<' '<<dayshow(order.aday)<<' '<<timeshow(order.atime)<<' '<<order.price<<' '<<order.number<<std::endl;
        return os;
    }
};

extern sjtu::map<mystr<25>,User>userstack;//登录列表

class Userinf
{
    friend class Traininf;
private:
    BPTree<25,User>userlist;//以username为关键字，内容为User的所有信息
    BPTree<25,Order>orderlist;//以username为关键字，内容为订单的所有信息
public:
    Userinf()
    {
        userlist.initial("userinfo");
        orderlist.initial("orderinfo");
    }

    int adduser(char c[],char u[],char p[],char n[],char m[],int g)
    {
        User tmp;
        strcpy(tmp.username,u);
        strcpy(tmp.name,n);
        strcpy(tmp.password,p);
        strcpy(tmp.mailAdder,m);
        if(userlist.isfirst())//特别地，创建第一个用户时，新用户权限为 10，不受上述权限规则约束。具体来讲，当创建第一个用户时，忽略 -c 和 -g 参数，并认为新用户优先级为 10
        {
            tmp.privilege=10;
            userlist.Insert(u,tmp);
        }
        else
        {
            tmp.privilege=g;
            if(userlist.Findval(u).empty())
            {
                userlist.Insert(u,tmp);
            }
            else//如果 <username> 已经存在则注册失败
            {
                return -1;
            }
        }
        return 0;
    }

    int login(char u[],char p[])
    {
       if(userlist.Findval(u).empty())//用户不存在
       {
           return -1;
       }
       if(strcmp(userlist.Findval(u)[0].password,p)!=0)//密码错误
       {
           return -1;
       }
       if(userstack.count(mystr<25>(u))>0)//已经登录
       {
           return -1;
       }
       userstack[mystr<25>(u)]=userlist.Findval(u)[0];
        return 0;
    }

    int logout(char u[])
    {
        if(userstack.count(mystr<25>(u))==0)//未登录
        {
            return -1;
        }
        User ori=userlist.Findval(u)[0];
        userlist.Delete(u,ori);
        userlist.Insert(u,userstack[mystr<25>(u)]);
        userstack.erase(userstack.find(mystr<25>(u)));
        return 0;
    }

    void queryprofile(char c[],char u[])
    {
        if(userstack.count(mystr<25>(c))>0)//c已登录
        {
            if(strcmp(c,u)==0)//-c 和 -u 相同
            {
                User res=userstack[mystr<25>(u)];
                std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                return;
            }
            else
            {
                if(userstack.count(mystr<25>(u))>0)
                {
                    User res=userstack[mystr<25>(u)];
                    if(userstack[mystr<25>(c)].privilege>res.privilege)
                    {
                        std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                        return;
                    }
                }
                else
                {
                    if(!userlist.Findval(u).empty())
                    {
                        User res=userlist.Findval(u)[0];
                        if(userstack[mystr<25>(c)].privilege>res.privilege)
                        {
                            std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                            return;
                        }
                    }
                }
            }
        }
        std::cout<<"-1"<<std::endl;
        return;
    }

    void modifyprofile(char c[],char u[],char p[],char n[],char m[],int g)
    {
        std::string empty_str="";
        if(userstack.count(mystr<25>(c))>0)//c已登录
        {
            User cur=userstack[mystr<25>(c)];
            if(g<cur.privilege)//-g 需低于 -c 的权限
            {
                if(strcmp(c,u)==0)//-c 和 -u 相同
                {
                    User& res=userstack[mystr<25>(u)];
                    if(strcmp(p,empty_str.c_str())!=0)
                    {
                        strcpy(res.password,p);
                    }
                    if(strcmp(n,empty_str.c_str())!=0)
                    {
                        strcpy(res.name,n);
                    }
                    if(strcmp(m,empty_str.c_str())!=0)
                    {
                        strcpy(res.mailAdder,m);
                    }
                    if(g>=0)
                    {
                        res.privilege=g;
                    }
                    std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                    return;
                }
                else
                {
                    if(userstack.count(mystr<25>(u))>0)
                    {
                        User& res=userstack[mystr<25>(u)];
                        if(userstack[mystr<25>(c)].privilege>res.privilege)
                        {
                            if(strcmp(p,empty_str.c_str())!=0)
                            {
                                strcpy(res.password,p);
                            }
                            if(strcmp(n,empty_str.c_str())!=0)
                            {
                                strcpy(res.name,n);
                            }
                            if(strcmp(m,empty_str.c_str())!=0)
                            {
                                strcpy(res.mailAdder,m);
                            }
                            if(g>=0)
                            {
                                res.privilege=g;
                            }
                            std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                            return;
                        }
                    }
                    else
                    {
                        if(!userlist.Findval(u).empty())
                        {
                            User res=userlist.Findval(u)[0];
                            User ori=res;
                            if(userstack[mystr<25>(c)].privilege>res.privilege)
                            {
                                if(strcmp(p,empty_str.c_str())!=0)
                                {
                                    strcpy(res.password,p);
                                }
                                if(strcmp(n,empty_str.c_str())!=0)
                                {
                                    strcpy(res.name,n);
                                }
                                if(strcmp(m,empty_str.c_str())!=0)
                                {
                                    strcpy(res.mailAdder,m);
                                }
                                if(g>=0)
                                {
                                    res.privilege=g;
                                }
                                std::cout<<res.username<<" "<<res.name<<" "<<res.mailAdder<<" "<<res.privilege<<std::endl;
                                userlist.Delete(u,ori);
                                userlist.Insert(u,res);
                                return;
                            }
                        }
                    }
                }
            }
        }
        std::cout<<"-1"<<std::endl;
        return;
    }

    void modifyrest()
    {
        for(auto it=userstack.begin();it!=userstack.end();it++)
        {
            char curname[25];
            strcpy(curname,it->first.value);
            User ori=userlist.Findval(curname)[0];
            userlist.Delete(curname,ori);
            userlist.Insert(curname,it->second);
        }
    }

    void queryorder(char u[])
    {
        if(userstack.count(mystr<25>(u))>0)//-u已登录
        {
            std::vector<Order> orders=orderlist.Findval(u);
            std::cout<<orders.size()<<std::endl;
            for(int i=0;i<orders.size();i++)
            {
                if(orders[i].state==0)//success
                {
                    std::cout<<"[success] "<<orders[i];
                }
                else if(orders[i].state==1)//pending
                {
                    std::cout<<"[pending] "<<orders[i];
                }
                else//refunded
                {
                    std::cout<<"[refunded] "<<orders[i];
                }
            }
            return;
        }
        std::cout<<"-1\n";
    }

    void refund(char u[],int n=1)
    {

    }
};
#endif //CODE_USERSYSTEM_H
