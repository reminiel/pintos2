#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

//System call functions
static void sys_halt (void);

//System call handler
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


syscall_handler (struct intr_frame *f)
{
  printf ("system call!\n");
  //Implement here
  int i = 0, args[4];
  while(i < 4)
    {
      args[i] = ((int *)f->esp + i);
      i++;
    }
  switch(args[0])
    {
      case SYS_HALT:
        sys_halt();
        break;
      case SYS_EXEC:
        break;
      case SYS_WAIT:
        break;
      case SYS_CREATE:
        break;
      case SYS_REMOVE:
        break;
      case SYS_OPEN:
        break;
      case SYS_FILESIZE:
        break;
      case SYS_READ:
        break;
      case SYS_WRITE:
        break;
      case SYS_SEEK:
        break;
      case SYS_TELL:
        break;
      case SYS_CLOSE:
        break;
    }
}
void sys_halt(void)
  {
    shutdown_power_off();
  }