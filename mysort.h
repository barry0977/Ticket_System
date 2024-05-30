//
// Created by 24115 on 2024/5/30.
//

#ifndef CODE_MYSORT_H
#define CODE_MYSORT_H
#include "myvector.h"
template<class T>
void merge(sjtu::vector<T>&a,int low,int high,int mid,bool (*f)(const T&,const T&))
{
    sjtu::vector<T>b,c;
    for(int i=0;i<=mid-low;i++)
    {
        b.push_back(a[low+i]);
    }
    for(int i=0;i<high-mid;i++)
    {
        c.push_back(a[mid+1+i]);
    }
    int i=0,j=0,k=low;
    while(i<=mid-low&&j<high-mid)
    {
        if(f(b[i],c[j]))
        {
            a[k]=b[i];
            i++;
        }
        else
        {
            a[k]=c[j];
            j++;
        }
        k++;
    }
    while(i<=mid-low)
    {
        a[k++]=b[i++];
    }
    while(j<high-mid)
    {
        a[k++]=c[j++];
    }
}

template<class T>
void mergesort(sjtu::vector<T>&a,int low,int high,bool (*f)(const T&,const T&))
{
    if(low<high)
    {
        int mid=(low+high)/2;
        mergesort(a,low,mid,f);
        mergesort(a,mid+1,high,f);
        merge(a,low,high,mid,f);
    }
}


#endif //CODE_MYSORT_H
