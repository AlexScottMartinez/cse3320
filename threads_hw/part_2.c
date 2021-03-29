#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 5

pthread_mutex_t count_mutex;
pthread_cond_t cond_consumer, cond_producer;
char chtr;
FILE *fp;
char items[SIZE];
int front = -1, rear = -1;

// Checks to see if the queue is full 
int isFull()
{
	if((front == rear + 1 )||(front == 0 && rear == SIZE-1))
    	{
        	return 1;

    	}
    	return 0;
}

// Checks to see if the queue is empty
int isEmpty()
{
    	if(front == -1)
    	{
        	return 1;

    	}
    	return 0;
}

// Insert element into queue 
void enQueue(int element)
{
    	if(isFull())
    	{
        	printf("\n Queue is full!!\n");

    	}
    	else
    	{
        	if(front == -1)
        	{
            		front = 0;

        	}
        	rear = (rear + 1)% SIZE;
        	items[rear] = element;
    	}
}

// remove single element from queue and return
int deQueue()
{
    	int element;
    	if (isEmpty())
    	{
        	printf("\nQueue is empty !!\n");
        	return -1;

    	}
    	else
    	{
        	element = items[front];
        	if (front == rear)
        	{
            		front = -1;
            		rear = -1;

        	}
        	else 
        	{
            		front = (front + 1)% SIZE;

        	}
        	return(element);
    	}
}

// Function puts the text of the file into the circular queue
void* producer(void *ptr)
{
	while(!feof(fp))
    	{
        	pthread_mutex_lock(&count_mutex);
        	while(isFull())
        	{
            		pthread_cond_wait(&cond_producer,&count_mutex);

        	}
        	fread(&chtr, 1, 1, fp);
        	enQueue(chtr);
        	pthread_cond_signal(&cond_consumer);
        	pthread_mutex_unlock(&count_mutex);

    	}
    	pthread_exit(0);
}

// Function prints out the text of the file from the circular queue 
void* consumer(void *ptr)
{
	printf("\n");
	while(!feof(fp))
    	{
        	pthread_mutex_lock(&count_mutex);
        	while(isEmpty())
        	{
            		pthread_cond_wait(&cond_consumer,&count_mutex);

        	}
        	printf("%c", deQueue());
        	pthread_cond_signal(&cond_producer);
        	pthread_mutex_unlock(&count_mutex);

    	}
    	pthread_exit(0);
}

int main(int argc, char *argv[])
{
   // Time
	struct timeval start, end;
    	float mtime; 
    	int secs, usecs;
    	
    	pthread_t pro, con;
    	
   // File I/O
    	char fname[20];
    	printf("Enter the filename: ");
    	scanf("%s", fname);
    	fp = fopen(fname, "r");
   // Start Timer 	
	gettimeofday(&start, NULL);
	
    	if (fp == 0)
    	{
        	printf("\nFailed to open message.txt for reading\n");
        	return 0;

    	}
    	pthread_mutex_init(&count_mutex, NULL);
    	pthread_cond_init(&cond_consumer, NULL);
    	pthread_cond_init(&cond_producer, NULL);
    	pthread_create(&con, NULL, consumer, NULL);
    	pthread_create(&pro, NULL, producer, NULL);
    	pthread_join(con, NULL);
    	pthread_join(pro, NULL);
    	pthread_mutex_destroy(&count_mutex);
    	pthread_cond_destroy(&cond_consumer);
    	pthread_cond_destroy(&cond_producer);
       
   // End Timer
    	gettimeofday(&end, NULL);

   // Calculate Time for Completion of Program, Then print it out
    	secs  = end.tv_sec  - start.tv_sec;
    	usecs = end.tv_usec - start.tv_usec;
    	mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    	printf ("\nElapsed time is : %f milliseconds\n", mtime );
    	
    	fclose(fp);

}
