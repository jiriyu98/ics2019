#include "common.h"
#include "fs.h"
#include "proc.h"
#include "syscall.h"

int sys_exit(){

}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
      // case SYS_exit:
      //     _halt(0);
      //     break;
      case SYS_yield:
          _yield();
          c->GPRx = 0;
          break;
      // case SYS_write:
      //     c->GPRx = do_write(a[1], (void*)(a[2]), a[3]);
      //     break;
      // case SYS_read:
      //     c->GPRx = do_read(a[1], (void*)(a[2]), a[3]);
      //     break;
      // case SYS_lseek:
      //     c->GPRx = do_lseek(a[1], a[2], a[3]);
      //     break;
      // case SYS_open:
      //     c->GPRx = do_open((const char *)a[1], a[2], a[3]);
      //     break;
      // case SYS_close:
      //     c->GPRx = do_close(a[1]);
      //     break;
      // case SYS_brk:
      //     c->GPRx = do_brk(a[1]);
      //     break;
      // case SYS_execve:
      //     c->GPRx = sys_execve((void *)a[1], (char *const *)(void *)a[2], (char *const *)(void *)a[3]);
      //     break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
