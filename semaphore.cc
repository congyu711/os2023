#include <semaphore.h>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void s(int i)
{
    cout << "function " << i << '\n';
}
int main()
{
    sem_t sems[7];
    for (int i = 0; i < 7; i++)
        sem_init(sems + i, 0, 0);
    auto lambda1 = [&]()
    {
        sleep(3);
        s(1);
        sem_post(sems);
        sem_post(sems + 1);
    };
    auto lambda2 = [&]()
    {
        sem_wait(sems);
        sleep(3);
        s(2);
        // sem_post(sems + 2);
        // sem_post(sems + 3);
    };
    auto lambda3 = [&]()
    {
        sem_wait(sems + 1);
        s(3);
        // sem_post(sems + 6);
    };
    thread s1(lambda1);
    thread s2(lambda2);
    thread s3(lambda3);
    s1.join();
    s2.join();
    s3.join();
}