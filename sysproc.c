#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "uproc.h"

int getprocinfo(int, struct uproc *);
int thread_create(void(*)(void*),void *,void*);
int thread_join();
int thread_exit();

int
sys_thread_create(void)
{
	int fcn,arg;
	char *stack;

	if(argint(0,&fcn)<0 || fcn<0 || fcn>=myproc()->sz || argint(1,&arg) <0 || arg<0 || arg>=myproc()->sz ||
		argptr(2,&stack,4096)<0)
		return -1;

	//cprintf("mythread\n");      
	return thread_create((void(*)(void*))fcn, (void*)arg, stack);
}

int sys_thread_join(void)
{
	//cprintf("join\n");
	//return 0;
 	return thread_join();
}


int sys_thread_exit(void)
{
	thread_exit();
	return 0;
}

int 
sys_getprocinfo(void)
{ 
  int pid;
  struct uproc *proc;
  if( (argint(0, &pid) < 0) || argptr(1, (void*)&proc, sizeof(struct uproc)) <0) {
    return -1;
  }	
  return getprocinfo(pid, proc);
}

int
sys_backtrace(void)
{
  struct proc *currproc=myproc();
  cprintf("REGISTER INFO \n");
  cprintf("-----------------------\n");
  cprintf("value of eax :0x%x \n",currproc->tf->eax);
  cprintf("value of ebx :0x%x \n",currproc->tf->ebx);
  cprintf("value of ecx :0x%x \n",currproc->tf->ecx);
  cprintf("value of edx :0x%x \n",currproc->tf->edx);
  cprintf("value of esi :0x%x \n",currproc->tf->esi);
  cprintf("value of edi :0x%x \n",currproc->tf->edi);
  cprintf("value of ebp :0x%x \n",currproc->tf->ebp);
  cprintf("value of esp :0x%x \n",currproc->tf->esp);
  cprintf("value of eip :0x%x \n",currproc->tf->eip);
  
  cprintf("\n");
  cprintf("STACK INFO \n");
  cprintf("------------------------\n");
  uint bp=currproc->tf->ebp;
  while(bp <= currproc->sz)
  {
   cprintf("Base pointer : 0x%x \n", *(uint*)bp);
   cprintf("Return address : 0x%x \n", *(uint*)(bp+sizeof(uint)));
   bp=*(uint*)bp;
  }
  
 return 0;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
