/* Solve Starvation - We have defined that any philosopher can take both forks only if all other 
    philosophers have laid down their forks. The result is that all philosophers 
    eat one after the other. That is why there can never be a philosopher who will not eat. 
    While this is not an effective implementation of the problem, it is certainly 
    not possible to cause starvation here. 
*/

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#define N 5 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t forks[N]; 

void test(int phnum) 
{ 
		sleep(2); 
		printf("Philosopher %d is Eating\n", phnum + 1); 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 
    // take fork only if available
	sem_wait(&mutex); 
	sem_wait(&forks[phnum]); 
	sleep(1); 
	sem_wait(&forks[LEFT]); 
	printf("Philosopher %d takes fork %d and %d\n", 
		phnum + 1, LEFT + 1, phnum + 1); 

	printf("Philosopher %d is Hungry\n", phnum + 1); 

	// eat if neighbours are not eating 
	test(phnum); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 
	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

    // Release forks
	sem_post(&forks[phnum]); 
	sem_post(&forks[LEFT]); 
	sem_post(&mutex);
} 

void* philospher(void* num) 
{ 

	while (1) { 

		int* i = num; 

		sleep(1); 

		take_fork(*i); 

		sleep(0); 

		put_fork(*i); 
	} 
} 

int main() 
{ 

	int i; 
	pthread_t thread_id[N]; 

	// initialize the mutexes 
	sem_init(&mutex, 0, 1); 
	
	for (i = 0; i < N; i++) 

		sem_init(&forks[i], 0, 1); 

	for (i = 0; i < N; i++) { 

		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, 
					philospher, &phil[i]); 

		printf("Philosopher %d is thinking\n", i + 1); 
	} 

	for (i = 0; i < N; i++) 

		pthread_join(thread_id[i], NULL); 
} 

