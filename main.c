/* Homework 5
 *
 * CPE-555
 * Stevens Institute of Technology
 * Spring 2017
 * Jiahui Chen
 * CWID:10422306
 * Description:using Pthreads on Linux that acts like a countdown timer alarm
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t timer_mutex;
pthread_cond_t timer_expired_cv;

void *Wait_notification(void *time)
{
  int WaitTime = *((int*)time);

  printf("Wait_notification started\n");

  while (WaitTime > 0)
  {
    /* sleep for 1 second */
    sleep(1);

    /* lock mutex */
    pthread_mutex_lock(&timer_mutex);

    /* WaitTime count down 1 s */
    WaitTime--;

    /* if timer expired, signal the alarm thread */
    if (WaitTime == 0)
    {
      pthread_cond_signal(&timer_expired_cv);
    }

    /* unlock the mutex */
    pthread_mutex_unlock(&timer_mutex);
  }

  /* thread is finished */
  pthread_exit(NULL);
}

void *Alarm_thread(void *t)
{
  printf("Alarm_thread started\n");

  /* lock mutex */
  pthread_mutex_lock(&timer_mutex);

  /* wait for signal */
  pthread_cond_wait(&timer_expired_cv, &timer_mutex);

  /* unlock mutex */
  pthread_mutex_unlock(&timer_mutex);

  /* print message */
  printf("The timer has  expired!\n");

  /* thread has finished */
  pthread_exit(NULL);
}

int main()
{
  pthread_t wait, alarm; /* two threads */
  pthread_attr_t attr;
  int timer = 0;

  /* prompt user to cin number of seconds for set timer */
  printf("Set seconds for timer: ");
  scanf("%d", &timer);

  /* initialize mutex and condition variable objects */
  pthread_mutex_init(&timer_mutex, NULL);
  pthread_cond_init (&timer_expired_cv, NULL);

  /* create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&wait,  &attr, Wait_notification,  (void*)&timer);
  pthread_create(&alarm, &attr, Alarm_thread, NULL);

  /* Wait for all threads to complete */
  pthread_join(wait, NULL);
  pthread_join(alarm, NULL);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&timer_mutex);
  pthread_cond_destroy(&timer_expired_cv);
  pthread_exit (NULL);
}
