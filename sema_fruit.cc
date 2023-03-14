#include <semaphore.h>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;
sem_t _mutex, full_a, full_o, _empty;

int main()
{
    sem_init(&_mutex, 0, 1);
    sem_init(&full_o, 0, 0);
    sem_init(&full_a, 0, 0);
    sem_init(&_empty, 0, 1);
    auto father = []()
    {
        int t = 20;
        while (t--)
        {
            sem_wait(&_empty);
            sem_wait(&_mutex);
            cout << "father put ";
            if (t%3)
            {
                cout << "apple\n";
                sem_post(&_mutex);
                sem_post(&full_a);
            }
            else
            {
                cout << "orange\n";
                sem_post(&_mutex);
                sem_post(&full_o);
            }
        }
    };
    auto son = []()
    {
        int t = 20;
        while (t--)
        {
            sem_wait(&full_a);
            sem_wait(&_mutex);
            cout << "son eat apple\n";
            sem_post(&_mutex);
            sem_post(&_empty);
        }
    };
    auto daughter = []()
    {
        int t = 20;
        while (t--)
        {
            sem_wait(&full_o);
            sem_wait(&_mutex);
            cout << "daughter eat orange\n";
            sem_post(&_mutex);
            sem_post(&_empty);
        }
    };
    thread father_t(father);
    thread son_t(son);
    thread daughter_t(daughter);
    father_t.join();son_t.join();daughter_t.join();
}