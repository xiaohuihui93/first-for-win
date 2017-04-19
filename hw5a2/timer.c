/*
 * timer.c
 * CPE555 Real Time and Embedded Systems HW5
 *
 *  Created on: 2017.4.18
 *  Author: Erwei Zhu
 *
 *  Detail: The program using Pthreads on Linux
 *          that acts like a count down timer alarm
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 2
#define TIMER_EXPIRED 0

pthread_mutex_t timer_mutex;
pthread_cond_t timer_expired_cv;

void *notification(void *time)
{
	printf("Notification thread starts.\n");

	int wait_time =*((int*) time);
        while(wait_time>0)
        {
            /*sleep for 1 second*/
            sleep(1);

            /*lock mutex*/
            pthread_mutex_lock(&timer_mutex);

            /*wait until timer has expired*/
            wait_time--;

            /*timer has expired, print thread start*/
            if(wait_time==TIMER_EXPIRED)
            {
                    pthread_cond_signal(&timer_expired_cv);
            }

            /*unlock mutex*/
            pthread_mutex_unlock(&timer_mutex);
        }

        /*thread is finished*/
        pthread_exit(NULL);
}

void *print(void *t)
{
	printf("Print thread starts.\n");

	/*lock mutex*/
	pthread_mutex_lock(&timer_mutex);

	/*waiting when condition is reached*/
	pthread_cond_wait(&timer_expired_cv,&timer_mutex);

	/* unlock mutex */
	pthread_mutex_unlock(&timer_mutex);

	/* print a message that the timer has expired */
	printf("Timer has expired!\n");

	/* thread is done */
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;

   /*Prompt the user for number of seconds to set the timer    */
   int timer =0;
   printf("Set the timer(seconds):  ");
   scanf("%d",&timer);

   /*Initialize mutex and condition variable ojects*/
   pthread_mutex_init(&timer_mutex,NULL);
   pthread_cond_init(&timer_expired_cv,NULL);

   /*Explicitly create threads in a joinable state*/
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
   pthread_create(&threads[0],&attr,notification,(void*)&timer);
   pthread_create(&threads[1],&attr,print,NULL);

   /*Wait for all threads to complete*/
   for(int i=0;i<NUM_THREADS;i++){
	   pthread_join(threads[i],NULL);
   }

   /* Clean up and exit */
     pthread_attr_destroy(&attr);
     pthread_mutex_destroy(&timer_mutex);
     pthread_cond_destroy(&timer_expired_cv);
     pthread_exit (NULL);

}
