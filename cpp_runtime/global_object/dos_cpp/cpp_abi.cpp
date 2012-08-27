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

    dobjs[obj_count].dtor_ = destructor;
    dobjs[obj_count].arg_ = arg;
    dobjs[obj_count].dso_handle_ = __dso_handle;
    ++obj_count;
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
      dobjs[i].dtor_(dobjs[i].arg_);
      //dobjs[i].dtor_((void*)1);
    }
    #endif
  }
}


