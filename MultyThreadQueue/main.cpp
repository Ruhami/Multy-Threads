#include <iostream>
#include "Queue.h"
#include <stdlib.h>


void* PushTest(void* args)
{
    int rand_a;
    rand_a = rand()%10;
    std::cout<<"in thread push"<<std::endl;

    ((SafeQueue<int>*)args) -> push(rand_a);

    pthread_exit(NULL);
}


void* PopTest(void* args)
{
    std::cout<<"in thread pop"<<std::endl;

    std::cout << ((SafeQueue<int>*)args) -> pop()<<std::endl;

    pthread_exit(NULL);
}

int main()
{
    SafeQueue<int> q;
    pthread_t pushThreads[3];
    pthread_t popThreads[3];


    for(int i = 0; i < 3; ++i)
    {
        pthread_create(&pushThreads[i], NULL, PushTest, (void*)&q);
        std::cout<<"created thread " << i <<std::endl;
    }

    for(int i = 0; i < 3; ++i)
    {
        pthread_create(&popThreads[i], NULL, PopTest, (void*)&q);
        std::cout<<"created thread "<< (i+3) << std::endl;
    }

    for(int i = 0; i < 3; ++i)
    {
        pthread_join(pushThreads[i], NULL);
    }

    for(int i = 0; i < 3; ++i)
    {
        pthread_join(popThreads[i], NULL);
    }

    return 0;
}
