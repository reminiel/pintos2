#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

//System call functions
static void sys_halt (void);
static void sys_exec(const char *cmd_line);

//System call handler
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f)
{
  printf ("system call!\n");
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
      case SYS_EXIT:
        break;
      case SYS_EXEC:
        sys_exec((char*) args[1]);
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
static void sys_halt(void)
  {
    power_off();
  }

static void sys_exec(const char *cmd_line)
  {
    return process_execute(cmd_line);
  }