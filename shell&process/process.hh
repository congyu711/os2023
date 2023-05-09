#include<bits/stdc++.h>
using namespace std;

enum _status
{
    ready = 0,
    running,
    block
};


class process
{
public:
    int priority;
    int pid;
    string pname;
    _status status;
    list<process*> children;
    process* parent;
    list<pair<int,int>> reqres;   //required resources
    process(int _pid, string& name,int pri):pid(_pid),pname(name),parent(nullptr),status(ready),priority(pri){}
};


list<process*> allproc;