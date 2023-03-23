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
    const int rsize=5;
    vector<_process> remaintime=plist;
    int fin=0;
    int T=plist.front().first;
    list<int> running;
    running.push_back(0);
    while(fin!=plist.size())
    {
        for(auto p=running.rbegin();p!=running.rend();++p)
        {
            auto timepiece=min(remaintime[*p].second, rsize);
            if(remaintime[*p].second<=rsize)    // finish this process
            {
                fin++;
                // https://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
                // the 10000 times i think c++ stl sucks...
                res[*p]=T+timepiece-remaintime[*p].first;
                remaintime[*p].second-=timepiece;
                running.erase(next(p).base());
            }
            else    remaintime[*p].second-=timepiece;
            // add new proess to `running`
            for(int i=0;i<plist.size();i++)
            {
                if(remaintime[i].second==plist[i].second&&     // haven't been added to running list.
                plist[i].first>T&&plist[i].first<=T+timepiece)
                {
                    running.push_back(i);
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
    vector<_aux> auxlist(plist.size());
    for(int i=0;i<auxlist.size();i++)
        auxlist[i]={plist[i].first,plist[i].second,i};
    priority_queue<_aux> pq;
    pq.push(auxlist[0]);
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
            if(res[i]==0&&plist[i].first<=T+c.second&&plist[i].first>T)
                pq.push(auxlist[i]);
        }
        T+=c.second;
        if(fin==plist.size())   break;
        if(pq.empty())
        {
            auto it=lower_bound(plist.begin(),plist.end(),make_pair(T,0),[](_process a,_process b){
                return a.first<b.first;
            })-plist.begin();
            pq.push(auxlist[it]);
            T=plist[it].first;
        }
    }
    return res;
}
int main()
{
    auto data=genProcess(5000);
    // for(auto e:data)
    //     cout<<e.first<<' '<<e.second<<'\n';
    
    // auto fifo=FIFO(data);
    // auto rr=RR(data);
    // auto spn=SPN(data);
    // cout<<"--------------------\n";
    // cout<<setw(5)<<"fifo"<<setw(5)<<"rr"<<setw(5)<<"spn"<<'\n';
    // for(int i=0;i<fifo.size();i++)
    // {
    //     cout<<setw(5)<<fifo[i]<<setw(5)<<rr[i]<<setw(5)<<spn[i]<<'\n';
    // }
}