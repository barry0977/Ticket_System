#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
const int M=200;
const int L=200;
template<int len,class VALUE>//len表示index字符数组的长度，VALUE表示值
class BPTree
{
private:
    std::fstream file;
    std::string file_name;

    struct element//约70
    {
        char index[len];
        VALUE value;

        element()=default;

        element(char ind[], VALUE num)
        {
            strcpy(index, ind);
            value = num;
        }

        friend bool operator<(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) != 0)
            {
                return strcmp(lhs.index, rhs.index) < 0;
            }
            else
            {
                return lhs.value < rhs.value;
            }
        }

        friend bool operator>(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) != 0)
            {
                return strcmp(lhs.index, rhs.index) > 0;
            }
            else
            {
                return lhs.value > rhs.value;
            }
        }

        friend bool operator==(const element& lhs, const element& rhs)
        {
            if (strcmp(lhs.index, rhs.index) == 0 && lhs.value == rhs.value)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const element& obj)
        {
            os << obj.index << " " << obj.value << std::endl;
            return os;
        }
    };

    struct node
    {
        bool isleaf=false;//是否是叶结点
        int keynum=0;
        element keylist[M+5];
        int location[M+5];

        node()=default;
    };


    struct database//数据块
    {
        int num=0;//数据数量
        int next=-1;//下一个数据地址
        element data[L+5];
    };

    struct firstnode//文件中的第一个结点，储存根结点地址和数据块头结点地址
    {
        long rootindex;
        long headindex;
    }first;

    template<class T>
    int Write(T& tmp)
    {
        file.open(file_name, std::ios::app | std::ios::binary);
        int index = file.tellp();
        file.write(reinterpret_cast<char*>(&tmp), sizeof(T));
        file.close();
        return index;
    }

    void get_first(firstnode& tmp)
    {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(firstnode));
        file.close();
    }

    void write_first(firstnode& tmp) {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(firstnode));
        file.close();
    }

    template<class T>
    void read_inf(T& tmp,int index)
    {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&tmp), sizeof(T));
        file.close();
    }

    template<class T>
    void write_inf(T& tmp,int index)
    {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(T));
        file.close();
    }

    void divide_database(int cur_index,int order,int data_index)//分裂数据块,order表示data_index在cur_index数组中的下标
    {
        database tmp;
        read_inf<database>(tmp,data_index);
        int mid=tmp.num/2;
        database new1,new2;
        for(int i=0;i<mid;i++)
        {
            new1.data[i]=tmp.data[i];
        }
        new1.num=mid;
        for(int i=mid;i<tmp.num;i++)
        {
            new2.data[i-mid]=tmp.data[i];
        }
        new2.num=tmp.num-mid;
        new2.next=tmp.next;
        int newindex=Write<database>(new2);
        element newkey=new2.data[0];
        new1.next=newindex;
        node cur;
        read_inf<node>(cur,cur_index);
        for(int i=cur.keynum-1;i>=order;i--)
        {
            cur.keylist[i+1]=cur.keylist[i];
            cur.location[i+2]=cur.location[i+1];
        }
        cur.keylist[order]=newkey;
        cur.location[order+1]=newindex;
        cur.keynum++;
        write_inf<node>(cur,cur_index);
        write_inf<database>(new1,data_index);
    }

    void divide_node(int fa_index,int order,int cur_index)
    {
        node cur;
        read_inf<node>(cur,cur_index);
        int mid=cur.keynum/2;
        node new1,new2;
        new1.isleaf=new2.isleaf=cur.isleaf;
        for(int i=0;i<mid;i++)
        {
            new1.keylist[i]=cur.keylist[i];
            new1.location[i]=cur.location[i];
        }
        new1.location[mid]=cur.location[mid];
        new1.keynum=mid;
        for(int i=mid+1;i<cur.keynum;i++)
        {
            new2.keylist[i-mid-1]=cur.keylist[i];
            new2.location[i-mid-1]=cur.location[i];
        }
        new2.location[cur.keynum-mid-1]=cur.location[cur.keynum];
        new2.keynum=cur.keynum-mid-1;
        int newindex= Write<node>(new2);
        element newkey=cur.keylist[mid];
        write_inf<node>(new1,cur_index);
        if(fa_index>=0)//如果有父结点
        {
            node fa;
            read_inf<node>(fa,fa_index);
            for(int i=fa.keynum-1;i>=order;i--)
            {
                fa.keylist[i+1]=fa.keylist[i];
                fa.location[i+2]=fa.location[i+1];
            }
            fa.keylist[order]=newkey;
            fa.location[order+1]=newindex;
            fa.keynum++;
            write_inf<node>(fa,fa_index);
        }
        else//如果已经是根结点
        {
            node newroot;
            newroot.keynum=1;
            newroot.keylist[0]=newkey;
            newroot.location[0]=cur_index;
            newroot.location[1]=newindex;
            first.rootindex=Write<node>(newroot);//把新的根结点写进去
            write_first(first);
        }
    }

    void insert(int cur_index,element& obj)
    {
        node cur;
        read_inf<node>(cur,cur_index);
        int min=0,max=cur.keynum-1,place=-1;
        while(min<=max)
        {
            int mid=(min+max)/2;
            if(obj>cur.keylist[mid])
            {
                place=mid;
                min=mid+1;
            }
            else
            {
                max=mid-1;
            }
        }
        place++;
        if(cur.isleaf)//如果是叶结点
        {
            int data_index=cur.location[place];
            database Data;
            read_inf<database>(Data,data_index);
            int mini=0,maxi=Data.num-1,order=-1;//order为最大的比obj小的元素
            while (mini <= maxi)//二分查找
            {
                int middle = (mini + maxi) / 2;
                if (Data.data[middle] < obj)
                {
                    order = middle;
                    mini = middle + 1;
                }
                else
                {
                    maxi = middle - 1;
                }
            }
            for(int i=Data.num-1;i>order;i--)
            {
                Data.data[i+1]=Data.data[i];
            }
            Data.data[order+1]=obj;
            Data.num++;
            write_inf<database>(Data,data_index);
            if(Data.num>L)
            {
                divide_database(cur_index,place,data_index);//把Data分裂，改变cur中的键值
            }
        }
        else//如果不是叶结点
        {
            int next_index=cur.location[place];
            insert(next_index,obj);
            node next;
            read_inf<node>(next,next_index);
            if(next.keynum>M)
            {
                divide_node(cur_index,place,next_index);//把next分裂，改变cur中的键值
            }
        }
        read_inf<node>(cur,cur_index);//要重新读一下
        if(cur_index==first.rootindex&&cur.keynum>M)//如果是根结点并且超过数量
        {
            node useless;
            divide_node(-1,-1,cur_index);
        }
    }

    void _delete(int cur_index,element& obj)
    {
        node cur;
        read_inf<node>(cur,cur_index);
        int min=0,max=cur.keynum-1,place=-1;
        while(min<=max)
        {
            int mid=(min+max)/2;
            if(obj==cur.keylist[mid])
            {
                place=mid;
                break;
            }
            else if(obj>cur.keylist[mid])
            {
                place=mid;
                min=mid+1;
            }
            else
            {
                max=mid-1;
            }
        }
        place++;//找到要删除的元素所在的位置(location,和key的位置不一样)
        if(cur.isleaf)//如果已经是叶结点
        {
            int data_index=cur.location[place];
            database Data;
            read_inf<database>(Data,data_index);
            int mini=0,maxi=Data.num-1,order=-1;
            while (mini <= maxi)//二分查找
            {
                int middle = (mini + maxi) / 2;
                if(obj==Data.data[middle])
                {
                    order=middle;
                    break;
                }
                else if(obj<Data.data[middle])
                {
                    maxi=middle-1;
                }
                else
                {
                    mini=middle+1;
                }
            }
            if(order>=0)//如果在块中找到了目标
            {
                if(Data.num>(L+1)/2||(place<1&&place+1>cur.keynum))//删除后仍满足要求(可能是根结点)
                {
                    for(int i=order;i<Data.num-1;i++)
                    {
                        Data.data[i]=Data.data[i+1];
                    }
                    Data.num--;
                    if(order==0&&place>=1)//需要修改父结点的key
                    {
                        cur.keylist[place-1]=Data.data[0];
                    }
                    write_inf<node>(cur,cur_index);
                    write_inf<database>(Data,data_index);
                }
                else//需要借孩子或者合并
                {
                    database lData,rData;
                    if(place-1>=0)//如果有左兄弟
                    {
                        read_inf<database>(lData,cur.location[place-1]);
                        if(lData.num>(L+1)/2)//有多余的孩子
                        {
                            lData.num--;
                            for(int i=order;i>0;i--)
                            {
                                Data.data[i]=Data.data[i-1];
                            }
                            Data.data[0]=lData.data[lData.num];
                            cur.keylist[place-1]=Data.data[0];
                            write_inf<database>(Data,data_index);
                            write_inf<database>(lData,cur.location[place-1]);
                            write_inf<node>(cur,cur_index);
                            return;
                        }
                    }
                    if(place+1<=cur.keynum)//如果有右兄弟
                    {
                        read_inf<database>(rData,cur.location[place+1]);
                        if(rData.num>(L+1)/2)//有多余的孩子
                        {
                            rData.num--;
                            for(int i=order;i<Data.num-1;i++)
                            {
                                Data.data[i]=Data.data[i+1];
                            }
                            Data.data[Data.num-1]=rData.data[0];
                            if(place-1>=0)
                            {
                                cur.keylist[place-1]=Data.data[0];
                            }
                            for(int i=0;i<rData.num;i++)
                            {
                                rData.data[i]=rData.data[i+1];
                            }
                            cur.keylist[place]=rData.data[0];
                            write_inf<database>(Data,data_index);
                            write_inf<database>(rData,cur.location[place+1]);
                            write_inf<node>(cur,cur_index);
                            return;
                        }
                    }
                    //到这里说明没有借到儿子，看能否合并
                    if(place-1>=0)//如果有左兄弟，把目前结点合并到左结点
                    {
                        int k=lData.num;
                        lData.next=Data.next;
                        for(int i=0;i<order;i++)
                        {
                            lData.data[k++]=Data.data[i];
                        }
                        for(int i=order+1;i<Data.num;i++)
                        {
                            lData.data[k++]=Data.data[i];
                        }
                        lData.num+=Data.num-1;
                        for(int i=place;i<cur.keynum;i++)//删除目前结点对应的key和地址，后面全都前移
                        {
                            cur.keylist[i-1]=cur.keylist[i];
                            cur.location[i]=cur.location[i+1];
                        }
                        cur.keynum--;
                        write_inf<database>(lData,cur.location[place-1]);
                        write_inf<node>(cur,cur_index);
                        return;
                    }
                    if(place+1<=cur.keynum)//如果有右兄弟,把右兄弟合并到目前结点
                    {
                        for (int i = order; i < Data.num - 1; i++)
                        {
                            Data.data[i] = Data.data[i + 1];
                        }
                        Data.num--;
                        int k = Data.num;
                        Data.next=rData.next;
                        for (int i = 0; i < rData.num; i++)
                        {
                            Data.data[k++] = rData.data[i];
                        }
                        Data.num += rData.num;
                        for (int i = place; i < cur.keynum-1; i++)
                        {
                            cur.keylist[i]=cur.keylist[i+1];
                            cur.location[i+1]=cur.location[i+2];
                        }
                        cur.keynum--;
                        write_inf<database>(Data,data_index);
                        write_inf<node>(cur,cur_index);
                        return;
                    }
                }
            }
        }
        else//不是叶结点
        {
            int next_index=cur.location[place];
//            node next;
//            read_inf<node>(next,next_index);
            _delete(next_index,obj);//递归后next已经被修改
            node next;
            read_inf<node>(next,next_index);
            if(next.keynum<(M+1)/2)///如果next的key数量不符合要求
            {
                node left,right;
                //先看能不能借个孩子
                if(place-1>=0)//如果有左兄弟
                {
                    read_inf<node>(left,cur.location[place-1]);
                    if(left.keynum>(M+1)/2)//有多余的孩子
                    {
                        for(int i=next.keynum-1;i>=0;i++)
                        {
                            next.keylist[i+1]=next.keylist[i];
                            next.location[i+2]=next.location[i+1];
                        }
                        next.location[1]=next.location[0];
                        next.keylist[0]=cur.keylist[place-1];//新增的key
                        next.location[0]=left.location[left.keynum];//新增的地址
                        cur.keylist[place-1]=left.keylist[left.keynum-1];//父结点的key也要修改
                        next.keynum++;
                        left.keynum--;
                        write_inf<node>(next,next_index);
                        write_inf<node>(left,cur.location[place-1]);
                        write_inf<node>(cur,cur_index);
                        return;
                    }
                }
                if(place+1<=cur.keynum)//如果有右兄弟
                {
                    read_inf<node>(right,cur.location[place+1]);
                    if(right.keynum>(M+1)/2)//有多余的孩子
                    {
                        next.keylist[next.keynum]=cur.keylist[place];
                        cur.keylist[place]=right.keylist[0];
                        next.location[next.keynum+1]=right.location[0];
                        for(int i=0;i<right.keynum-1;i++)
                        {
                            right.keylist[i]=right.keylist[i+1];
                            right.location[i]=right.location[i+1];
                        }
                        right.location[right.keynum-1]=right.location[right.keynum];
                        next.keynum++;
                        right.keynum--;
                        write_inf<node>(next,next_index);
                        write_inf<node>(right,cur.location[place+1]);
                        write_inf<node>(cur,cur_index);
                        return;
                    }
                }
                //到这里说明没有借到儿子，看能否合并
                if(place-1>=0)//如果有左兄弟，把目前结点合并到左结点
                {
                    left.keylist[left.keynum]=cur.keylist[place-1];
                    left.location[left.keynum+1]=next.location[0];
                    for(int i=0;i<next.keynum;i++)
                    {
                        left.keylist[left.keynum+1+i]=next.keylist[i];
                        left.location[left.keynum+2+i]=next.location[i+1];
                    }
                    left.keynum+=1+next.keynum;
                    for(int i=place-1;i<cur.keynum-1;i++)
                    {
                        cur.keylist[i]=cur.keylist[i+1];
                        cur.location[i+1]=cur.location[i+2];
                    }
                    cur.keynum--;
                    write_inf<node>(left,cur.location[place-1]);
                    write_inf<node>(cur,cur_index);
                }
                else if(place+1<=cur.keynum)//如果有右兄弟,把右兄弟合并到目前结点
                {
                    next.keylist[next.keynum]=cur.keylist[place];
                    next.location[next.keynum+1]=right.location[0];
                    for(int i=0;i<right.keynum-1;i++)
                    {
                        next.keylist[next.keynum+1+i]=right.keylist[i];
                        next.location[next.keynum+2+i]=right.location[i+1];
                    }
                    next.keynum+=1+right.keynum;
                    for(int i=place;i<cur.keynum-1;i++)
                    {
                        cur.keylist[i]=cur.keylist[i+1];
                        cur.location[i+1]=cur.location[i+2];
                    }
                    cur.keynum--;
                    write_inf<node>(next,next_index);
                    write_inf<node>(cur,cur_index);
                }
                if(cur_index==first.rootindex&&cur.keynum==0)//根结点只有一个孩子
                {
                    first.rootindex=cur.location[0];
                    write_first(first);
                    return;
                }
            }
        }
    }

    void find(int cur_index,char obj[])
    {
        node cur;
        read_inf<node>(cur,cur_index);
        int min=0,max=cur.keynum-1,place=-1;
        while(min<=max)
        {
            int mid=(min+max)/2;
            if(std::strcmp(obj,cur.keylist[mid].index)>0)
            {
                place=mid;
                min=mid+1;
            }
            else
            {
                max=mid-1;
            }
        }
        place++;
        if(cur.isleaf)//如果是叶结点
        {
            int findnum=0;
            int data_index=cur.location[place];
            database Data;
            read_inf<database>(Data,data_index);
            int mini=0,maxi=Data.num-1,order=-1;//找到比obj小的最大的index
            while (mini <= maxi)//二分查找
            {
                int middle = (mini + maxi) / 2;
                if (std::strcmp(obj,Data.data[middle].index)>0)
                {
                    order = middle;
                    mini=middle+1;
                }
                else
                {
                    maxi = middle - 1;
                }
            }
            if(order==Data.num-1)//如果是该数据块最后一个元素
            {
                while(Data.next>=0)
                {
                    bool sign=false;
                    read_inf<database>(Data,Data.next);
                    for(int i = 0;i<Data.num;i++)
                    {
                        if(strcmp(obj,Data.data[i].index)<0)
                        {
                            sign=true;
                            break;
                        }
                        else
                        {
                            findnum++;
                            std::cout<<Data.data[i].value<<" ";
                        }
                    }
                    if(sign)
                    {
                        break;
                    }
                }
                if(findnum==0)
                {
                    std::cout<<"null";
                }
                std::cout<<"\n";
                return;
            }
            else
            {
                if(strcmp(obj,Data.data[order+1].index)<0)
                {
                    std::cout<<"null\n";
                    return;
                }
                bool sign=false;
                while(1)
                {
                    for(int i=order+1;i<Data.num;i++)
                    {
                        if(strcmp(obj,Data.data[i].index)<0)
                        {
                            sign= true;
                            break;
                        }
                        else
                        {
                            findnum++;
                            std::cout<<Data.data[i].value<<" ";
                        }
                    }
                    if(sign)
                    {
                        std::cout<<"\n";
                        return;
                    }
                    if(Data.next>=0)
                    {
                        read_inf<database>(Data,Data.next);
                        order=-1;//进入下一个数据块前order要重置
                    }
                    else
                    {
                        std::cout<<"\n";
                        return;
                    }
                }
            }
        }
        else
        {
            int next_index=cur.location[place];
            find(next_index,obj);
        }
    }
public:
    BPTree()=default;

    BPTree(std::string FN)
    {
        file_name = FN;
        file.open(file_name);
        if (!file.good())
        {
            file.open(file_name, std::ios::out | std::ios::binary);
            file.close();
            file.open(file_name);
            file.write(reinterpret_cast<char*>(&first), sizeof(firstnode));
            file.close();//要先关掉
            database head;
            head.next=-1;
            first.headindex= Write<database>(head);//如果是新开的文件，先写入首个数据块和根结点
            node root;
            root.isleaf=true;
            root.location[0]=first.headindex;
            first.rootindex= Write<node>(root);
            write_first(first);
        }
        file.close();
    }

    void Insert(char name[], VALUE number)
    {
        element obj(name,number);
        get_first(first);
//        node root;
//        read_inf<node>(root,first.rootindex);
        insert(first.rootindex,obj);
    }

    void Delete(char name[], VALUE number)
    {
        element obj(name,number);
        get_first(first);
//        node root;
//        read_inf<node>(root,first.rootindex);
        _delete(first.rootindex,obj);
    }

    void Find(char obj[])
    {
        get_first(first);
        find(first.rootindex,obj);
    }
};
int main() {
    int n,value;
    std::cin>>n;
    std::string mode;
    BPTree<66,int>bpTree("file");
    for (int i = 0; i < n; i++)
    {
        std::cin >> mode;
        std::cin.get();//要把空格读掉
        if (mode == "insert")
        {
            std::string index;
            std::cin >> index;
            char tmp[66];
            std::strcpy(tmp, index.c_str());
            std::cin >> value;
            bpTree.Insert(tmp, value);
        }
        else if (mode == "delete")
        {
            std::string index;
            std::cin >> index;
            char tmp[66];
            strcpy(tmp, index.c_str());
            std::cin >> value;
            bpTree.Delete(tmp, value);
        }
        else if (mode == "find")
        {
            std::string index;
            std::cin >> index;
            char tmp[66];
            strcpy(tmp, index.c_str());
            bpTree.Find(tmp);
        }
    }
    return 0;
}
