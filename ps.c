#include "types.h"
#include "stat.h"
#include "user.h"
#include "uproc.h"
#include "param.h"

int
main(int argc, char *argv[])
{
  struct uproc *u=(struct uproc*)malloc(sizeof(struct uproc));
  int i;
  for(i = 1; i<NPROC; i++) 
  {
	if(getprocinfo(i, u)==0)
	{
		printf(1,"Info for process %d \n",i);
		printf(1, "Process name: %s \n", u->prname);
		printf(1, "Process ID:%d \n", u->prid);
		printf(1, "Process killed:%d \n",u->pk);
		if(i==1)
		printf(1, "NOTE : Parent id does not exist \n");
		printf(1, "Process parent ID:%d \n", u->prpid);
		printf(1, "Process Memory Size:%d \n", u->prsz);
		printf(1, "Process State:%s \n", u->prstate);
		printf(1, "-------------------------------\n");
	}
  }
  exit();
}
