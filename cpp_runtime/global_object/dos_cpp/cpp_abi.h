__asm__(".code16gcc\n");

#include "dos_io.h"
#include "type.h"

#ifndef CPP_ABI_H
#define CPP_ABI_H

#define DOBJS_SIZE 3

struct DObjs
{
  void (*dtor_) (void *);
  void *arg_;
  void *dso_handle_;
};

extern "C"
{
  int __cxa_atexit(void (*destructor) (void *), void *arg, void *__dso_handle);
  void g_dtors(void);
  int __cxa_guard_acquire(u32 *myself);
  void __cxa_guard_release(u32 *myself);
}

#endif
