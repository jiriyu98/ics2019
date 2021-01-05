#include "common.h"
#include "fs.h"
#include "proc.h"
#include "syscall.h"

extern void naive_uload(PCB *pcb, const char *filename);
static int programBrk;

static inline int sys_open(const char*path, int flags, int mode){
    return fs_open(path, flags, mode);
}

static inline int sys_close(int fd){
    return fs_close(fd);
}

static inline int sys_read(int fd, void*buf, size_t len){
    return fs_read(fd, buf, len);
}

static inline int sys_write(int fd, const void*buf, size_t len){
  Log("%s", buf);
    return fs_write(fd, buf, len);
}

static inline int sys_lseek(int fd, size_t offset, int whence){
    return fs_lseek(fd, offset, whence);
}

static inline int sys_brk(int addr){
    programBrk = addr;
    return 0;
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
      case SYS_exit:
          naive_uload(NULL,"/bin/init");
          // _halt(a[1]);
          break;
      case SYS_yield:
          _yield();
          c->GPRx = 0;
          break;
      case SYS_write:
          c->GPRx = sys_write(a[1], (void*)(a[2]), a[3]);
          break;
      case SYS_read:
          c->GPRx = sys_read(a[1], (void*)(a[2]), a[3]);
          break;
      case SYS_lseek:
          c->GPRx = sys_lseek(a[1], a[2], a[3]);
          break;
      case SYS_open:
          c->GPRx = sys_open((const char *)a[1], a[2], a[3]);
          break;
      case SYS_close:
          c->GPRx = sys_close(a[1]);
          break;
      case SYS_brk:
          c->GPRx = sys_brk(a[1]);
          break;
      case SYS_execve:
          naive_uload(NULL, (const char*)a[1]);
          c->GPRx = 0;
          break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
