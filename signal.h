#ifndef XV6_SIGNAL
#define XV6_SIGNAL

#define SIGFPE 50
#define SIGALRM 100

typedef void (signal_handler_t)(siginfo_t);
int signal(int signum, signal_handler_t func);

typedef struct signal_info_t {
	int signal_number;
}signal_info_t;


#endif
