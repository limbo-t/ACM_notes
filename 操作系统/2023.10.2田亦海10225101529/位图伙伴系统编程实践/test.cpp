#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<list>
#include<bitset>
#include<math.h>
#include<map>
using namespace std;

bool blocks[16];
list<int> free_list[5];
bitset<8> bm[5];
map<int,int> p;

int fun(int x)//fun(2)=2,fun(3~4)=4,fun(5~8)=8...
{
    int y = x;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);

    return (y == (x & ~(x >> 1))) ? y : 2 * (x & ~(x >> 1));
}

void pm()
{
    printf("//==================当前视图=================//\n");
    printf("blocks:   ");
    for (int i = 0; i < 16;i++)
        blocks[i] = 0;
    for (auto x : p)
    {
        for (int i = 0; i < x.second;i++)
            blocks[x.first + i] = 1;
    }
    for(auto x:blocks)
    {
        if(x) printf("@ ");
        else printf("? ");
    }
    printf("\n\n");
    for (int i = 1; i <= 4;i++)
    {
        printf("bitmap[%d]:",i);
        for (int k = 0; k < (1 << (i))-1;k++)
                printf(" ");
        for (int j = 0; j < 16 / (1 << i);j++)
        {
            if(bm[i][j])
                printf("1");
            else
                printf("0");
            for (int k = 0; k < (1 << (i+1))-1;k++)
                printf(" ");
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i <= 4;i++)
    {
        printf("free_list[%d]: ", i);
        for(auto x:free_list[i])
            printf("%d ", x);
        printf("\n");
    }
    printf("\n已申请:\n");
    for(auto x:p)
        printf("p=%d,size=%d\n", x.first, x.second);
    printf("//===========================================//\n");
}

signed main()
{
    printf("本程序模拟一个最小分配块大小1KB,最大分配16KB下的buddy算法\n");
    printf("?表示此块未被占用,@表示已被占用\n");
    printf("blocks对应序号0~15,size=1\n");
    printf("bitmap表示其两个大小相等的子块的分配状态,如果子块均未分配或均分配记为0,否则为1(用于指示是否合并)\n");
    printf("bitmap[1]对应序号0~7,单个size=2\n");
    printf("bitmap[2]对应序号0~3,单个size=4\n");
    printf("bitmap[3]对应序号0~1,单个size=8\n");
    printf("bitmap[4]对应序号0,单个size=16\n");
    printf("输入方式:\nm 3(意为malloc 3KB)\n");
    printf("f 2(意为free 2号块所在的内存块)\n");
    free_list[4].push_back(0);
    bool changed = true;
    while(1)
    {
        if(changed)pm();
        char op;
        int n;
        cin >> op >> n;
        if(op=='m')
        {
            if(n<=0)
            {
                printf("建议别分配size=%d\n\n", n);
                changed = false;
                continue;
            }
            int need = fun(n);
            bool judge = false;
            for (int i = log2(need); i <= 4;i++)
            {
                if(!free_list[i].empty())
                {
                    judge = true;
                    int tmp=free_list[i].front();
                    if(i!=4)
                        bm[i + 1][tmp / 2] = bm[i + 1][tmp / 2] ? 0 : 1;
                    free_list[i].pop_front();
                    while(i>log2(need))
                    {
                        bm[i][tmp]= bm[i][tmp]?0:1;
                        i--;
                        tmp *= 2;
                        free_list[i].push_back(tmp + 1);
                    }
                    p[tmp * (1 << i)]=(1 << i);
                    break;
                }
            }
            if(!judge)
            {
                printf("分配失败(内存不够)\n");
                changed = false;
            }
            else
            {
                printf("分配成功\n");
                changed = true;
            }
        }
        else if(op=='f')
        {
            if(p.find(n)==p.end())
            {
                printf("释放失败(不存在该指针)\n");
                changed = false; 
            }
            else
            {
                printf("释放成功\n");
                changed = true;
                int i = log2(p[n]);
                int tmp = n / p[n];
                p.erase(n);
                while(i<4)
                {
                    if(bm[i+1][tmp/2]==1)//说明伙伴空闲
                    {
                        bm[i + 1][tmp / 2] = 0;
                        for (list<int>::iterator it = free_list[i].begin(); it != free_list[i].end();it++)
                        {
                            if(*it/2==tmp/2)
                            {
                                free_list[i].erase(it);
                                break;
                            }
                        }
                        tmp /= 2;
                        i++;
                    }
                    else
                    {
                        bm[i + 1][tmp / 2] = 1;
                        free_list[i].push_back(tmp);
                        break;
                    }
                }
                if(i==4)
                {
                    free_list[4].push_back(0);
                }
            }
        }
        else
        {
            printf("未知命令\n");
        }
        printf("\n\n");
    }
    return 0;
}