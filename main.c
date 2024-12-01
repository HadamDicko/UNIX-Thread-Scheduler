/* ********************************************************************** *
 * Hadam Dicko: Thread Scheduler Implementation
 * ********************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#define SCHEDULE_INTERVAL  1           /* scheduling interval in second */
#define SLOWDOWN_FACTOR   1000000     /* child thread display slowdown */

int schedule_vector[5] = {0, 1, 2, 3, 4};

/* Function prototypes */
void clock_interrupt_handler(void);
void child_thread_routine(void* arg);

/* Global variables from samples */
pthread_mutex_t condition_mutex;       /* pthread mutex_condition */
pthread_cond_t t_condition;           /* pthread condition */
int current_thread = -1;              /* currently running thread */
int interrupt_count = 0;              /* number of interrupts */
volatile int thread_count = 0;        /* count of initialized threads */

int main(void) {
    pthread_t threads[5];            /* thread IDs */
    int thread_args[5];             /* arguments for threads */
    int i;

    /* Initialize pthread mutex and condition variable */
    pthread_mutex_init(&condition_mutex, NULL);
    pthread_cond_init(&t_condition, NULL);

    /* Create child threads one at a time */
    for (i = 0; i < 5; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i],
                      NULL,
                      (void *)(child_thread_routine),
                      &thread_args[i]);
        usleep(100000);  /* Small delay between thread creation */
    }

    /* Wait for all threads to initialize */
    while (thread_count < 5) {
        usleep(100000);
    }

    /* Set up interrupt handler */
    signal(SIGALRM, (void *)(clock_interrupt_handler));
    alarm(SCHEDULE_INTERVAL);

    /* Parent freezes using sample code mechanism */
    while (1) {
        pthread_mutex_lock(&condition_mutex);
        pthread_cond_wait(&t_condition, &condition_mutex);
        pthread_mutex_unlock(&condition_mutex);
    }

    return 0;
}

void clock_interrupt_handler(void) {
    printf("I woke up on the timer interrupt (%d)\n", interrupt_count);

    pthread_mutex_lock(&condition_mutex);
    current_thread = schedule_vector[interrupt_count % 5];
    interrupt_count++;
    pthread_cond_broadcast(&t_condition);
    pthread_mutex_unlock(&condition_mutex);

    alarm(SCHEDULE_INTERVAL);
}

void child_thread_routine(void* arg) {
    int myid = *(int*)arg;
    int my_counter = 0;

    /* Required thread start message */
    printf("Child thread %d started ...\n", myid);

    /* Signal thread initialization */
    pthread_mutex_lock(&condition_mutex);
    thread_count++;
    pthread_mutex_unlock(&condition_mutex);

    /* Infinite loop (required) */
    while (1) {
        pthread_mutex_lock(&condition_mutex);
        while (current_thread != myid) {
            pthread_cond_wait(&t_condition, &condition_mutex);
        }
        pthread_mutex_unlock(&condition_mutex);

        /* Required thread running message */
        my_counter++;
        if ((my_counter % SLOWDOWN_FACTOR) == 0) {
            printf("Thread: %d is running ...\n", myid);
        }
    }
}
