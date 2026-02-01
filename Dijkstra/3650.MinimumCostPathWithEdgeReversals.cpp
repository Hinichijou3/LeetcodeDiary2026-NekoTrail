#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int minCost(int n, vector<vector<int>>& edges) 
    {
        vector<vector<pair<int,int>>> g(n);
        //邻接表
        for(auto& e:edges)
        {
            int x=e[0],y=e[1],wt=e[2];
            g[x].emplace_back(y,wt);
            g[y].emplace_back(x,2*wt);
        }

        vector<int> dis(n,INT_MAX);
        dis[0]=0;
        priority_queue<pair<int,int>, vector<pair<int,int>>,greater<>> pq;
        pq.emplace(0,0);

        while(!pq.empty())
        {
            auto [dis_x,x]=pq.top();
            pq.pop();
            if(x==n-1)  return dis_x;
            if(dis_x>dis[x])    continue;
            for(auto& [y, wt]:g[x])
            {
                if(dis[y]>dis_x+wt)
                {
                    dis[y]=dis_x+wt;
                    pq.emplace(dis[y],y);
                }
            }
        }
        return -1;
    }

int main()
{
    int n=4;
    vector<vector<int>> edges={{0,1,3},{3,1,1},{2,3,4},{0,2,2}};
    cout << minCost(n, edges) << endl;
    return 0;
}