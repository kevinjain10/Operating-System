//Extra credit (15%): Conditional variables
#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lock.h"

#define THREAD_NUM 10

struct balance {
    char name[32];
    int amount;
};

volatile int total_balance = 0;
volatile int count; //indicate which thread should start to work

struct q q; //includes condition variable and the lock for protecting count

volatile unsigned int delay (unsigned int d) {
   unsigned int i; 
   for (i = 0; i < d; i++) {
       __asm volatile( "nop" ::: );
   }

   return i;   
}

void do_work(void *arg){

    int i;
    int old;

    struct balance *b = (struct balance*) arg;

    char *name = b->name;
    int amt = b->amount;

    thread_mutex_lock(&q.m);

    //while(count < 0)
      //  thread_cond_wait(&q.cv, &q.m);
 
    printf(1, "[thread: %s] Starting do_work. Amount: %d\n", name, amt);

    thread_mutex_unlock(&q.m);
 
    for (i = 0; i < amt; i++) {
         old = total_balance;
         delay(100000);
         total_balance = old + 1;
    }

    //After finishing work, the thread becomes a sender
    thread_mutex_lock(&q.m);

    count++; //increment count to ask the next thread to work
    printf(1, "[thread: %s] Increment count to %d\n", name, count);

    //wake up all threads sleeping on condition q.cv
    printf(1, "[thread: %s] Work done: %s\n", name, name);
    thread_cond_signal(&q.cv);

    thread_mutex_unlock(&q.m);

    thread_exit();

    return;
}

int main(int argc, char *argv[]) {

  struct balance b[THREAD_NUM];
  void *s[THREAD_NUM];
  int t[THREAD_NUM], r[THREAD_NUM];
  int correct_balance = 0;

  thread_cond_init(&q.cv);
  thread_mutex_init(&q.m);
  count = -1;

  int i;

  for(i = 0; i < THREAD_NUM; i++){

	  b[i].name[0] = 65+i;
	  b[i].name[1] = '\0';
	  b[i].amount = (i+1)*1000;
	  correct_balance += b[i].amount;
	  s[i] = malloc(4096);
	  t[i] = thread_create(do_work, (void*)&b[i], s[i]);
  }

  //Before child threads start to work, the main thread is a sender
  thread_mutex_lock(&q.m);

  count++; //ask the first thread to work
  printf(1, "[main thread] Increment count to %d\n", count);

  //wake up all threads sleeping on condition q.cv
  printf(1, "[main thread] Initialization done. Signaling condition.\n");
  thread_cond_signal(&q.cv);

  thread_mutex_unlock(&q.m);

  //After finishing initialization work, the main thread becomes a receiver
  thread_mutex_lock(&q.m);

  while(count < THREAD_NUM)
  	thread_cond_wait(&q.cv, &q.m);

  thread_mutex_unlock(&q.m);

  //main thread collects exited child threads' pids
  for(i = 0; i < THREAD_NUM; i++){

	  r[i] = thread_join();
  }

  printf(1, "[main thread] Threads finished: ");

  for(i = 0; i < THREAD_NUM; i++){

	  printf(1, "(%d):%d, ", t[i], r[i]);
  }
  printf(1,"\n");

  printf(1, "[main thread] Shared balance: %d, correct balance: %d, diffs: %d\n", total_balance, correct_balance, correct_balance-total_balance);

  exit();
}

