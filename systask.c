/*
 * system task
 */

#include "process.h"
#include "k_assert.h"

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
