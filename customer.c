/* Skeleton code for the customer program. 
 *
 * Compile as follows: gcc -o customer customer.c -std=c99 -Wall -lpthread -lm 
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

#define MIN_TIME 2
#define MAX_TIME 10

/* Simulate walking to barber shop, by sleeping for some time between [min. max] in seconds */
void
walk (int min, int max) 
{
    sleep ((int) floor((double) (min + ((max - min + 1) * ((float) rand ()/(float) RAND_MAX)))));
    return;
}

int 
main (int argc, char **argv)
{
    srand (time (NULL)); 

    int customer_number = atoi (argv[1]);
    /* FIXME: unpack the semaphore names from the rest of the arguments */
    sem_t *bed;
    bed = sem_open (argv[2], 0); //bed
    if (bed == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }
    sem_t *barber_chair;
    barber_chair = sem_open (argv[3], 0); //chair barber
    if (barber_chair == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }
    sem_t *done;  
    done = sem_open (argv[4], 0); //done
    if (done == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }
    sem_t *num_chair;  
    num_chair = sem_open (argv[5], 0); //number of wait chairs available
    if (num_chair == SEM_FAILED) {
        perror ("sem_open");
        exit (EXIT_FAILURE);
    }

    printf ("Customer %d: Walking to barber shop\n", customer_number);
    walk (MIN_TIME, MAX_TIME);

    /* FIXME: Get hair cut by barber and go home. */

    int bed_avail;
    if (sem_getvalue(bed, &bed_avail) == -1)
    {
        perror ("sem_getvalue");
        exit (EXIT_FAILURE);
    }
    int bchair_avail;
    if (sem_getvalue(barber_chair, &bchair_avail) == -1)
    {
        perror ("sem_getvalue");
        exit (EXIT_FAILURE);
    }
    int num_chair_avail;
    if (sem_getvalue(num_chair, &num_chair_avail) == -1)
    {
        perror ("sem_getvalue");
        exit (EXIT_FAILURE);
    }
    printf("Customer %d: Reached the shop! \n", customer_number);
    if (bed_avail<=0){
            sem_post(bed);
            printf("Baber sleeping, Wake up! \n");
    }
    if (num_chair_avail == 0){
        printf("Customer %d: Enter the shop but no chair. Went home!\n", customer_number);
        exit(EXIT_SUCCESS);
    }
    else if (num_chair_avail > 0){
        printf("Customer %d: Entered WAITING AREA \n", customer_number);
        sem_wait(num_chair); //took a wait chair, number of wait chair avail --
        printf("Customer %d: Getting hair cut\n", customer_number);
        sem_wait(done); //take barber chair if it ready
        sem_post(barber_chair); //release the chair when done
    }

    printf ("Customer %d: all done\n", customer_number);

    exit (EXIT_SUCCESS);
}
