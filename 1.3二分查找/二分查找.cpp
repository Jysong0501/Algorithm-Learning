#include <iostream>
using namespace std;

int n,m;
const int N = 1e5+10;
int q[N];

//此二分查找默认数字按大小顺序输入
int main()
{
    cin>>n>>m;//n为数字个数   m为查询次数
    for(int i=0;i<n;i++)    cin>>q[i];
    while(m--)
    {
        int x;//查询数字
        cin>>x;
        int l=0,r = n-1;
        while(l<r)
        {
            int mid = l +r >>1;
            if(q[mid] >= x) r = mid;
            else l = mid +1;
        }
        if(q[l]!=x)cout<<"-1 -1"<<endl;
        else
        {
            cout<<l<<" ";
            while(l <r)
            {
                int mid = l+r+1>>1;
                if(q[mid] <= x) l = mid;
                else r = mid - 1;
            }
            cout<<l<<endl;
        }
    }
    return 0;
}