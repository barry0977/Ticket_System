//
// Created by 24115 on 2024/5/25.
//

#ifndef CODE_TRAINSYSTEM_H
#define CODE_TRAINSYSTEM_H
#include "BPlusTree.h"
struct Train
{
    char trainID[25]{};
    int stationNum;
    mystr<35> stations[105];
    int seatNum;
    int prices[100];
    int startTime;//hour*60+minute
    int travelTimes[100];//单位:minute
    int stopoverTimes[100];//单位:minute
    int begindate,enddate;//从6月1日开始,6月1日为1，8月31日为92
    char type;
    int seatleft[100][100];//每趟列车的剩余票数
    bool isrelease;//记录是否发布
};

struct waitinf//记录需要候补的信息
{
    char username[25]{};
    char trainID[25]{};
    int date;
    char start[35]{};
    char end[35]{};
    int number;
};
class Traininf
{
private:
    BPTree<25, Train> trainlist;//以trainID为关键字，内容为Train的所有信息
    BPTree<25, mystr<25>> releasedtrain;//记录已经发布的车次
    BPTree<35, mystr<25>>stationlist;//关键字为车站名，内容为经过该车站的所有列车的trainID
    BPTree<25, waitinf>waitlist;//关键字为trainID,内容为候补的购票信息
public:
    void addtrain(char i[], int n, int m, char s[], char p[], char x[], char t[], char o[], char d[], char y)
    {

    }

    void deletetrain(char i[])
    {

    }

    void releasetrain(char i[])
    {

    }

    void querytrain(char i[],char d[])
    {

    }

    void queryticket(char s[],char t[],char d[],int order=0)//order代表排序的标准，0为
    {

    }

    void querytransfer(char s[],char t[],char d[],int order=0)
    {

    }

    void buyticket(char u[],char i[],char d[],int n,char f[],char t[],bool q)
    {

    }

    void refundticket(char i[],char d[],int n,char f[],char t[])
    {

    }
};
#endif //CODE_TRAINSYSTEM_H
