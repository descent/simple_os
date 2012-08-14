#include "cpp_abi.h"

void *__dso_handle;

static DObjs dobjs[DOBJS_SIZE];
static int obj_count=0;

void print_str(const char   *s);

extern "C"
{
  int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle)
  {

    dobjs[0].dtor_ = destructor;
    dobjs[0].arg_ = arg;
    dobjs[0].dso_handle_ = __dso_handle;
#if 0
    //if (obj_count >= DOBJS_SIZE)
      //return -1;
    dobjs[obj_count].dtor_ = destructor;
    dobjs[obj_count].arg_ = arg;
    dobjs[obj_count].dso_handle_ = __dso_handle;
    ++obj_count;
    //print_str("__cxa_atexit: register dtor here\r\n");
#endif
    return 0;
  }
  void g_dtors(void)
  {
    //print_str("g_dtors\r\n");
    for (int i=0 ; i < 1 ; ++i)
    {
      //print_str("xx g_dtors\r\n");
      //dobjs[i].dtor_(dobjs[i].arg_);
      dobjs[i].dtor_(0);
    }
  }
}


