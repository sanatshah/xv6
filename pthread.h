#ifndef XV6_PTHREAD
#define XV6_PTHREAD

// Define all functions and types for pthreads here.
// This can be included in both kernel and user code.

//types
struct pthread_t {
  //define pthread_t struct here
  int pid;
  void* stack;


} typedef pthread_t;

struct pthread_mutex_t {


} typedef pthread_mutex_t;


typedef int pthread_mutexattr_t;
typedef int pthread_attr_t;
//function declarations
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int pthread_join(pthread_t thread, void **retval);
int pthread_exit(void *retval);


int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

#endif
