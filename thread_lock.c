#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#include "thread_lock.h"

void thread_spin_init(struct thread_spinlock *lk)
{
   lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk)
{
  // The xchg is atomic.
  while(xchg(&lk->locked, 1) != 0)
    ;
  __sync_synchronize();
}

void thread_spin_unlock(struct thread_spinlock *lk)
{
   __sync_synchronize();
   asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

void thread_mutex_init(struct thread_mutexlock *m)
{
   m->locked = 0;
}

void thread_mutex_lock(struct thread_mutexlock *m)
{
  while(xchg(&m->locked,1)!=0)
     sleep(1);
  __sync_synchronize();
}

void thread_mutex_unlock(struct thread_mutexlock *m)
{
  __sync_synchronize();
  asm volatile("movl $0, %0" : "+m" (m->locked) : );
}

void thread_cond_init(struct thread_cond *cv)
{
  cv->signal=0;
}

void thread_cond_wait(struct thread_cond *cv,struct thread_mutexlock *m)
{
   if(m->locked==1)
 	thread_mutex_unlock(m);
   while(!cv->signal)
	{
	  sleep(1);	
	}	
	thread_mutex_lock(m);
}

void thread_cond_signal(struct thread_cond *cv)
{
    cv->signal=1;
}

int thread_sem_init(struct thread_sem *s,int val)
{ 
  s->count=val;
  thread_mutex_init(&s->m);
  thread_cond_init(&s->cv);
  return 0;
}

void thread_sem_wait(struct thread_sem *s)
{
  thread_mutex_lock(&s->m);
  while(s->count==0)
  {
	thread_cond_wait(&s->cv,&s->m);
  }
  s->count--;
  thread_mutex_unlock(&s->m);
}

void thread_sem_post(struct thread_sem *s)
{
  thread_mutex_lock(&s->m);
  s->count++;
  if(s->count==1)
    thread_cond_signal(&s->cv);
  thread_mutex_unlock(&s->m);
}

int gettid(void)
{
  uint a;
  a=((uint)&a)/4096;
  a=(a+1)*4096;
  a-=sizeof(struct tls);

  struct tls *t=(struct tls*)a;

  if(t->id<0)
     return -1;

  return t->id;
}
