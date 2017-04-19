/* Homework 5 Solution
 *
 * CPE-555
 * Stevens Institute of Technology
 * Fall 2016
 *
 * To compile: gcc homework_5_solution.c -pthread -o hw5_sol
 * To run: ./hw5_sol
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t timer_mutex;
pthread_cond_t timer_expired_cv;

void *wait_thread(void *time) 
{
  int time_to_wait = *((int*)time);

  printf("wait_thread started\n");

  while (time_to_wait > 0)
  {
    /* sleep for 1 second */
    usleep(1000000);

    /* lock mutex */
    pthread_mutex_lock(&timer_mutex);

    /* decrement time to wait */
    --time_to_wait;

    /* if timer expired, signal the printer thread */
    if (time_to_wait == 0)
    {
      pthread_cond_signal(&timer_expired_cv);
    }
    
    /* unlock the mutex */
    pthread_mutex_unlock(&timer_mutex);  
  }

  /* thread is finished */
  pthread_exit(NULL);
}

void *print_thread(void *t) 
{
  printf("print_thread started\n");

  /* lock mutex */
  pthread_mutex_lock(&timer_mutex);

  /* wait for signal */
  pthread_cond_wait(&timer_expired_cv, &timer_mutex);

  /* unlock mutex */
  pthread_mutex_unlock(&timer_mutex);

  /* print message */
  printf("Timer expired!\n");

  /* thread is done */
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t waiter, printer; /* two threads */
  pthread_attr_t attr;
  int timeout = 0;

  /* prompt user for timeout */
  printf("Enter seconds to wait: ");
  scanf("%d", &timeout);

  /* initialize mutex and condition variable objects */
  pthread_mutex_init(&timer_mutex, NULL);
  pthread_cond_init (&timer_expired_cv, NULL);

  /* create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&waiter,  &attr, wait_thread,  (void*)&timeout);
  pthread_create(&printer, &attr, print_thread, NULL);

  /* Wait for all threads to complete */
  pthread_join(waiter, NULL);
  pthread_join(printer, NULL);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&timer_mutex);
  pthread_cond_destroy(&timer_expired_cv);
  pthread_exit (NULL);
}

