#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include "userprog/pagedir.c"

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
        sys_wait(args[1]);
        break;
      case SYS_EXEC:
        ptr_is_valid((const void *) args[1]);
        f->eax = sys_exec((char*) args[1]);
        break;
      case SYS_WAIT:
        f->eax = sys_wait(args[1]);
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
 * thread has two new features : proc(process_info struct), list of child_process
 *
 *                          +<---------parent<------------+
 *   (procA) <--proc--- (threadA) ---listof_child--> [ (procB)->(procC)->(procD)... ]
 *                                                  upward|  upward|  upward|
 *                                                    (threadB)(threadC)(threadD)
 *
 * accessing to the child process : use list of child_process
 * accessing to parent process : use parent in proc struct element
 */


// valid address checker for user program pointer
// (1) null? (2) above PHYS_BASE? (3) unmapped memory?
static void ptr_is_valid(const void *vaddr)
{
  static struct thread *curr = thread_current();

  // If given addr is invalid, exit(-1)
  if (!is_user_vaddr ((void *) vaddr) || !vaddr || !pagedir_get_page (curr->pagedir, vaddr)
  )
  {
    sys_exit(-1);
  }
}

/* Traverse the user’s page tables
 * The valid address should have the corresponding PTE (page table entry)
 * “Present” flag in the PTE should be set
 * Refer to userprog/pagedir.c, threads/vaddr.h, and threads/pte.h
 */


static void sys_halt(void)
  {
    power_off();
  }

static void sys_exit(int status)
  {
    struct thread *t = current_thread();
    struct thread *parent = t->proc->parent;

    struct list_elem *e; // for list traversing
    struct process_info *p;

    // process information modification
    t->proc->exit = true;

    // modify its children process information
    // and remove child_list (they become orphaned threads)
    e = list_begin(&t->listof_child);
    for (e = list_begin (&t->listof_child); e != list_end (&t->listof_child); e = list_remove (e))
    {
      p = list_entry(e, struct process_info, elem);
      p->parent = NULL;
    }

    if (t->parent != NULL)
    {
      t->proc->status = status;
      printf ("return to the parent\n");
    }

    // process modify ends, terminating thread
    thread_exit();
    break;
  }

static pid_t sys_exec(const char *cmd_line)
  {
    // use process_execute for creating process & corresponding thread
    // Part for handling & creating process_info struct is in thread_create
    struct thread * curr = thread_current();
    pid_t pid = process_execute(cmd_line);

    // if child's exec is not yet loaded ; have to wait(cannot return)
    // use semaphore for synchronization ;
    // sema down when loading starts, sema up when loading finishes
    sema_down(&curr->exec_sema);

    if (pid == TID_ERROR) {
      return -1;
    }
    return pid;
  }

static int sys_wait(pid_t pid)
  {
    // wait for a child process with pid
    // Returns the status that pid passed to exit. -1 otherwise.
    return process_wait();
  }
