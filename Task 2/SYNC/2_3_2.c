/* Solve Livelock - same implementation as 2_1_2. forks will be numbered 1 to 5 and each philosopher will 
   always pick up the lower-numbered fork first, and then the higher-numbered fork. 
   In this case philosophers can never get stuck. if four of the five philosophers simultaneously 
   pick up their lower-numbered fork, only the highest-numbered fork will remain on the table, 
   so the fifth philosopher will not be able to pick up any fork. Moreover, only one philosopher 
   will have access to that highest-numbered fork, so he will be able to eat using two forks. 
   Therefore, there will always be one philosopher who can progress and prevent a situation that the 
   system is not progressing. This method can cause starvation but it will never create Livelock.
 */

#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#define N 5 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[N]; 
sem_t forks[N]; 

void test(int phnum) 
{ 
		printf("Philosopher %d takes fork %d and %d\n", 
					phnum + 1, LEFT + 1, phnum + 1); 

		printf("Philosopher %d is Eating\n", phnum + 1); 
		sleep(2); 
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 
    // find lower and higher numbered forks
    int lowerFork, higherFork;
    if(phnum != 0) {
    	lowerFork = LEFT;
    	higherFork = phnum;
    }
    else {
    	lowerFork = phnum;
    	higherFork = LEFT;
    }
    // philosopher will always pick up the lower-numbered fork first, and then the higher-numbered fork.
	sem_wait(&forks[lowerFork]); 
	sleep(1); 
	sem_wait(&forks[higherFork]); 

	// eat if neighbours are not eating 
	test(phnum); 

	//sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 
     // find lower and higher numbered forks
	int lowerFork, higherFork;
    if(phnum != 0) {
    	lowerFork = LEFT;
    	higherFork = phnum;
    }
    else {
    	lowerFork = phnum;
    	higherFork = LEFT;
    }

	printf("Philosopher %d putting fork %d and %d down\n", 
		phnum + 1, LEFT + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1); 

    // release the higher-numbered fork first, and then the lower-numbered fork.
	sem_post(&forks[higherFork]); 
	sem_post(&forks[lowerFork]); 
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

