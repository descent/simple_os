#ifndef IPC_H
#define IPC_H

#include "process.h"

typedef struct Message_
{
  int source;
  int type;
}Message;

static inline proc2pid(Process *p)
{
}

static inline void* va2la(int pid, void* va)
{
}

int msg_send(Process* current, int dest, Message* m);
int msg_receive(Process* current, int src, Message* m);

#define SEND            1
#define RECEIVE         2
#define BOTH            3       /* BOTH = (SEND | RECEIVE) */

#define ANY             (NR_TASKS + NR_PROCS + 10)
#define INTERRUPT       -10



#endif
