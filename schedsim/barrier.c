#include "barrier.h"
#include <errno.h>


#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads)
{
	return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	return pthread_barrier_wait(barrier);
}

#else


/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	/* Initialize fields in sys_barrier_t
	     ... To be completed ....
	*/
	//Creamos hilos para variables de barrier, NULL para coger atributos default
	pthread_mutex_init(&(barrier->mutex),NULL);//se inicializa y desbloquea
	pthread_cond_init(&(barrier->cond),NULL);

	
	barrier->nr_threads_arrived =0;
	barrier->max_threads =nr_threads;

	return 0;
}

/* Destroy barrier resources */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
	/* Destroy synchronization resources associated with the barrier
	      ... To be completed ....
	*/
	pthread_mutex_destroy(&(barrier->mutex));
	pthread_cond_destroy(&(barrier->cond));
	return 0;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{
	/* Implementation outline:
	   - a)Every thread arriving at the barrier acquires the lock and increments the nr_threads_arrived
	    counter atomically
	     * b.1)If the event this is not the last thread to arrive at the barrier, the thread
	       must block in the condition variable
	     * b.2)Otherwise...
	        1.Reset the barrier state in preparation for the next invocation of sys_barrier_wait() and
	        2. Wake up all threads blocked in the barrier
	   - Don't forget to release the lock before returning from the function

	    ... To be completed ....
	*/

	//a)
	pthread_mutex_lock(&(barrier->mutex));
	barrier-> nr_threads_arrived++;
	//b.1)
	if(barrier-> nr_threads_arrived < barrier->max_threads){
		while(barrier->nr_threads_arrived!=0){
			pthread_cond_wait(&(barrier->cond),&(barrier->mutex));
		}
	}
	//b.2)
	else{
		//1
		barrier->nr_threads_arrived =0;
		//2
		pthread_cond_broadcast(&(barrier->cond));		
	
	}
	//desbloqueamos mutex
	pthread_mutex_unlock(&(barrier->mutex));
	return 0;
}

#endif /* POSIX_BARRIER */
