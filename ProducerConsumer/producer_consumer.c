#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#define N 6

sem_t mutex;
sem_t empty;
sem_t full;
pthread_mutex_t count_c_mutex;
pthread_mutex_t count_p_mutex;
int count_c = 0;
int count_p = 0;
int count = 0;
int boxes[6];

void init_semaphores()
{
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);
}

int produceItem()
{
	int item;
	printf("----------produce item----------\n");
	item = rand() % 1000001;  
	return item;
}

void consumeItem(int i)
{
	printf("----------consume item----------\n");
}

void insertItem(int i)
{
	boxes[count] = i;
	count++;
	printf("----------insert item---------\n");
}

int removeItem()
{
	int x = count;
	count--;
	printf("----------remove item----------\n");
	return boxes[x];
}

void doun()
{
}

void *producer()
{
	int item;
	int i;
	while (count_p < 120)
	{
		pthread_mutex_lock(&count_p_mutex);
		if (count_p == 120)
		{
			pthread_mutex_unlock(&count_p_mutex);
			break;
		}
		++count_p;
		printf("p %d", count_p);
		pthread_mutex_unlock(&count_p_mutex);
		item = produceItem();
		sem_wait(&empty);
		sem_wait(&mutex);
		insertItem(item);
		sem_post(&mutex);
		sem_post(&full);
	}
	
}

void *consumer()
{
	int item;
	int i;
	pthread_mutex_lock(&count_c_mutex);
	while (count_c < 120)
	{
		++count_c;
		printf("c %d", count_c);
		pthread_mutex_unlock(&count_c_mutex);
		
		sem_wait(&full);
		sem_wait(&mutex);
		item = removeItem();
		sem_post(&mutex);
		sem_post(&empty);
		consumeItem(item);
		pthread_mutex_lock(&count_c_mutex);
	}
	pthread_mutex_unlock(&count_c_mutex);
}

int main()
{
	int i;
	pthread_t producers[3];
	pthread_t consumers[4];
	
	init_semaphores();
	
	for(i = 0; i < 3; ++i)
		pthread_create(&producers[i], NULL, producer, NULL);
		
	for(i = 0; i < 4; ++i)
		pthread_create(&consumers[i], NULL, consumer, NULL);
		
	for(i = 0; i < 3; ++i)
		pthread_join(producers[i], NULL);

	for(i = 0; i < 4; ++i)
		pthread_join(consumers[i], NULL);
		
	return 0;
}
