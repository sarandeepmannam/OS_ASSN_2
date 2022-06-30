#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include<time.h>
#include <stdbool.h>
#include <atomic>
#include <random>
#include <limits.h>

using namespace std;
float lambda1 , lambda2 ;// Average time in CS and remainder section respectively
int n,k;//No of threads and frequency 
FILE* fp;
 struct Time
{
  int hr;
  int min;
  int sec;
};
typedef struct Time time_x;
time_x getSysTime()// Function to get the time 
{  
  time_x x;
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  x.hr=timeinfo->tm_hour;
  x.min=timeinfo->tm_min;
  x.sec=timeinfo->tm_sec;
  return x;
}

float max_time(float t[],int n)//Function to find the worst case running time of a process
{
  float temp=t[0];
  for(int i=1;i<n;i++)
  {
    if(t[i]>temp)
    {
      temp=t[i];
    }
  }
  return temp;
}
atomic<int> lock(0);

void *function1(void *p)// function where new thread execution begins
{
  default_random_engine generator;
  exponential_distribution<double> d1(1.0/lambda1); // critical section time	
exponential_distribution<double> d2(1.0/lambda2); // remainder section time 	
 time_x time,time1,time2;
  float t=0;
  float t1[k];
   pthread_t x=*((pthread_t*)p);
  
  for(int i=0;i<k;i++)
    {
      time = getSysTime();
      time1= getSysTime();
    fprintf(fp,"%dth CS request at %d:%d:%d by thread %lu\n",i+1,time.hr,time.min,time.sec,x);
      
      while (1){int x =0 , y=1;			if(lock.compare_exchange_strong(x,y)) break;} 
     
       time = getSysTime();
       time2 = getSysTime();
       t= (time2.hr-time1.hr)*3600+(time2.min-time1.min)*60+(time2.sec-time1.sec)+t;
      t1[i]=(time2.hr-time1.hr)*3600+(time2.min-time1.min)*60+(time2.sec-time1.sec);
     fprintf(fp,"%dth CS enter at %d:%d:%d by thread %lu\n",i+1,time.hr,time.min,time.sec,x);
/* critical section */
      usleep(d1(generator)*1e6); 
      lock=0;//exit section
      time = getSysTime();
      fprintf(fp,"%dth CS exit at %d:%d:%d by thread %lu\n",i+1,time.hr,time.min,time.sec,x);
/* remainder section */
      usleep(d2(generator)*1e6);
    }
  printf("Average waiting time of thread %lu is: %f seconds\n",x,float(t/k));
  printf("Worst case waiting time of thread %lu is: %f seconds\n",x,max_time(t1,k));
pthread_exit(0);
}
  
//Parallel processing

int main()
{  
FILE* f;
char str[20];
printf("Enter the name of the input file:\n");
scanf("%s",str);
f=fopen(str,"r");//open file input.txt

if(f==NULL)
 {
   printf("File opening was unsuccessful");
   return EXIT_SUCCESS;
 }
   fscanf(f,"%d%d%f%f",&n,&k,&lambda1,&lambda2);// reading no of threads from the input file

fclose(f);
fp=fopen("output_CAS.txt","w");
printf("%d %d %f %f\n",n,k,lambda1,lambda2);

pthread_t tid[n];//Identifiers for the threads we are going to create
  for(int i=0;i<n;i++)
   {
     pthread_create(&tid[i],NULL,function1,(void*)(&tid[i]));
    // In parallel processing all threads run together in parallel
    // So first we create all threads and thread execution start from my_thread function
   }
  for(int i=0;i<n;i++)
   {
     pthread_join(tid[i],NULL);
     //Now main thread waits for all the child threads to end by calling the pthread_join function
   }
fclose(fp);
	return 0;
}