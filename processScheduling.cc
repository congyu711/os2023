#include<bits/stdc++.h>
using namespace std;

// arrive time. last time.
using _process=pair<int, int>;

vector<_process> genProcess(int n)
{
    vector<_process> res;
    mt19937 gen(114514);
    for(int i=0;i<n;i++)
        res.push_back(make_pair(gen()%100+1, gen()%10+1));
    sort(res.begin(),res.end(),[](_process a, _process b){
        return a.first<b.first;
    });
    return res;
}
// turnaround time for each process
vector<int> FIFO(const vector<_process> &plist)
{
    vector<int> res;
    int T=0;
    for(auto e:plist)
    {
        if(T<e.first) T=e.first;
        T+=e.second;
        res.push_back(T-e.first);
    }
    return res;
}
vector<int> RR(const vector<_process> &plist)
{
    vector<int> res(plist.size());
    vector<bool> inrunning(plist.size(),0);
    const int rsize=5;
    vector<_process> remaintime=plist;
    int fin=0;
    int T=plist.front().first;
    list<int> running;
    running.push_back(0);
    inrunning[0]=1;
    while(fin!=plist.size())
    {
        for(auto p=running.rbegin();p!=running.rend();++p)
        {
            auto timepiece=min(remaintime[*p].second, rsize);
            if(remaintime[*p].second<=rsize)    // finish this process
            {
                fin++;
                // https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
                res[*p]=T+timepiece-remaintime[*p].first;
                remaintime[*p].second-=timepiece;   // this must be done separately since *p may change
                // inrunning[*(next(p).base())]=0;
                running.erase(next(p).base());
            }
            else    remaintime[*p].second-=timepiece;
            // add new proess to `running`
            for(int i=0;i<plist.size();i++)
            {
                if(remaintime[i].second==plist[i].second&&     // haven't been added to running list.
                inrunning[i]==0&&
                plist[i].first>=T&&plist[i].first<=T+timepiece)
                {
                    running.push_back(i);
                    inrunning[i]=1;
                    p=running.rbegin();
                }
            }
            T+=timepiece;
        }
        if(running.empty()&&fin!=plist.size())
        {
            auto it=lower_bound(plist.begin(),plist.end(),make_pair(T,0),[](_process a,_process b){
                return a.first<b.first;
            })-plist.begin();
            while(remaintime[it].second==0)    it++;
            running.push_back(it);
            inrunning[it]=1;
            T=plist[it].first;
        }

    }
    return res;
}
// This is ugly... I forgot to add IDs to _processes
class _aux
{
public:
    // start time, last time, no
    int first,second,no;
    _aux():first(0),second(0),no(0){}
    _aux(int a,int b,int c):first(a),second(b),no(c){}
};
bool operator<(const _aux &a, const _aux &b)
{
    return a.second>b.second;
}

vector<int> SPN(const vector<_process> &plist)
{
    vector<int> res(plist.size(),0);
    vector<bool> vis(plist.size(),0);
    vector<_aux> auxlist(plist.size());
    for(int i=0;i<auxlist.size();i++)
        auxlist[i]={plist[i].first,plist[i].second,i};
    priority_queue<_aux> pq;
    pq.push(auxlist[0]);
    vis[0]=1;
    int T=auxlist[0].first;
    int fin=0;
    while(fin!=plist.size())
    {
        auto c=pq.top();
        fin++;
        pq.pop();
        res[c.no]=T+c.second-c.first;
        // add new process to pq.
        for(int i=0;i<plist.size();i++)
        {
            if(res[i]==0&&vis[i]==0&&plist[i].first<=T+c.second&&plist[i].first>=T)
            {
                pq.push(auxlist[i]);
                vis[i]=1;
            }
        }
        T+=c.second;
        if(pq.empty()&&fin!=plist.size())
        {
            auto it=lower_bound(plist.begin(),plist.end(),make_pair(T,0),[](_process a,_process b){
                return a.first<b.first;
            })-plist.begin();
            while(vis[it]==0) it++;
            pq.push(auxlist[it]);
            vis[it]=1;
            T=plist[it].first;
        }
    }
    return res;
}

vector<int> HRRN(const vector<_process> &plist)
{
    vector<int> res(plist.size(),0);
    vector<_aux> auxlist(plist.size());
    for(int i=0;i<auxlist.size();i++)
        auxlist[i]={plist[i].first,plist[i].second,i};
    // Currently I don't want to use binomial heap just for log time update...
    // a brute force method:
    int T=plist[0].first;
    int fin=0;
    while(1)
    {
        // decide run which process
        int tmp=-1;
        double maxR=-1.0;
        for(int i=0;i<plist.size();i++)
        {
            if(auxlist[i].first<=T&&auxlist[i].second!=0)
            {
                double r=1.0*(T-auxlist[i].first)/auxlist[i].second;
                if(r>=maxR)
                {
                    maxR=r;
                    tmp=i;
                }
            }
        }
        if(tmp==-1)
        {
            T++;
            continue;
        }
        fin++;
        T+=auxlist[tmp].second;
        res[tmp]=T;
        auxlist.at(tmp).second=0;
        if(fin==plist.size())   break;
    }
    return res;
}

vector<int> SRT(const vector<_process> &plist)
{
    vector<int> res(plist.size(),0);
    vector<_aux> auxlist(plist.size());
    for(int i=0;i<auxlist.size();i++)
        auxlist[i]={plist[i].first,plist[i].second,i};
    // Currently I don't want to use binomial heap just for log time update...
    // a brute force method:
    int T=plist[0].first;
    int fin=0;
    while(1)
    {
        // decide run which process
        int tmp=-1,minn=INT_MAX;
        for(int i=0;i<plist.size();i++)
        {
            if(auxlist[i].first<=T&&auxlist[i].second!=0)
            {
                if(auxlist[i].second<=minn)
                {
                    minn=auxlist[i].second;
                    tmp=i;
                }
            }
        }
        auxlist.at(tmp).second--;
        if(auxlist[tmp].second==0)
        {
            res[tmp]=T+1;
            fin++;
        }
        T++;
        if(fin==plist.size())   break;
    }
    return res;
}

// feedback with n RQs and 2^i timepiece for the i th queue.
vector<int> Feedback(const vector<_process> &plist)
{
    vector<int> res(plist.size(),0);
    vector<bool> vis(plist.size());
    auto remaintime=plist;
    const int N=5;
    queue<int> q[N];
    int T=plist[0].first;
    int fin=0;
    q[0].push(0);
    vis[0]=1;
    while(1)
    {
        int __q=0;                      // select a queue.
        while(__q<N&&q[__q].empty())   __q++;
        if(__q==N)
        {
            T++;
            continue;
        }
        int top=q[__q].front();
        q[__q].pop();
        int timepiece=(1<<__q);
        if(remaintime[top].second<=timepiece)
        {
            timepiece=remaintime[top].second;
            res[top]=T+timepiece;
            fin++;
        }
        else
        {
            remaintime[top].second-=timepiece;
            if(__q==N-1)    q[__q].push(top);
            else q[__q+1].push(top);
        }
        if(fin==plist.size())   break;
        // add new processes to queues;
        bool added=0;
        for(int i=0;i<plist.size();i++)
        {
            if(plist[i].first<=T+timepiece&&vis[i]==0)
            {
                added=1;
                q[0].push(i);
                vis[i]=1;
            }
        }
        T+=timepiece;
    }
    return res;
}
int main()
{
    const int __width=8;
    auto data=genProcess(500);
    // for(auto e:data)
    //     cout<<e.first<<' '<<e.second<<'\n';
    
    auto fifo=FIFO(data);
    auto rr=RR(data);
    auto spn=SPN(data);
    auto srt=SRT(data);
    auto hrrn=HRRN(data);
    auto feedback=Feedback(data);
    cout<<"--------------------\n";
    cout<<setw(__width)<<"fifo"<<setw(__width)<<"rr"<<setw(__width)<<"spn"
        <<setw(__width)<<"srt"<<setw(__width)<<"hrrn"<<setw(__width)<<"fb"<<'\n';
    for(int i=0;i<fifo.size();i++)
    {
        cout<<setw(__width)<<fifo[i]<<setw(__width)<<rr[i]<<setw(__width)
            <<spn[i]<<setw(__width)<<srt[i]<<setw(__width)<<hrrn[i]<<setw(__width)<<feedback[i]<<'\n';
    }
    auto avg=[&](vector<int> arr)->double
    {
        double res=0;
        for(auto e:arr) res+=e;
        return res/arr.size();
    };
    cout<<setw(__width)<<avg(fifo)<<setw(__width)<<avg(rr)<<setw(__width)
            <<avg(spn)<<setw(__width)<<avg(srt)<<setw(__width)<<avg(hrrn)<<setw(__width)<<avg(feedback)<<'\n';
}