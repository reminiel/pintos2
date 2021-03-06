#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

// process_info struct
// stores information about wait, exit, status..
struct process_info
  {
    int pid;                    /* process id */
    int status;                 /* process status */
    bool wait;
    bool exit;
    struct thread* parent;      /* parent thread */
    struct semaphore sema;      /* semaphore for sync. */
    struct list_elem elem;      /* for list element */
};

#endif /* userprog/process.h */
