#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);
  
  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}

//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(proc->killed)
      exit();
    proc->tf = tf;
    syscall();
    if(proc->killed)
      exit();
    return;
  }

  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:

	/*timer signal code*/

	for (i = 0; i < 64; i++){
	p = (struct proc*) getproc(i);	
	if (p && p->alarm_time > 0) {
		p->alarm_counter++; 
		//cprintf("alarmtime is %d, counter is %d", proc->alarmtime, proc->alarmcounter);
		if (p->alarm_counter >= p->alarm_time){
			p->alarm_time = 0;
			p->alarm_counter = 0;
			//callUserHandler(proc->sighandlers[1]);
			struct signal_info_t info;			
			info.signal_number = SIGALRM;
			*((signal_info_t*)(proc->tf->esp - 4)) = info;
			cprintf("&info is %d, info is %d, info.signum is %d", &info, info, info.signal_number);
	 		p->tf->esp -= 8;  
	 		p->tf->eip = (uint) p->signal_handlers[1];
		}		
	}
	}	

    if(cpu->id == 0){
      acquire(&tickslock);
      ticks++;
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpu->id, tf->cs, tf->eip);
    lapiceoi();
    break;
   
  case T_DIVIDE:  /*Divide by zero trap*/
	cprintf("Got to the divide trap - value of sigfpe handler is %d\n", proc->signal_handlers[0]);
    if (proc->signal_handlers[0] >= 0) {
	//uint m = proc->sighandlers[0];
	//callUserHandler(m);
	 struct signal_info_t info;
			info.signal_num = SIGFPE;
			*((signal_info_t*)(proc->tf->esp - 4)) = info;
			cprintf("&info is %d, info is %d, info.signum is %d", &info, info, info.signal_number);
	 		proc->tf->esp -= 8;  
	 		proc->tf->eip = (uint) proc->signal_handlers[0]; 
        return;
    }

    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip, 
            rcr2());
    proc->killed = 1;

    if (proc->killed)
	exit();
    return;
  //PAGEBREAK: 13
  default:
    if(proc == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpu->id, tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            proc->pid, proc->name, tf->trapno, tf->err, cpu->id, tf->eip, 
            rcr2());
    proc->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running 
  // until it gets to the regular system call return.)
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
  if(proc && proc->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Check if the process has been killed since we yielded
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();
}
