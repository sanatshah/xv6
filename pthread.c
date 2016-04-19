#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "pthread.h"

// Implement your pthreads library here.

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
  void *stack = (void*) malloc(4096);
  thread->pid=clone(*start_routine, arg, stack);
  thread->stack = stack;

  if (thread->pid>=0)
    return 0;

  //0 on success, on error returns negative number
	return -1;
}

int pthread_join(pthread_t thread, void **retval)
{

  int r;

  pthread_t* t=&thread;

  r=join(t->pid, t->stack, retval);

  //has to return 0 on sucess, negative number on fail
	return r;
}
int pthread_exit(void *retval)
{
  texit(retval);
	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex){




}


int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr){



}

int pthread_mutex_lock(pthread_mutex_t *mutex){




}


int pthread_mutex_unlock(pthread_mutex_t *mutex){




  
}
