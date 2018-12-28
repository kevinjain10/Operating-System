#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock.h"

#define MAX_THREADS 10

int i;

typedef struct balance {
	char name[32];
	char amount;
}balance_t;

balance_t per_thread_balance[MAX_THREADS];

struct thread_mutexlock lock;

volatile int total_balance = 0;

volatile unsigned int delay(unsigned int d) {
	unsigned int i;
	for (i = 0; i < d; i++) {
		__asm volatile( "nop" ::: );
	}
	return i;
}

int foo() {

	int id = gettid();
	thread_mutex_lock(&lock);
	per_thread_balance[id].amount += id*10;
	thread_mutex_unlock(&lock);
	thread_exit();

	return 0;
}

int main(int argc, char *argv[]) {

	//initialize the local variables
	for (i = 0; i < MAX_THREADS; i++) {
		per_thread_balance[i].amount = i*10;
		per_thread_balance[i].name[0] = (char) i + 'a';
	}

	thread_mutex_init(&lock);

	int x = 0;
	for (i = 0; i < MAX_THREADS; i++) {
		void* sb = malloc(4096);
		thread_create((void *) foo, (void *)x, sb);
	}

	for (i = 0; i < MAX_THREADS; i++) {
		thread_join();
	}

	for (i = 0; i < MAX_THREADS; i++) {
		printf(1, "Thread Name : %s Thread Number : %d Thread Balance : %d \n",per_thread_balance[i].name,i,
				per_thread_balance[i].amount);
	}
	printf(1,"Completed using per thread\n");
	exit();
}
