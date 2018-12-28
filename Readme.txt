README

thread - 	   runs the normal thread.c code without implementation of any locks.

thread_spin -      runs the thread.c file code with implementation of spinlocks.
		   Correct balance is printed here as both work in synchronisation.

thread_mutex -     runs the thread.c file code with implementation of mutex locks because spinlocks are not so efficient.

thread_grow  -	   fixes sbrk and malloc to grow the process size

thread_cv - 	   Using condition variables we implement the thread_cv.c file where 10 threads are run together 
            	   and each of them is executed one after the other. One thread runs at a time and after it finishes it
	    	   signals the other thread to wakeup and start executing.

thread_perthread - implement variables that are private to each thread by keeping track of the thread ID for each thread.

thread_cutemacro - provide support for cleaner per thread variables that declare  per thread variables array and provide
		   access to per-thread variables as macros.	
