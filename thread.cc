#include<iostream>
#include<thread>
using namespace std;

int main()
{
    thread t([](){cout<<"thread1\n";});
    t.detach();
    cout<<t.hardware_concurrency<<endl;
}