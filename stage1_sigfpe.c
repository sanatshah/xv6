#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

void other_function() { 
}

void handle_signal(siginfo_t info)
{
	printf(1, "Caught signal %d...\n", info.signum);
	printf(1, "info.signum is %d and SIGFPE is %d.", info.signum, SIGFPE);
	if (info.signum == SIGFPE)
		printf(1, "TEST PASSED\n");
	else
		printf(1, "TEST FAILED: wrong signal sent.\n");
	exit();
}

int main(int argc, char *argv[])
{
	int x = 5;
	int y = 0;

	signal(SIGFPE, handle_signal);

	x = x / y;

	printf(1, "TEST FAILED: no signal sent.\n");
	
	exit();
}
