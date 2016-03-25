#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

static int trapCount = 0;

void handle_signal(siginfo_t info)
{
	trapCount++;
	
	if(trapCount >= 1000000)
	{
		__asm__ ("movl $0x7c,4(%ebp)\n\t"); //put address of next instruction in return address
	}
}

int main(int argc, char *argv[])
{
	int x = 5;
	int y = 0;
	int start;
	int time;
	
	signal(SIGFPE, &handle_signal);
	start = uptime();
	printf(1,"waiting for 1000000 traps...\n");
	x = x / y;
	time = uptime()-start;
	printf(1, "Traps Performed: %d\n", trapCount);
	printf(1, "Total Elapsed Time: %d ticks\n",time);
	printf(1, "Average Time Per Trap: %d * 10^-6 ticks\n", (int)(time*1000000/trapCount));
	exit();
}
