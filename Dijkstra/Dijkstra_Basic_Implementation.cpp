vector<int> dis(n,INT_MAX);
priority_queue<pair<int,int>, vector<pair<int,int>>,greater<>> pq;
dis[0]=0;
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