#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// These will be initialized in main() from the command line.
int seat_count;
int broker_count;
int *seat_taken;  // Array of seats
int transaction_count;

int seat_taken_count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


/* This function is unneccessary, as it can be replaced in the if statements where it would have been called
Doing so removes the need for a recursive lock. */

// int is_free(int n) {
//     pthread_mutex_lock(&lock); //lock the mutex
//     if (seat_taken[n] == 0) {   //check if the seat is free
//         pthread_mutex_unlock(&lock);   //unlock the mutex
//         return 1;
//     } else {
//         pthread_mutex_unlock(&lock);   //unlock the mutex
//         return 0;
//     }
// }

int reserve_seat(int n)
{
    // Attempt to reserve seat number n
    pthread_mutex_lock(&lock); //lock the mutex
    if (seat_taken[n]==0) { //if the seat wasn't already taken.
        seat_taken[n] = 1;  //mark the seat as taken and return 0
        seat_taken_count++;  //increment seat_taken_count 
        pthread_mutex_unlock(&lock);   //unlock the mutex
        return 0;
    } else {    // If the seat is already taken, return -1
        pthread_mutex_unlock(&lock);   //unlock the mutex
        return -1;
    }
}

int free_seat(int n)
{
    pthread_mutex_lock(&lock); //lock the mutex
    if (seat_taken[n]==0) { //if the seat is already free, return -1
        pthread_mutex_unlock(&lock);   //unlock the mutex
        return -1;
    } else {    // Otherwise mark the seat as free and return 0
        seat_taken[n] = 0;
        seat_taken_count--; //decrement seat_taken_count
        pthread_mutex_unlock(&lock);   //unlock the mutex
        return 0;
    }
}

int verify_seat_count(void) {
    int tmp_seat_taken_count = 0;
    
    pthread_mutex_lock(&lock); //lock the mutex
    for (int i = 0; i < seat_count; i++){   //iterate through the seat_taken array and count all taken seats.
        if (seat_taken[i]){
            tmp_seat_taken_count++;
        }
    }
    int result = tmp_seat_taken_count == seat_taken_count;    //return true if the count from the array is the same as the seat_taken_coun
    pthread_mutex_unlock(&lock);   //unlock the mutex
    return result;
}

// ------------------- DO NOT MODIFY PAST THIS LINE -------------------

void *seat_broker(void *arg)
{
    int *id = arg;

    for (int i = 0; i < transaction_count; i++) {
        int seat = rand() % seat_count;
        if (rand() & 1) {
            // buy a random seat
            reserve_seat(seat);

        } else {
            // sell a random seat
            free_seat(seat);
        }

        if (!verify_seat_count()) {
            printf("Broker %d: the seat count seems to be off! " \
                   "I quit!\n", *id);
            return NULL;
        }
    }

    printf("Broker %d: That all seemed to work very well.\n", *id);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Parse command line
    if (argc != 4) {
        fprintf(stderr, "usage: reservations seat_count broker_count xaction_count\n");
        exit(1);
    }

    seat_count = atoi(argv[1]);
    broker_count = atoi(argv[2]);
    transaction_count = atoi(argv[3]);

    // Allocate the seat-taken array
    seat_taken = calloc(seat_count, sizeof *seat_taken);

    // Allocate thread handle array for all brokers
    pthread_t *thread = calloc(broker_count, sizeof *thread);

    // Allocate thread ID array for all brokers
    int *thread_id = calloc(broker_count, sizeof *thread_id);

    srand(time(NULL) + getpid());
    
    // Launch all brokers
    for (int i = 0; i < broker_count; i++) {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, seat_broker, thread_id + i);
    }

    // Wait for all brokers to complete
    for (int i = 0; i < broker_count; i++)
        pthread_join(thread[i], NULL);
}

