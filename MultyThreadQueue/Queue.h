#ifndef __SAFEQUEUE_H__
#define __SAFEQUEUE_H__
#include <iostream>
#include <queue>
#include <pthread.h>
#include <semaphore.h>

template<typename T, int SIZE = 30>
class SafeQueue
{
public:
    SafeQueue();
    ~SafeQueue();
    void push (const T& val);
    T pop();

private:
    std::queue<T> m_queue;

    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
};

template<typename T, int SIZE >
SafeQueue<T, SIZE>::SafeQueue()
{
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
	int status = pthread_mutex_init(&mutex, NULL);
}

template<typename T, int SIZE >
SafeQueue<T, SIZE>::~SafeQueue()
{
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
}

template<typename T, int SIZE >
void SafeQueue<T, SIZE>::push (const T& val)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    m_queue.push(val);
    std::cout<<"in push"<<std::endl;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

template<typename T, int SIZE >
T SafeQueue<T, SIZE>::pop()
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    T item = m_queue.front();
    m_queue.pop();
    std::cout<<"in pop"<<std::endl;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return item;
}

#endif //__SAFEQUEUE_H__
