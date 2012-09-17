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
    dobjs[obj_count].dtor_ = destructor;
    dobjs[obj_count].arg_ = arg;
    dobjs[obj_count].dso_handle_ = __dso_handle;
    ++obj_count;
    return 0;
  }
  void g_dtors(void)
  {
    for (int i=obj_count-1 ; i >= 0 ; --i)
    {
      dobjs[i].dtor_(dobjs[i].arg_);
    }
  }
}


int __cxa_guard_acquire(u32 *myself)
{
  //print_str("__cxa_guard_acquire\r\n");
  return !(*((char*)myself));
}

void __cxa_guard_release(u32 *myself)
{
  //print_str("__cxa_guard_release\r\n");
  *((char*)myself) = 1;
}
