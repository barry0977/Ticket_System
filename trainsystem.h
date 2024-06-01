//
// Created by 24115 on 2024/5/25.
//

#ifndef CODE_TRAINSYSTEM_H
#define CODE_TRAINSYSTEM_H
#include "BPlusTree.h"
#include "mysort.h"
#include "usersystem.h"

extern sjtu::map<mystr<25>,User>userstack;//登录列表
extern Userinf userinf;
struct Train
{
    char trainID[25]{};
    int stationNum;
    mystr<35> stations[105];
    int seatNum;
    int prices[105]{};
    int startTime;//hour*60+minute
    int travelTimes[105]{};//单位:minute
    int stopoverTimes[105]{};//单位:minute
    int begindate,enddate;//从6月1日开始,6月1日为1，8月31日为92
    char type;
};

struct Queryans//Query_ticket
{
    char trainID[25]{};
    char from[35]{},to[35]{};
    int fday,tday,ftime,ttime;
    int time;
    int price;
    int seat;
};

struct Transferans
{
    char train1ID[25]{},train2ID[25]{};
    char from[35]{},mid[35]{},to[35]{};
    int fday1,tday1,ftime1,ttime1;
    int fday2,tday2,ftime2,ttime2;
    int time;
    int price1,price2;
    int seat1,seat2;

    Transferans()=default;
    Transferans(char t1id[],char t2id[],char f[],char m[],char t[],int fd1,int td1,int ft1,int tt1,int fd2,int td2,int ft2,int tt2,int ti,int p1,int p2,int s1,int s2)
    {
        strcpy(train1ID,t1id);
        strcpy(train2ID,t2id);
        strcpy(from,f);
        strcpy(mid,m);
        strcpy(to,t);
        fday1=fd1;
        tday1=td1;
        ftime1=ft1;
        ttime1=tt1;
        fday2=fd2;
        tday2=td2;
        ftime2=ft2;
        ttime2=tt2;
        time=ti;
        price1=p1;
        price2=p2;
        seat1=s1;
        seat2=s2;
    }

    Transferans& operator=(const Transferans& obj)
    {
        if(this==&obj)
        {
            return *this;
        }
        strcpy(train1ID,obj.train1ID);
        strcpy(train2ID,obj.train2ID);
        strcpy(from,obj.from);
        strcpy(mid,obj.mid);
        strcpy(to,obj.to);
        fday1=obj.fday1;
        tday1=obj.tday1;
        ftime1=obj.ftime1;
        ttime1=obj.ttime1;
        fday2=obj.fday2;
        tday2=obj.tday2;
        ftime2=obj.ftime2;
        ttime2=obj.ttime2;
        time=obj.time;
        price1=obj.price1;
        price2=obj.price2;
        seat1=obj.seat1;
        seat2=obj.seat2;
    }
};

bool comptime(const Queryans& a, const Queryans& b)
{
    if(a.time!=b.time)
    {
        return a.time<b.time;
    }
    else
    {
        return strcmp(a.trainID,b.trainID)<0;
    }
}

bool compcost(const Queryans& a,const Queryans& b)
{
    if(a.price!=b.price)
    {
        return a.price<b.price;
    }
    else
    {
        return strcmp(a.trainID,b.trainID)<0;
    }
}

bool Comptime(const Transferans& a,const Transferans& b)
{
    if(a.time!=b.time)
    {
        return a.time<b.time;
    }
    else if((a.price1+a.price2)!=(b.price1+b.price2))
    {
        return (a.price1+a.price2)<(b.price1+b.price2);
    }
    else if(strcmp(a.train1ID,b.train1ID)!=0)
    {
        return strcmp(a.train1ID,b.train1ID)<0;
    }
    else
    {
        return strcmp(a.train2ID,b.train2ID)<0;
    }
}


bool Compcost(const Transferans& a,const Transferans& b)
{
    if((a.price1+a.price2)!=(b.price1+b.price2))
    {
        return (a.price1+a.price2)<(b.price1+b.price2);
    }
    else if(a.time!=b.time)
    {
        return a.time<b.time;
    }
    else if(strcmp(a.train1ID,b.train1ID)!=0)
    {
        return strcmp(a.train1ID,b.train1ID)<0;
    }
    else
    {
        return strcmp(a.train2ID,b.train2ID)<0;
    }
}

struct Ticket//一趟列车的车票信息，每个日期存一个
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
    int order;//在该列车中是第几站
    int cost;//从出发站到这一站的费用
    int begindate,enddate;//该列车售票日期
    int arrivetime,leavetime;//该站到达时间，离开时间

    bool operator<(const stationinf& obj)const
    {
        return strcmp(trainID, obj.trainID) < 0;
    }
    bool operator>(const stationinf& obj)const
    {
        return strcmp(trainID,obj.trainID) > 0;
    }
    stationinf()=default;
    stationinf(const stationinf& obj)
    {
        strcpy(trainID,obj.trainID);
        order=obj.order;
        cost=obj.cost;
        begindate=obj.begindate;
        enddate=obj.enddate;
        arrivetime=obj.arrivetime;
        leavetime=obj.leavetime;
    }
    stationinf operator=(const stationinf& obj)
    {
        strcpy(trainID,obj.trainID);
        order=obj.order;
        cost=obj.cost;
        begindate=obj.begindate;
        enddate=obj.enddate;
        arrivetime=obj.arrivetime;
        leavetime=obj.leavetime;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const stationinf& obj)
    {
        os << obj.trainID<< '\n';
        return os;
    }
};
struct waitinf//记录需要候补的信息
{
    char username[25]{};
    char trainID[25]{};
    long index;
    int rank1,rank2;//f和t是第几站
    int number;
    int timestamp;//下单时间

    waitinf()=default;
    waitinf(char u[],char i[],long ind,int r1,int r2,int n,int t)
    {
        strcpy(username,u);
        strcpy(trainID,i);
        index=ind;
        rank1=r1;
        rank2=r2;
        number=n;
        timestamp=t;
    }

    bool operator<(const waitinf& obj)const
    {
        return timestamp<obj.timestamp;
    }
    bool operator==(const waitinf& obj)const
    {
        return timestamp==obj.timestamp;
    }
};

class Traininf
{
    friend class Userinf;
private:
    std::fstream trainfile;//储存火车信息的文件
    std::fstream ticketfile;//储存车票信息的文件
    BPTree<25, long> trainlist;//以trainID为关键字，内容为Train的信息在文件中的地址
    BPTree<25, long> ticketlist;//以trainID为关键字，内容为一趟列车的车票信息在文件中地址
    BPTree<25, long> releasedtrain;//记录已经发布的车次
    BPTree<35, stationinf>stationlist;//关键字为车站名，内容为经过该车站的列车的基本信息
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
    long atrain(Train& tmp)//在文件尾部添加新信息
    {
        trainfile.seekp(0, std::ios::end);
        long index = trainfile.tellp();
        trainfile.write(reinterpret_cast<char*>(&tmp), sizeof(Train));
        return index;
    }
    void rtrain(Train& tmp,long index)//读出文件
    {
        trainfile.seekg(index, std::ios::beg);
        trainfile.read(reinterpret_cast<char*>(&tmp), sizeof(Train));
    }
    void wtrain(Train& tmp,long index)//写入文件
    {
        trainfile.seekp(index, std::ios::beg);
        trainfile.write(reinterpret_cast<char*>(&tmp), sizeof(Train));
    }

    long aticket(Ticket& tmp)//在文件尾部添加新信息
    {
        ticketfile.seekp(0, std::ios::end);
        long index = ticketfile.tellp();
        ticketfile.write(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
        return index;
    }
    void rticket(Ticket& tmp,long index)//读出文件
    {
        ticketfile.seekg(index, std::ios::beg);
        ticketfile.read(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
    }
    void wticket(Ticket& tmp,long index)//写入文件
    {
        ticketfile.seekp(index, std::ios::beg);
        ticketfile.write(reinterpret_cast<char*>(&tmp), sizeof(Ticket));
    }
public:
    Traininf()
    {
        ini();
        trainlist.initial("traininfo");
        ticketlist.initial("ticketinfo");
        releasedtrain.initial("releaseinfo");
        stationlist.initial("stationinfo");
        waitlist.initial("waitinfo");
    }

    ~Traininf()
    {
        trainfile.close();
    }

    int addtrain(char i[], int n, int m,std::string& s,std::string& p, std::string& x, std::string& t, std::string& o,std::string& d, char y)
    {
        if(trainlist.Findval(i).empty())//trainID不存在
        {
            Train tmp;
            strcpy(tmp.trainID,i);
            tmp.stationNum=n;
            tmp.seatNum=m;
            vector<std::string> _s= cutstring(s),_p= cutstring(p),_t= cutstring(t),_o= cutstring(o),_d= cutstring(d);
            for(int j=0;j<n;j++)
            {
                tmp.stations[j]=mystr<35>(_s[j]);
            }
            tmp.startTime= timetrans(x);
            for(int j=0;j<n-1;j++)
            {
                tmp.prices[j]= stringToInteger(_p[j]);
                tmp.travelTimes[j]= stringToInteger(_t[j]);
            }
            tmp.begindate= daytrans(_d[0]);
            tmp.enddate= daytrans(_d[1]);
            if(n>2)
            {
                for(int j=0;j<n-2;j++)
                {
                    tmp.stopoverTimes[j]= stringToInteger(_o[j]);
                }
            }
            tmp.type=y;
            long index= atrain(tmp);
            trainlist.Insert(i,index);
            return 0;
        }
        return -1;
    }

    int deletetrain(char i[])
    {
        if(releasedtrain.Findval(i).empty())//未发布
        {
            vector<long>num=trainlist.Findval(i);
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
            vector<long>num=trainlist.Findval(i);
            if(!num.empty())//找到列车
            {
                releasedtrain.Insert(i,num[0]);
                Train res;//读出火车信息
                rtrain(res,num[0]);
                Ticket tmp;
                strcpy(tmp.trainID,res.trainID);
                tmp.stationNum=res.stationNum;
                tmp.seatNum=res.seatNum;
                for(int j=0;j<res.stationNum-1;j++)
                {
                    tmp.ticketleft[j]=tmp.seatNum;
                }
                for(int j=res.begindate;j<=res.enddate;j++)
                {
                    tmp.day=j;
                    int index=aticket(tmp);
                    ticketlist.Insert(tmp.trainID,index);//将车票信息储存
                }
                stationinf stmp;
                strcpy(stmp.trainID,res.trainID);
                stmp.begindate=res.begindate;
                stmp.enddate=res.enddate;
                int cost=0,at=res.startTime,lt=at;
                for(int j=0;j<res.stationNum;j++)
                {
                    stmp.order=j;
                    stmp.cost=cost;
                    stmp.arrivetime=at;
                    stmp.leavetime=lt;
                    char station[25];
                    strcpy(station,res.stations[j].value);
                    stationlist.Insert(station,stmp);
                    cost+=res.prices[j];
                    at=lt+res.travelTimes[j];
                    lt=at+res.stopoverTimes[j];
                }
                return 0;
            }
        }
        return -1;
    }

    void querytrain(char i[],std::string& d)
    {
        Train res;
        int date= daytrans(d);
        if(!trainlist.Findval(i).empty())//列车存在
        {
            int index=trainlist.Findval(i)[0];
            rtrain(res,index);
            if(date>=res.begindate&&date<=res.enddate)//在售票日期
            {
                if(releasedtrain.Findval(i).empty())//如果还未发布
                {
                    std::cout<<res.trainID<<" "<<res.type<<std::endl;
                    int cost=0,at=res.startTime,lt=at;
                    for(int j=0;j<res.stationNum;j++)
                    {
                        std::cout<<res.stations[j]<<" ";
                        if(j==0)
                        {
                            std::cout<<"xx-xx xx:xx"<<" ";
                        }
                        else
                        {
                            std::cout<<dayshow(date+at/1440)<<" "<<timeshow(at%1440)<<" ";
                        }
                        std::cout<<"-> ";
                        if(j==res.stationNum-1)
                        {
                            std::cout<<"xx-xx xx:xx"<<" "<<cost<<" "<<"x"<<std::endl;
                        }
                        else
                        {
                            std::cout<<dayshow(date+lt/1440)<<" "<<timeshow(lt%1440)<<" "<<cost<<" "<<res.seatNum<<std::endl;
                        }
                        cost+=res.prices[j];
                        at=lt+res.travelTimes[j];
                        lt=at+res.stopoverTimes[j];
                    }
                }
                else//已经发布
                {
                    std::cout<<res.trainID<<" "<<res.type<<std::endl;
                    int order=date-res.begindate;
                    long addr=ticketlist.Findval(i)[order];//存放车票信息的地址
                    Ticket ans;
                    rticket(ans,addr);
                    int cost=0,at=res.startTime,lt=at;
                    for(int j=0;j<res.stationNum;j++)
                    {
                        std::cout<<res.stations[j]<<" ";
                        if(j==0)
                        {
                            std::cout<<"xx-xx xx:xx"<<" ";
                        }
                        else
                        {
                            std::cout<<dayshow(date+at/1440)<<" "<<timeshow(at%1440)<<" ";
                        }
                        std::cout<<"-> ";
                        if(j==res.stationNum-1)
                        {
                            std::cout<<"xx-xx xx:xx"<<" "<<cost<<" "<<"x"<<std::endl;
                        }
                        else
                        {
                            std::cout<<dayshow(date+lt/1440)<<" "<<timeshow(lt%1440)<<" "<<cost<<" "<<ans.ticketleft[j]<<std::endl;
                        }
                        cost+=res.prices[j];
                        at=lt+res.travelTimes[j];
                        lt=at+res.stopoverTimes[j];
                    }
                }
                return;
            }
        }
        std::cout<<"-1\n";
        return;
    }

    void queryticket(char s[],char t[],std::string& d,int order=0)//order代表排序的标准
    {
        int num=0;
        vector<Queryans>querylist;
        int date= daytrans(d);
        vector<stationinf>train1=stationlist.Findval(s);//经过-s的列车
        vector<stationinf>train2=stationlist.Findval(t);//经过-t的列车
        int l1=train1.size(),l2=train2.size();
        int begin=0;
        for(int i=0;i<l1;i++)
        {
            if(date>=(train1[i].begindate+train1[i].leavetime/1440)&&date<=(train1[i].enddate+train1[i].leavetime/1440))//该车在—d会售票
            {
                for(int j=begin;j<l2;j++)
                {
//                    if(train2[j]<train1[i])
//                    {
//                        begin=j;
//                    }
//                    if(train2[j]>train1[i])
//                    {
//                        continue;
//                    }
                    if(strcmp(train1[i].trainID,train2[j].trainID)==0&&train1[i].order<train2[j].order)//如果找到,并且先后次序正确 其中train1[i]为该车在出发站信息，train2[j]为该车在到达站信息
                    {
                        num++;
                        Queryans tmp;
                        char trainID[25]{};
                        strcpy(trainID,train1[i].trainID);
                        strcpy(tmp.trainID,trainID);
                        strcpy(tmp.from,s);
                        strcpy(tmp.to,t);
                        int place=date-train1[i].leavetime/1440-train1[i].begindate;//日期的次序,即起点站出发的日期减去开始售票日期
                        Ticket res;
                        rticket(res,ticketlist.Findval(trainID)[place]);
                        tmp.price=train2[j].cost-train1[i].cost;//票价
                        tmp.time=train2[j].arrivetime-train1[i].leavetime;//时间
                        int seat=res.seatNum;
                        for(int k=train1[i].order;k<train2[j].order;k++)
                        {
                            seat=std::min(seat,res.ticketleft[k]);
                        }
                        tmp.seat=seat;//最大剩余座位
                        tmp.fday=date;//出发日
                        tmp.tday=date-train1[i].leavetime/1440+train2[j].arrivetime/1440;//到达日
                        tmp.ftime=train1[i].leavetime%1440;//出发时间
                        tmp.ttime=train2[j].arrivetime%1440;//到达时间
                        querylist.push_back(tmp);
                    }
                }
            }
        }
        if(order==0)//按时间排序
        {
            mergesort<Queryans>(querylist,0,querylist.size()-1,comptime);
        }
        else//按费用排序
        {
            mergesort<Queryans>(querylist,0,querylist.size()-1,compcost);
        }
        std::cout<<num<<std::endl;
        for(int i=0;i<num;i++)
        {
            Queryans tmp=querylist[i];
            std::cout<<tmp.trainID<<" "<<tmp.from<<" "<<dayshow(tmp.fday)<<" "<<timeshow(tmp.ftime)<<" -> "<<tmp.to<<" "<<dayshow(tmp.tday)<<" "<<timeshow(tmp.ttime)<<" "<<tmp.price<<" "<<tmp.seat<<std::endl;
        }
    }

    void querytransfer(char s[],char t[],std::string& d,int order=0)
    {
        bool find=false;//是否找到
        Transferans res;
        int date= daytrans(d);
        vector<stationinf>train1=stationlist.Findval(s);//经过-s的列车
        vector<stationinf>train2=stationlist.Findval(t);//经过-t的列车
        int l1=train1.size(),l2=train2.size();
        for(int i=0;i<l1;i++)
        {
            int day1=train1[i].begindate+train1[i].leavetime/1440;
            int day2=train1[i].enddate+train1[i].leavetime/1440;
            if(date>=day1&&date<=day2)//该车在—d会售票
            {
                for(int j=0;j<l2;j++)
                {
                    if(strcmp(train1[i].trainID,train2[j].trainID)!=0)//不能换乘同一辆车
                    {
                        Train t1,t2;//两辆火车信息
                        rtrain(t1,trainlist.Findval(train1[i].trainID)[0]);
                        rtrain(t2,trainlist.Findval(train2[j].trainID)[0]);
                        int at1,lt1=train1[i].leavetime,c1=0;
                        for(int k=train1[i].order+1;k<t1.stationNum;k++)
                        {
                            at1=lt1+t1.travelTimes[k-1];//1车到站时间 #
                            lt1=at1+t1.stopoverTimes[k-1];//1车离站时间
                            c1+=t1.prices[k-1];
//                            int At2[100],Lt2[100],C2[100];
//                            At2[train2[j].order]=train2[j].arrivetime;
//                            C2[train2[j].order]=0;
//                            for(int l=train2[j].order-1;l>=0;l--)
//                            {
//                                Lt2[l]=At2[l+1]-t2.travelTimes[l];//2车离站时间#
//                                if(l>=1) At2[l]=Lt2[l]-t2.stopoverTimes[l-1];//2车到站时间
//                                C2[l]=C2[l+1]+t2.prices[l];
//                            }
                            int at2=train2[j].arrivetime,lt2,c2=0;
                            for(int l=train2[j].order-1;l>=0;l--)
                            {
                                lt2=at2-t2.travelTimes[l];//2车离站时间#
                                if(l>=1) at2=lt2-t2.stopoverTimes[l-1];//2车到站时间
                                c2=c2+t2.prices[l];
                                if(t1.stations[k]==t2.stations[l])//找到共同的中间站
                                {
//                                    int lt2=Lt2[l],c2=C2[l];
                                    //计算1车到中转站的日期
                                    int middate=date-train1[i].leavetime/1440+at1/1440;
                                    if((at1%1440)>(lt2%1440))//如果当天1车到，2车已经离开
                                    {
                                        middate++;
                                    }
                                    int sday2=middate-lt2/1440;//2车最早出发日期
                                    if(sday2<=t2.enddate)//2车会售票，说明找到(不一定正好处于售票区间，可以更早)
                                    {
                                        int fday1=date;
                                        int tday1=date-train1[i].leavetime/1440+at1/1440;
                                        int ftime1=train1[i].leavetime%1440;
                                        int ttime1=at1%1440;
                                        int fday2=(sday2>=t2.begindate?sday2:t2.begindate)+lt2/1440;
                                        int tday2=(sday2>=t2.begindate?sday2:t2.begindate)+train2[j].arrivetime/1440;
                                        int ftime2=lt2%1440;
                                        int ttime2=train2[j].arrivetime%1440;
                                        int time=(tday2-fday1)*1440+ttime2-ftime1;
                                        Ticket inf1,inf2;
                                        int place1=date-train1[i].leavetime/1440-t1.begindate,place2=(sday2>=t2.begindate?sday2:t2.begindate)-t2.begindate;
                                        long add1=ticketlist.Findval(t1.trainID)[place1],add2=ticketlist.Findval(t2.trainID)[place2];
                                        rticket(inf1,add1);
                                        rticket(inf2,add2);
                                        int seat1=t1.seatNum,seat2=t2.seatNum;
                                        for(int a=train1[i].order;a<k;a++)
                                        {
                                            seat1=std::min(seat1,inf1.ticketleft[a]);
                                        }
                                        for(int a=l;a<train2[j].order;a++)
                                        {
                                            seat2=std::min(seat2,inf2.ticketleft[a]);
                                        }
                                        Transferans tmp(t1.trainID,t2.trainID,s,t1.stations[k].value,t,fday1,tday1,ftime1,ttime1,fday2,tday2,ftime2,ttime2,time,c1,c2,seat1,seat2);
                                        if(!find)//如果之前还没找到过
                                        {
                                            res=tmp;
                                            find=1;
                                        }
                                        else
                                        {
                                            if(order==0)//按时间排序
                                            {
                                                if(Comptime(tmp,res))
                                                {
                                                    res=tmp;
                                                }
                                            }
                                            else
                                            {
                                                if(Compcost(tmp,res))
                                                {
                                                    res=tmp;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if(!find)
        {
            std::cout<<0<<std::endl;
        }
        else
        {
            std::cout<<res.train1ID<<" "<<res.from<<" "<<dayshow(res.fday1)<<" "<<timeshow(res.ftime1)<<" -> "<<res.mid<<" "<<dayshow(res.tday1)<<" "<<timeshow(res.ttime1)<<" "<<res.price1<<" "<<res.seat1<<std::endl;
            std::cout<<res.train2ID<<" "<<res.mid<<" "<<dayshow(res.fday2)<<" "<<timeshow(res.ftime2)<<" -> "<<res.to<<" "<<dayshow(res.tday2)<<" "<<timeshow(res.ttime2)<<" "<<res.price2<<" "<<res.seat2<<std::endl;
        }
    }

    void buyticket(char u[],char i[],std::string& d,int n,char f[],char t[],bool q,int time)
    {
        if(userstack.count(mystr<25>(u))>0&&!releasedtrain.Findval(i).empty())//-u 已登录，且购买的车次必须已经被 release
        {
            Train target;
            rtrain(target,trainlist.Findval(i)[0]);
            if(n<=target.seatNum)//小于最大票数
            {
                bool findf= false,findt=false;
                int day,r1,r2,at,lt;
                int c=0,att=target.startTime,ltt=att,c1=0;
                for(int j=0;j<target.stationNum;j++)
                {
                    if(strcmp(f,target.stations[j].value)==0)//找到起点站
                    {
                        findf=1;
                        lt=ltt;
                        r1=j;
                        c1=c;
                    }
                    if(strcmp(t,target.stations[j].value)==0)//找到终点站
                    {
                        findt=1;
                        at=att;
                        r2=j;
                        c-=c1;
                        break;
                    }
                    c+=target.prices[j];
                    att=ltt+target.travelTimes[j];
                    ltt=att+target.stopoverTimes[j];
                }
                day= daytrans(d)-lt/1440;
                if(findf&&findt)
                {
                    if(day>=target.begindate&&day<=target.enddate)//在售票日期
                    {
                        int num=target.seatNum;
                        Ticket obj;
                        long place=ticketlist.Findval(i)[day-target.begindate];
                        rticket(obj,place);
                        for(int j=r1;j<r2;j++)
                        {
                            num=std::min(num,obj.ticketleft[j]);
                            if(num<n)
                            {
                                if(q)//接受候补
                                {
                                    waitinf towrite(u,i,place,r1,r2,n,time);
                                    waitlist.Insert(i,towrite);
                                    std::cout<<"queue\n";
                                    Order tmp(i,f,t,day+lt/1440,day+at/1440,lt%1440,at%1440,c,n,1,time,place,r1,r2);
                                    userinf.orderlist.Insert(u,tmp);
                                    return;
                                }
                                else
                                {
                                    std::cout<<"-1"<<std::endl;
                                    return;
                                }
                            }
                        }
                        if(num>=n)//能成功买票
                        {
                            for(int j=r1;j<r2;j++)
                            {
                                obj.ticketleft[j]-=n;
                            }
                            wticket(obj,place);//把修改的车票信息写回去
                            std::cout<<c*n<<std::endl;
                            Order tmp(i,f,t,day+lt/1440,day+at/1440,lt%1440,at%1440,c,n,0,time,place,r1,r2);
                            userinf.orderlist.Insert(u,tmp);
                            return;
                        }
                    }
                }
            }
        }
        std::cout<<"-1"<<std::endl;
    }

    int refundticket(char u[],int n,int time)
    {
        if(userstack.count(mystr<25>(u))>0)//-u 已登录
        {
            vector<Order> orders=userinf.orderlist.Findval(u);
            if(n<=orders.size())
            {
                Order obj=orders[n-1];
                if(obj.state==0)//success
                {
                    userinf.orderlist.Delete(u,obj);
                    obj.state=2;
                    userinf.orderlist.Insert(u,obj);
                    Ticket revise;
                    rticket(revise,obj.index);
                    for(int i=obj.rank1;i<obj.rank2;i++)
                    {
                        revise.ticketleft[i]+=obj.number;
                    }
                    if(!waitlist.Findval(obj.trainID).empty())//有候补订单
                    {
                        vector<waitinf> waits=waitlist.Findval(obj.trainID);
                        for(int j=0;j<waits.size();j++)
                        {
                            if(waits[j].index==obj.index)//是同一辆车
                            {
                                int left=revise.seatNum;
                                for(int k=waits[j].rank1;k<waits[j].rank2;k++)
                                {
                                    left=std::min(left,revise.ticketleft[k]);
                                }
                                if(left>=waits[j].number)//能满足
                                {
                                    for(int k=waits[j].rank1;k<waits[j].rank2;k++)
                                    {
                                        revise.ticketleft[k]-=waits[j].number;
                                    }
                                    waitlist.Delete(obj.trainID,waits[j]);//删除候补信息
                                    vector<Order>orderinf=userinf.orderlist.Findval(waits[j].username);
                                    for(int x=0;x<orderinf.size();x++)
                                    {
                                        if(orderinf[x].timestamp==waits[j].timestamp)
                                        {
                                            userinf.orderlist.Delete(waits[j].username,orderinf[x]);
                                            orderinf[x].state=0;
                                            userinf.orderlist.Insert(waits[j].username,orderinf[x]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    wticket(revise,obj.index);
                    return 0;
                }
                else if(obj.state==1)
                {
                    vector<waitinf> waits=waitlist.Findval(obj.trainID);
                    for(int j=0;j<waits.size();j++)
                    {
                        if(waits[j].timestamp==obj.timestamp)
                        {
                            waitlist.Delete(obj.trainID,waits[j]);
                            break;
                        }
                    }
                    userinf.orderlist.Delete(u,obj);
                    obj.state=2;
                    userinf.orderlist.Insert(u,obj);
                    return 0;
                }
            }
        }
        return -1;
    }
};
#endif //CODE_TRAINSYSTEM_H
