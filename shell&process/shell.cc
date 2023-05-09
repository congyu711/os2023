#include<bits/stdc++.h>
#include"process.hh"
using namespace std;

mt19937 gen(114514);

queue<process*> ready_proc[3]; // priority lists for ready processes;
list<process*> block_proc;
process* running_proc=nullptr;

const int resnum=64;
vector<int> avil_res;

inline void banker()
{
    process* __p=nullptr;
    for(auto e:block_proc)
    {
        if(__p!=nullptr)    continue;
        bool vio=0;
        for(auto ee:e->reqres)
            if(avil_res[ee.first]<ee.second)    vio=1;
        if(!vio)
        {
            for(auto ee:e->reqres)
                avil_res[ee.first]-=ee.second;
            e->status=ready;
            ready_proc[e->priority].push(e);
            __p=e;
        }
    }
    if(__p!=nullptr)    block_proc.remove(__p);
}

int scheduling()
{
    // running. no need to schedule.
    if(running_proc!=nullptr)   return 0;


    // ready
    for(int pri=0;pri<3;pri++)
    {
        if(ready_proc[pri].size()!=0)
        {
            running_proc=ready_proc[pri].front();
            ready_proc[pri].pop();
            running_proc->status=running;
            return 1;
        }
    }

    // nothing to do
    return -1;
}

int main()
{
    // init
    for(int i=0;i<resnum;i++)
    {
        avil_res.push_back(gen()%16);
    }

    string __buffer;
    int __pid=1;
    while (cout<<"> "&&getline(cin,__buffer))
    {
        stringstream ss;
        cout<<__buffer; puts("");
        // if(__buffer.length()==0||__buffer.substr(0,2)=="//")    continue;
        ss<<__buffer;
        string opt;
        ss>>opt;
        if(opt=="cr")
        {
            string pname; int pri;
            ss>>pname; ss>>pri;
            auto newp=new process(__pid++,pname,pri);
            allproc.push_back(newp);
            ready_proc[pri].push(newp);
            scheduling();
        }
        else if(opt=="req")
        {
            if(running_proc==nullptr)
                cerr<<"no running proc\n";
            int a,b;
            ss>>a>>b;
            bool __f=0;
            for(auto &e:running_proc->reqres)
                if(e.first==a)  {e.second+=b;__f=1;break;}
            if(!__f) running_proc->reqres.push_back({a,b});
            if(avil_res[a]<b)
            {
                cerr<<"running process requires too much resources. blocked.\n";
                running_proc->status=block;
                block_proc.push_back(running_proc);
                running_proc=nullptr;
                banker();
                scheduling();
            }
            else
            {
                avil_res[a]-=b;
            }
        }
        else if(opt=="to")
        {
            if(running_proc==nullptr)
                cerr<<"no running proc\n";
            running_proc->status=ready;
            ready_proc[running_proc->priority].push(running_proc);
            running_proc=nullptr;
            if(scheduling()==-1)
            {
                banker();
                scheduling();
            }
        }
        else if(opt=="de")  // delete(terminate) the currnet running process;
        {
            if(running_proc==nullptr)
                cerr<<"no running proc\n";
            allproc.remove(running_proc);
            for(auto e:running_proc->reqres)
                avil_res[e.first]+=e.second;
            delete running_proc;
            running_proc=nullptr;
            if(scheduling()==-1)
            {
                banker();
                scheduling();
            }
        }
        else if(opt=="print")
        {
            for(auto e:allproc)
            {
                cout<<e->pid<<' '<<e->pname<<' '<<e->priority<<'\n';
                for(auto ee:e->reqres)  cout<<" ("<<ee.first<<','<<ee.second<<") ";
                puts("");
            }
        }
        cout<<"                 ";
        if(running_proc!=nullptr)   cout<<running_proc->pid<<' '<<running_proc->pname<<endl;
        else cout<<"no running proc\n";
    }
    
}