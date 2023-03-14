#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

int main()
{
    if (fork() == 0)
    {
        cout << "child process\n";
        cout << getpid() << ' ' << getppid()<<endl;
    }
    else
    {
        cout << "parent process\n";
        cout << getpid() << ' ' << getppid()<<endl;
    }
}