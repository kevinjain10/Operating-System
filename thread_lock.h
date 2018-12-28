
struct thread_spinlock
{
  uint locked;
};

struct thread_mutexlock
{
  uint locked;
};

struct thread_cond
{
  int signal;
};

struct q
{
  struct thread_cond cv;
  struct thread_mutexlock m;
};

struct thread_sem
{
  int count;
  struct thread_cond cv;
  struct thread_mutexlock m;
};

struct tls
{
  uint id; 
};

void thread_spin_init(struct thread_spinlock *);
void thread_spin_lock(struct thread_spinlock *);
void thread_spin_unlock(struct thread_spinlock *);
void thread_mutex_init(struct thread_mutexlock *);
void thread_mutex_lock(struct thread_mutexlock *);
void thread_mutex_unlock(struct thread_mutexlock *);
void thread_cond_init(struct thread_cond *);
void thread_cond_wait(struct thread_cond *,struct thread_mutexlock *);
void thread_cond_signal(struct thread_cond *);
int thread_sem_init(struct thread_sem *,int);
void thread_sem_wait(struct thread_sem *);
void thread_sem_post(struct thread_sem *);
int gettid(void);
