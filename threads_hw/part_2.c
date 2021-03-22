#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 5

pthread_mutex_t t_mutex;
pthread_cond_t cond_c, cond_p;
char ch;
FILE *fp;
//circular queue
char items[SIZE];
int front = -1, rear = -1;

int isFull()
{
    if((front == rear + 1 )||(front == 0 && rear == SIZE-1))
    {
        return 1;

    }
    return 0;

}
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
void* producer(void *ptr)
{
    while(!feof(fp))
    {
        pthread_mutex_lock(&t_mutex);
        while(isFull())
        {
            pthread_cond_wait(&cond_p,&t_mutex);

        }
        fread(&ch, 1, 1, fp);
        enQueue(ch);
        pthread_cond_signal(&cond_c);
        pthread_mutex_unlock(&t_mutex);

    }
    pthread_exit(0);

}
void* consumer(void *ptr)
{
    while(!feof(fp))
    {
        pthread_mutex_lock(&t_mutex);
        while(isEmpty())
        {
            pthread_cond_wait(&cond_c,&t_mutex);

        }
        printf("%c", deQueue());
        pthread_cond_signal(&cond_p);
        pthread_mutex_unlock(&t_mutex);

    }
    pthread_exit(0);

}
int main(int argc, char **argv)
{
    pthread_t pro, con;
    char fname[20];
    printf("Enter the filename: ");
    scanf("%s", fname);
    fp = fopen(fname, "r");
    if (fp == 0)
    {
        printf("\nFailed to open message.txt for reading\n");

    }
    printf("\n File message.txt opened for reading\n");
    pthread_mutex_init(&t_mutex, NULL);
    pthread_cond_init(&cond_c, NULL);
    pthread_cond_init(&cond_p, NULL);
    pthread_create(&con, NULL, consumer, NULL);
    pthread_create(&pro, NULL, producer, NULL);
    pthread_join(con, NULL);
    pthread_join(pro, NULL);
    pthread_mutex_destroy(&t_mutex);
    pthread_cond_destroy(&cond_c);
    pthread_cond_destroy(&cond_p);
    
    // Time
    struct timeval start, end;
    float mtime; 
    int secs, usecs;    

    gettimeofday(&start, NULL);

    gettimeofday(&end, NULL);

    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

    printf ("Elapsed time is : %f milliseconds\n", mtime );
    fclose(fp);

}
