#include "BPlusTree.h"
#include "sentenceslice.h"
#include "usersystem.h"
#include "trainsystem.h"
#include "mymap.h"
#include <vector>
sjtu::map<mystr<25>,User>userstack;//登录列表
Userinf userinf;
Traininf traininf;
int main()
{
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    std::string input;
    while(1)
    {
//        std::cout<<"enter\n";
        std::getline(std::cin,input);
        std::vector<std::string> token= readtokens(input);
        int l=token.size();
        std::string timestamp=token[0];
        if(token[1]=="add_user")
        {
            char c[25],u[25],p[35],n[20],m[35];
            int g;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-c")
                {
                    strcpy(c,token[i+1].c_str());
                }
                else if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
                else if(token[i]=="-p")
                {
                    strcpy(p,token[i+1].c_str());
                }
                else if(token[i]=="-n")
                {
                    strcpy(n,token[i+1].c_str());
                }
                else if(token[i]=="-m")
                {
                    strcpy(m,token[i+1].c_str());
                }
                else if(token[i]=="-g")
                {
                    g = stringToInteger(token[i+1]);
                }
            }
            std::cout<<token[0]<<" "<<userinf.adduser(c,u,p,n,m,g)<<std::endl;
        }
        else if(token[1]=="login")
        {
            char u[25],p[30];
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
                else if(token[i]=="-p")
                {
                    strcpy(p,token[i+1].c_str());
                }
            }
            std::cout<<token[0]<<" "<<userinf.login(u,p)<<std::endl;
        }
        else if(token[1]=="logout")
        {
            char u[25];
            strcpy(u,token[3].c_str());
            std::cout<<token[0]<<" "<<userinf.logout(u)<<std::endl;
        }
        else if(token[1]=="query_profile")
        {
            char c[25],u[25];
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-c")
                {
                    strcpy(c,token[i+1].c_str());
                }
                else if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
            }
            std::cout<<token[0]<<" ";
            userinf.queryprofile(c,u);
        }
        else if(token[1]=="modify_profile")
        {
            char c[25]={},u[25]={},p[35]={},n[20]={},m[35]={};
            int g=-1;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-c")
                {
                    strcpy(c,token[i+1].c_str());
                }
                else if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
                else if(token[i]=="-p")
                {
                    strcpy(p,token[i+1].c_str());
                }
                else if(token[i]=="-n")
                {
                    strcpy(n,token[i+1].c_str());
                }
                else if(token[i]=="-m")
                {
                    strcpy(m,token[i+1].c_str());
                }
                else if(token[i]=="-g")
                {
                    g = stringToInteger(token[i+1]);
                }
            }
            std::cout<<token[0]<<" ";
            userinf.modifyprofile(c,u,p,n,m,g);
        }
        else if(token[1]=="add_train")
        {
            char id[25];
            std::string s,p,x,t,o,d;
            int n,m;
            char y;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-i")
                {
                    strcpy(id,token[i+1].c_str());
                }
                else if(token[i]=="-n")
                {
                    n= stringToInteger(token[i+1]);
                }
                else if(token[i]=="-m")
                {
                    m= stringToInteger(token[i+1]);
                }
                else if(token[i]=="-s")
                {
                    s=token[i+1];
                }
                else if(token[i]=="-p")
                {
                    p=token[i+1];
                }
                else if(token[i]=="-x")
                {
                    x=token[i+1];
                }
                else if(token[i]=="-t")
                {
                    t=token[i+1];
                }
                else if(token[i]=="-o")
                {
                    o=token[i+1];
                }
                else if(token[i]=="-d")
                {
                    d=token[i+1];
                }
                else if(token[i]=="-y")
                {
                    y=token[i+1][0];
                }
            }
            std::cout<<token[0]<<" ";
            std::cout<<traininf.addtrain(id,n,m,s,p,x,t,o,d,y)<<std::endl;
        }
        else if(token[1]=="delete_train")
        {
            char id[25]{};
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-i")
                {
                    strcpy(id,token[i+1].c_str());
                }
            }
            std::cout<<token[0]<<" ";
            std::cout<<traininf.deletetrain(id)<<std::endl;
        }
        else if(token[1]=="release_train")
        {
            char id[25]{};
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-i")
                {
                    strcpy(id,token[i+1].c_str());
                }
            }
            std::cout<<token[0]<<" ";
            std::cout<<traininf.releasetrain(id)<<std::endl;
        }
        else if(token[1]=="query_train")
        {
            char id[25]{};
            std::string d;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-i")
                {
                    strcpy(id,token[i+1].c_str());
                }
                else if(token[i]=="-d")
                {
                    d=token[i+1];
                }
            }
            std::cout<<token[0]<<" ";
            traininf.querytrain(id,d);
        }
        else if(token[1]=="query_ticket")
        {
            char s[35]{},t[35]{};
            std::string d;
            int order=0;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-s")
                {
                    strcpy(s,token[i+1].c_str());
                }
                else if(token[i]=="-t")
                {
                    strcpy(t,token[i+1].c_str());
                }
                else if(token[i]=="-d")
                {
                    d=token[i+1];
                }
                else if(token[i]=="-p")
                {
                    if(token[i+1]=="cost")
                    {
                        order=1;
                    }
                }
            }
            std::cout<<token[0]<<" ";
            traininf.queryticket(s,t,d,order);
        }
        else if(token[1]=="query_transfer")
        {
            char s[35]{},t[35]{};
            std::string d;
            int order=0;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-s")
                {
                    strcpy(s,token[i+1].c_str());
                }
                else if(token[i]=="-t")
                {
                    strcpy(t,token[i+1].c_str());
                }
                else if(token[i]=="-d")
                {
                    d=token[i+1];
                }
                else if(token[i]=="-p")
                {
                    if(token[i+1]=="cost")
                    {
                        order=1;
                    }
                }
            }
            std::cout<<token[0]<<" ";
            traininf.querytransfer(s,t,d,order);
        }
        else if(token[1]=="buy_ticket")
        {
            char u[25]{},id[25]{},f[35]{},t[35]{};
            std::string d;
            int n;
            bool q=false;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
                else if(token[i]=="-i")
                {
                    strcpy(id,token[i+1].c_str());
                }
                else if(token[i]=="-f")
                {
                    strcpy(f,token[i+1].c_str());
                }
                else if(token[i]=="-t")
                {
                    strcpy(t,token[i+1].c_str());
                }
                else if(token[i]=="-d")
                {
                    d=token[i+1];
                }
                else if(token[i]=="-n")
                {
                    n= stringToInteger(token[i+1]);
                }
                else if(token[i]=="-q")
                {
                    if(token[i+1]=="true")
                    {
                        q=1;
                    }
                }
            }
            std::cout<<token[0]<<" ";
            traininf.buyticket(u,id,d,n,f,t,q, stamp(token[0]));
        }
        else if(token[1]=="query_order")
        {
            char u[25]{};
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
            }
            std::cout<<token[0]<<" ";
            userinf.queryorder(u);
        }
        else if(token[1]=="refund_ticket")
        {
            char u[25]{};
            int n=1;
            for(int i=2;i<l;i+=2)
            {
                if(token[i]=="-u")
                {
                    strcpy(u,token[i+1].c_str());
                }
                else if(token[i]=="-n")
                {
                    n= stringToInteger(token[i+1]);
                }
            }
            std::cout<<token[0]<<" ";
            std::cout<<traininf.refundticket(u,n, stamp(token[0]))<<std::endl;
        }
        else if(token[1]=="clean")
        {

        }
        else if(token[1]=="exit")
        {
            std::cout<<token[0]<<" "<<"bye\n";
            userinf.modifyrest();
            return 0;
        }
    }
}