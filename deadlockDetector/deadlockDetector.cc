#include<bits/stdc++.h>
#include"kdtree-cpp/kdtree.hpp"

using namespace std;


int main()
{
    mt19937 gen(114514);
    const int n=500,m=50;   // number of processes and resources;
    vector<int> avil_resources(m);
    vector<vector<double>> required_resources(n,vector<double>(m)); // use double for Kdtree
    vector<vector<double>> allocated_resources(n,vector<double>(m));
    Kdtree::KdNodeVector nodes;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            required_resources[i][j]=gen()%20;
            allocated_resources[i][j]=gen()%10;
        }
        nodes.push_back(Kdtree::KdNode(required_resources[i]));
    }
    for(auto &e:avil_resources) e=gen()%10;
    Kdtree::KdTree tree(&nodes);
    // No... This kdtree is not able to do range query. It can only handle knn queries.

    //-----------------------------------
    // the basic algorithm
    vector<bool> fin(n);
    auto __avil_resources=avil_resources;
    int cnt=0;
    while(1)
    {
        bool flag=0;
        for(int i=0;i<n;i++)
        {
            if(fin[i])  continue;
            bool violated=0;
            for(int j=0;j<m;j++)    if(required_resources[i][j]>__avil_resources[j])  violated=1;
            if(!violated)   // run the process. allocate the required resources
            {
                fin[i]=1;
                cnt++;
                flag=1;
                for(int j=0;j<m;j++)    __avil_resources[j]+=allocated_resources[i][j];
            }
        }
        if(!flag)
        {
            if(cnt!=n)  cout<<"deadlock!\n";
            else    cout<<"no deadlock\n";
            break;
        }
    }
    //-----------------------------------
}