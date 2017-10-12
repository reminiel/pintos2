#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "devices/shutdown.h"
#include "userprog/process.h"

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
      // file system
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

/*
 * data structure for thread & process
 *
 * new struct : process_info
 * it contains pid, wait information, status(?)
 *
 * thread has two new features : parent thread*, list of child_process
 *
 * accessing to the child process : use list of child_process
 * accessing to parent process : use parent
 */

/* TODO : valid addr checker */


static void sys_halt(void)
  {
    power_off();
  }

static void sys_exit(int status)
  {
    struct thread *t = current_thread();
    struct thread *parent = t->parent;
    struct list child_list = t->listof_child;

    struct list_elem *e; // for list traversing
    struct process_info *p;

    // process information modification
    t->process_info->exit = true;

    // modify its children process information
    // and remove child_list (they become orphaned threads)
    e = list_begin(&child_list);
    for (e = list_begin (&child_list); e != list_end (&child_list); e = list_remove (e))
    {
      p = list_entry(e, struct process_info, elem);
      p->parent = NULL;
    }

    // status passing ; passing to eax???
    if (t->parent == NULL)
    {
      print("return to the kernel(how?)\n");
    }
    else
    {
      t->process_info->status = status;
      print("return to the parent(how?)\n");
    }

    // process modify ends, terminating thread
    thread_exit();
    break;
  }


static void sys_exec(const char *cmd_line)
  {

    // generate processinfo
    return process_execute(cmd_line);
  }