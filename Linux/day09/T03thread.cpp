
#include "../header.h"
#include <list>
using namespace std;

list<int> l;
int a  = 0;

void* thread_func(void* ptr)
{
    int i;
    for(i=0; i<100000; ++i)
    {
        if(l.size() > 0)
            l.pop_back();
        a++;
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    int i;
    for(i=0; i<100000; ++i)
    {
        a++;
        l.push_back(a);
    }

    pthread_join(tid, NULL);
    printf("a is %d\n", a); // 200000
}
