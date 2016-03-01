#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define Q_LEN 1024
int buff[Q_LEN];
int front = 0;
int back = -1;
int size = 0;

int producerfd;
int consumerfd;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void *arg)
{
	//pthread_detach(pthread_self());

	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(size == Q_LEN)
		{
			printf("buffer full\n");
			pthread_cond_wait(&cond, &mutex);
		}
		
		back = (back + 1) % Q_LEN;
		buff[back] = random() % 100;
		printf("producer[%d:%d]\n", back, buff[back]);
		char wbuff[64] = { 0 };
		sprintf(wbuff, "%d\n", buff[back]);
		write(producerfd, (const void*)wbuff, strlen(wbuff));
		++size;
		if(size == 1)
		{
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
	}
}

void* consumer(void *arg)
{
	//pthread_detach(pthread_self());

	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(size == 0)
		{
			printf("buff empty\n");
			pthread_cond_wait(&cond, &mutex);
		}

		printf("consumer[%d:%d]\n", front, buff[front]);
		char wbuff[64] = { 0 };
		sprintf(wbuff, "%d\n", buff[front]);
		write(consumerfd, (const void*)wbuff, strlen(wbuff));
		front = (front + 1) % Q_LEN;
		--size;
		if(size == Q_LEN - 1)
		{
			pthread_cond_signal(&cond);
		}

		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	producerfd = open("producerfile", O_CREAT | O_TRUNC);
	consumerfd = open("consumerfile", O_CREAT | O_TRUNC);
	
	if(producerfd == -1 || consumerfd == -1)
	{
		printf("open file err\n");
		return -1;
	}

	srandom((unsigned int)time(NULL));

	pthread_t consumerid;
	pthread_t producerid;
	
	pthread_create(&consumerid, NULL, consumer, NULL);
	pthread_create(&producerid, NULL, producer, NULL);
	
	pthread_join(consumerid, NULL);
	pthread_join(producerid, NULL);

	sleep(1);
	
	close(producerfd);
	close(consumerfd);
	
	return 0;
}

