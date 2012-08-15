#include "cpp_abi.h"

void *__dso_handle;

static DObjs dobjs[DOBJS_SIZE];
int obj_count=0;

void print_str(const char   *s);
void s16_print_int(int i, int radix);

extern "C"
{

  int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle)
  {
    __asm__ __volatile__("xchg %bx, %bx");
    print_str("atexit\r\n");
    s16_print_int(obj_count, 10);
    print_str("\r\n");

#if 0
    dobjs[0].dtor_ = destructor;
    dobjs[0].arg_ = arg;
    dobjs[0].dso_handle_ = __dso_handle;
#else
    //if (obj_count >= DOBJS_SIZE)
      //return -1;
     #if 1
    dobjs[obj_count].dtor_ = destructor;
    dobjs[obj_count].arg_ = arg;
    dobjs[obj_count].dso_handle_ = __dso_handle;
    #endif
    ++obj_count;
    //print_str("__cxa_atexit: register dtor here\r\n");
#endif
    return 0;
  }
  void g_dtors(void)
  {
    print_str("g_dtors\r\n");
    s16_print_int(obj_count, 10);
    print_str("\r\n");
    //print_str("g_dtors\r\n");
    #if 1
    for (int i=obj_count-1 ; i >= 0 ; --i)
    {
      //print_str("xx g_dtors\r\n");
      //dobjs[i].dtor_(dobjs[i].arg_);
      dobjs[i].dtor_(0);
    }
    #endif
  }
}


