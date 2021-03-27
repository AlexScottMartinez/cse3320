#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX 5000000

int n1,n2;
int NUM_THREADS; 
char *s1,*s2;
FILE *fp;

typedef struct str_Data
{
	int threadNum;
	int partialCount;
} Data;

//	Function that reads the textfile and puts the text into local variables
int readf(char* filename)
{
    if((fp=fopen(filename, "r"))==NULL)
    {
        printf("ERROR: can’t open %s!\n", filename);
        return 0;
    }
    
    s1=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory!\n") ;
        return -1;
    }
    
    s2=(char *)malloc(sizeof(char)*MAX);
    
    if (s1==NULL)
    {
        printf ("ERROR: Out of memory\n") ;
        return -1;
    }
    
    /*read s1 s2 from the file*/   
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1); /*length of s1*/
    n2=strlen(s2)-1; /*length of s2*/

    if( s1==NULL || s2==NULL || n1 < n2 ) /*when error exit*/
    {
        return -1;
    }
}

//	Function that finds the number of instances found without threads
int numSubstring ( void )
{
	int i,j,k;
    	int count;
	int total = 0;
    	for (i = 0; i <= (n1-n2); i++)
    	{
        	count =0;
        	for(j = i ,k = 0; k < n2; j++,k++)
        	{ /*search for the next string of size of n2*/
            		if (*(s1+j)!=*(s2+k))
            		{
                		break;
            		}
            		else
            		{
                		count++;
            		}
            		if (count==n2)
			{
                		total++; /*find a substring in this step*/
			}
         	}
    	}
    	return total ;
}

//	Function that finds the number of instances found with threads
int numSubstringThreads(void *ptr)
{
	int a,b,c;
	int count;
	int total = 0;
    	for (a =((Data*) ptr)->threadNum; a <= (n1-n2); a = a + NUM_THREADS)
    	{
        	count = 0;
        	for(b = a ,c = 0; c < n2; b++,c++)
       		{ /*search for the next string of size of n2*/
            		if (*(s1+b)!=*(s2+c))
            		{
                		break;
            		}
            		else
            		{
                		count++;
            		}
            		if (count==n2)
			{
                		total++; /*find a substring in this step*/
			}
         	}
    	}
    	((Data*)ptr)->partialCount = total;
}
    
int main(int argc, char *argv[])
{ 
	int count;
	/*Makes sure that the user enters a .txt in the command line arguement when compiling*/	
    	if( argc < 2 )
    	{
      		printf("Error: You must pass in the datafile as a commandline parameter\n");
    	}

    	readf ( argv[1] ) ;
    	struct timeval start, start2, end, end2;
    	float mtime, mtime2; 
    	int secs, secs2, usecs, usecs2;  

	/*Calculate the elapsed time without threads*/
	gettimeofday(&start, NULL);
    	count = numSubstring () ;
    	gettimeofday(&end, NULL);
    	secs  = end.tv_sec  - start.tv_sec;
    	usecs = end.tv_usec - start.tv_usec;
    	mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    	printf ("The number of substrings without threads is : %d\n", count) ;
    	printf ("Elapsed time is : %f milliseconds\n\n", mtime );

	/*Calculate the elapsed time with threads
	  Creates the threads and termination of threads*/
	pthread_t thread0, thread1, thread2, thread3;
	Data data0, data1, data2, data3;
	int partialSum;
	data0.threadNum = 0;
	data1.threadNum = 1;
	data1.threadNum = 2;
	data1.threadNum = 3;
	printf("How many threads do you want to use to compile the program (1,2,4): ");
	scanf("%d", &NUM_THREADS);
	gettimeofday(&start2, NULL);
	if (NUM_THREADS == 1)
	{
		pthread_create (&thread0, NULL, (void *) &numSubstringThreads, (void *) &data0);
		partialSum = 0;
		pthread_join(thread0, NULL);
		partialSum += data0.partialCount;
		gettimeofday(&end2, NULL);
    		secs2  = end2.tv_sec  - start2.tv_sec;
    		usecs2 = end2.tv_usec - start2.tv_usec;
   		mtime2 = ((secs2) * 1000 + usecs2/1000.0) + 0.5;
		printf("The number of substrings with threads is : %d\n", partialSum);
		printf("Elapsed time is : %f milliseconds\n", mtime2);
	}
	else if (NUM_THREADS == 2)
	{
		pthread_create (&thread0, NULL, (void *) &numSubstringThreads, (void *) &data0);
		pthread_create (&thread1, NULL, (void *) &numSubstringThreads, (void *) &data1);
		partialSum = 0;
		pthread_join(thread0, NULL);
		partialSum += data0.partialCount;
		pthread_join(thread1, NULL);
		partialSum += data1.partialCount;
		gettimeofday(&end2, NULL);
    		secs2  = end2.tv_sec  - start2.tv_sec;
    		usecs2 = end2.tv_usec - start2.tv_usec;
   		mtime2 = ((secs2) * 1000 + usecs2/1000.0) + 0.5;
		printf("The number of substrings with threads is : %d\n", partialSum);
		printf("Elapsed time is : %f milliseconds\n", mtime2);
	}
	else if (NUM_THREADS == 4)
	{
		pthread_create (&thread0, NULL, (void *) &numSubstringThreads, (void *) &data0);
		pthread_create (&thread1, NULL, (void *) &numSubstringThreads, (void *) &data1);
		pthread_create (&thread2, NULL, (void *) &numSubstringThreads, (void *) &data2);
		pthread_create (&thread3, NULL, (void *) &numSubstringThreads, (void *) &data3);
		partialSum = 0;
		pthread_join(thread0, NULL);
		partialSum += data0.partialCount;
		pthread_join(thread1, NULL);
		partialSum += data1.partialCount;
		pthread_join(thread2, NULL);
		partialSum += data2.partialCount;
		pthread_join(thread3, NULL);
		partialSum += data3.partialCount;
		gettimeofday(&end2, NULL);
    		secs2  = end2.tv_sec  - start2.tv_sec;
    		usecs2 = end2.tv_usec - start2.tv_usec;
   		mtime2 = ((secs2) * 1000 + usecs2/1000.0) + 0.5;
		printf("The number of substrings with threads is : %d\n", partialSum);
		printf("Elapsed time is : %f milliseconds\n", mtime2);
	}
	else
	{
		printf("Number of threads entered is not valid.\n");
	}
	

	/*Deallocates the memory*/
    	if( s1 )
    	{
      		free( s1 );
    	}
    	if( s2 )
    	{
      		free( s2 );
    	}

	return 0 ; 
}
