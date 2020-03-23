/* Skeleton code for the barber program. 
 *
 * Compile as follows: gcc -o barber barber.c -std=c99 -Wall -lpthread -lm 
 *
 * Author: Naga Kandasamy
 * Date: February 7, 2020
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#define MIN_TIME 2
#define MAX_TIME 5
#define TIME_OUT 10

static void alarm_handler (int); /* Signal handler to catch the ALRM signal */

/* Simulate cutting hair, by sleeping for some time between [min. max] in seconds */
void
cut_hair (int min, int max) 
{
    printf("Barber cutting hair for customer\n");
    sleep ((int) floor((double) (min + ((max - min + 1) * ((float) rand ()/(float) RAND_MAX)))));
    return;
}

sem_t *bed;
sem_t *barber_chair;
sem_t *done; 
sem_t *num_chair;  


int 
main (int argc, char **argv)
{
    srand (time (NULL)); 

    printf ("Barber: Opening up the shop\n");
    
    int waiting_room_size = atoi (argv[1]);
    printf ("Barber: size of waiting room = %d\n", waiting_room_size);

    /* Set up our signal handler to catch SIGALRM */
    if (signal (SIGALRM, alarm_handler) == SIG_ERR){ 
        printf ("Cannot catch SIGALRM \n");
        exit (0);
    }

    /* FIXME: Unpack the semaphore names from the rest of the command-line arguments 
     * and open them for use.
     */
    bed = sem_open (argv[2], 0); //bed
    if (bed == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }
    
    barber_chair = sem_open (argv[3], 0); //chair barber
    if (barber_chair == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }
     
    done = sem_open (argv[4], 0); //done
    if (done == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }

    num_chair = sem_open (argv[5], 0); //number of wait chairs available
    if (num_chair == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }

    /* FIXME: Stay in a loop, cutting hair for customers as they arrive. 
     * If no customer wakes the barber within TIME_OUT seconds, barber
     * closes shop and goes home.
     */
    int num_customers = 0;
    int wait_chair_avail = waiting_room_size;
    while (1){
        if (sem_getvalue(num_chair, &wait_chair_avail)==-1){
            perror ("sem_getvalue");
            exit (EXIT_FAILURE);
        }
        num_customers =  waiting_room_size - wait_chair_avail;
        if (num_customers == 0)
        {
            alarm(10);
            printf("No customer: Barber go to sleep\n");
            sem_wait(bed); //barber sleep
            alarm(0);
        }
        //sem_wait(barber_chair);
        else{
            sem_wait(barber_chair);
            sem_post(num_chair); //took customer to barber room      
            cut_hair (MIN_TIME, MAX_TIME);
            sem_post(done);
        }
    }

    printf ("Barber: all done for the day\n");

    exit (EXIT_SUCCESS);
}
static void 
alarm_handler (int sig)
{
    printf ("Timeout. Barber went home! \n");
    exit (EXIT_SUCCESS);   
    
}