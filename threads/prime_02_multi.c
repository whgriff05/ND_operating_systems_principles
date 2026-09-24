/* prime_multi.c: brute-force prime finder (multi-threaded) */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

/* Macros */

#define max(a, b)   ((a) > (b) ? (a) : (b))

/* Constants */

const size_t NUM_THREADS = 8;
const size_t PRIME_MAX	 = 400000;

/* Structure */

typedef struct {
    size_t start;
    size_t stop;
} PrimeArgs;

/* Global Variables */

size_t Count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Functions */

size_t is_prime(size_t n) {
    for (size_t i = 2; i < n; i++) {
    	if (n % i == 0)
    	    return 0;
    }
    return 1;
}

/* Threads */

void* count_primes(void* arg) {
    PrimeArgs* pa = (PrimeArgs*)arg;

    printf("[%ld] start = %lu, stop = %lu\n", pthread_self(), pa->start, pa->stop);

    size_t local_count = 0;

    for (size_t n = pa->start; n < pa->stop; ++n) {
        if (is_prime(n)) {
            local_count++;
        }
    }


    return (void*)local_count;
}


int main(int argc, char *argv[]) {
    pthread_t thread[NUM_THREADS];
    PrimeArgs args[NUM_THREADS];

    // Spwaning NUM_THREADS threads to count subranges
    for (size_t t = 0; t < NUM_THREADS; ++t) {
        args[t].start = max(2, t*PRIME_MAX/NUM_THREADS);
        args[t].stop = (t+1)*PRIME_MAX/NUM_THREADS;

        pthread_create(&thread[t], NULL, count_primes, &args[t]);
    }

    size_t local_count = 0;
    for (size_t t = 0; t < NUM_THREADS; ++t) {
        pthread_join(thread[t], (void **)&local_count);
        Count += local_count;
    }

    printf("There are %lu primes less than %ld\n", Count, PRIME_MAX);
    return 0;
}
