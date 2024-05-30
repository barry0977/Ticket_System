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
    bool isrelease;//记录是否发布
};

struct Ticket//一趟列车的车票信息
{
    char trainID[25]{};
    int day;//几号发车
    int ticketleft[100];//所有站的车票信息
    int seatNum;
    int stationNum;

    bool operator<(const Ticket& obj)const
    {
        if(strcmp(trainID,obj.trainID)!=0)
        {
            return strcmp(trainID, obj.trainID) < 0;
        }
        else
        {
            return day<obj.day;
        }
    }
};

struct stationinf
{
    char trainID[25]{};
    int day;//几号发车
    int order;//在该列车中是第几站
    int addr;//Ticket结构踢在文件中地址
    int arrivetime,leavetime;//到达时间，离开时间

    bool operator<(const Ticket& obj)const
    {
        if(strcmp(trainID,obj.trainID)!=0)
        {
            return strcmp(trainID, obj.trainID) < 0;
        }
        else
        {
            return day<obj.day;
        }
    }
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
    std::fstream trainfile;//储存火车信息的文件
    std::fstream ticketfile;//储存车票信息的文件
    BPTree<25, int> trainlist;//以trainID为关键字，内容为Train的信息在文件中的地址
    BPTree<25, int> releasedtrain;//记录已经发布的车次
    BPTree<35, stationinf>stationlist;//关键字为车站名，内容为经过该车站的所有列车的trainID
    BPTree<25, waitinf>waitlist;//关键字为trainID,内容为候补的购票信息

    void ini()//给记录详细火车票信息的文件初始化
    {
        trainfile.open("detailtrain");
        if (!trainfile.good())
        {
            trainfile.open("detailtrain", std::ios::out | std::ios::binary);
            trainfile.close();
            trainfile.open("detailtrain");
        }
        ticketfile.open("detailticket");
        if(!ticketfile.good())
        {
            ticketfile.open("detailticket",std::ios::out | std::ios::binary);
            ticketfile.close();
            ticketfile.open("detailticket");
        }
    }
    int atrain(Train& tmp)//在文件尾部添加新信息
    {
        trainfile.seekp(0, std::ios::end);
        int index = trainfile.tellp();
        trainfile.write(reinterpret_cast<char*>(&tmp), sizeof(Train));
        return index;
    }
    void rtrain(Train& tmp,int index)//读出文件
    {
        trainfile.seekg(index, std::ios::beg);
        trainfile.read(reinterpret_cast<char*>(&tmp), sizeof(Train));
    }
    void wtrain(Train& tmp,int index)//写入文件
    {
        trainfile.seekp(index, std::ios::beg);
        trainfile.write(reinterpret_cast<char*>(&tmp), sizeof(Train));
    }

    int aticket(Ticket& tmp)//在文件尾部添加新信息
    {
        ticketfile.seekp(0, std::ios::end);
        int index = ticketfile.tellp();
        ticketfile.write(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
        return index;
    }
    void rticket(Ticket& tmp,int index)//读出文件
    {
        ticketfile.seekg(index, std::ios::beg);
        ticketfile.read(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
    }
    void wticket(Ticket& tmp,int index)//写入文件
    {
        ticketfile.seekp(index, std::ios::beg);
        ticketfile.write(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
    }
public:
    Traininf()
    {
        ini();
        trainlist.initial("traininfo");
        releasedtrain.initial("releaseinfo");

    }

    ~Traininf()
    {
        trainfile.close();
    }

    int addtrain(char i[], int n, int m,std::string s,std::string p, std::string x, std::string t, std::string o,std::string d, char y)
    {
        if(trainlist.Findval(i).empty())//trainID不存在
        {
            Train tmp;
            strcpy(tmp.trainID,i);
            tmp.stationNum=n;
            tmp.seatNum=m;
            std::vector<std::string> _s= curstring(s),_p= curstring(p),_t= curstring(t),_o= curstring(o),_d= curstring(d);
            for(int i=0;i<n;i++)
            {
                tmp.stations[i]=mystr<35>(_s[i]);
            }
            tmp.startTime= starttime(x);
            for(int i=0;i<n-1;i++)
            {
                tmp.prices[i]= stringToInteger(_p[i]);
            }
            tmp.begindate= daytrans(_d[0]);
            tmp.enddate= daytrans(_d[1]);
            if(n>2)
            {
                for(int i=0;i<n-1;i++)
                {
                    tmp.stopoverTimes[i]= stringToInteger(_o[i]);
                }
            }
            tmp.type=y;
            int index= atrain(tmp);
            trainlist.Insert(i,index);
            return 0;
        }
        return -1;
    }

    int deletetrain(char i[])
    {
        if(releasedtrain.Findval(i).empty())//未发布
        {
            sjtu::vector<int>num=trainlist.Findval(i);
            if(!num.empty())
            {
                trainlist.Delete(i,num[0]);
                return 0;
            }
        }
        return -1;
    }

    int releasetrain(char i[])
    {
        if(releasedtrain.Findval(i).empty())
        {
            sjtu::vector<int>num=trainlist.Findval(i);
            if(!num.empty())//找到列车
            {
                releasedtrain.Insert(i,num[0]);
                Train res;//读出火车信息
                rtrain(res,num[0]);
                Ticket tmp;
                strcpy(tmp.trainID,res.trainID);
                tmp.stationNum=res.stationNum;
                tmp.seatNum=res.seatNum;
                for(int i=0;i<res.stationNum-1;i++)
                {
                    tmp.ticketleft[i]=tmp.seatNum;
                }
                for(int i=res.begindate;i<=res.enddate;i++)
                {
                    tmp.day=i;
                    int index=aticket(tmp);

                }
            }
        }
        return -1;
    }

    void querytrain(char i[],char d[])
    {
        Train tmp;
        if(!trainlist.Findval(i).empty())
        {
            int index=trainlist.Findval(i)[0];
            rtrain(tmp,index);
            std::cout<<tmp.trainID<<" "<<tmp.type<<std::endl;

        }
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
