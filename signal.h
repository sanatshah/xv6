#ifndef XV6_SIGNAL
#define XV6_SIGNAL 

// You should define anything signal related that needs to be shared between
// kernel and userspace here

#define SIGFPE 50
#define SIGALRM 100
typedef struct siginfo_t {
	int signum;
}siginfo_t;

typedef void (signal_handler_t)(siginfo_t);
int signal(int signum, signal_handler_t func);	


// At a minimum you must define the signal constants themselves
// as well as a sighandler_t type.

#endif
