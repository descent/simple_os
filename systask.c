/*
 * system task
 */

#include "process.h"
#include "k_assert.h"
#include "mm.h"

void task_sys(void)
{
  extern u32 ticks;

  Message msg;
  while(1)
  {
#ifdef IPC
    send_recv(RECEIVE, ANY, &msg);
    int src = msg.source;
    switch (msg.type)
    {
      case GET_TICKS:
      {
        msg.RETVAL = ticks;
        send_recv(SEND, src, &msg);
        break;
      }
      default :
      {
        panic("unknown message type");
        break;
      }
    }
#endif
  }
 
}

Message mm_msg;

void task_mm(void)
{
  while(1)
  {
    send_recv(RECEIVE, ANY, &mm_msg);
    int src = mm_msg.source;
    int reply = 1;

    switch (mm_msg.type)
    {
      case FORK:
      {
        mm_msg.RETVAL = do_fork();
        break;
      }
      default :
      {
        panic("unknown message type");
        break;
      }
    }
    if (reply)
    {
      mm_msg.type = SYSCALL_RET;
      send_recv(SEND, src, &mm_msg);
      
    }
  }

}

