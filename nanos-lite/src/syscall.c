#include "common.h"
#include "fs.h"
#include "proc.h"
#include "syscall.h"

extern void naive_uload(PCB *pcb, const char *filename);
static int programBrk;

int do_open(const char*path, int flags, int mode){
    int res = fs_open(path, flags, mode);
    return res;
}

int do_close(int fd){
    int res = fs_close(fd);
    return res;
}

int do_read(int fd, void*buf, size_t count){
    if(fd>=0 && fd<=2){
        return 0;
    }
    int res = fs_read(fd, buf, count);
    return res;
}

int do_write(int fd, const void*buf, size_t count){
    /*
    if(fd==1 || fd==2){
        for(int i = 0;i < count;i++){
            _putc(((char*)buf)[i]);
        }
        return count;
    }
    if(fd==0)
        return 0;
    */
    int res = fs_write(fd, buf, count);
    return res;
}

size_t do_lseek(int fd, size_t offset, int whence){
    size_t res = fs_lseek(fd, offset, whence);
    return res;
}

int do_brk(int addr){
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
          _halt(a[1]);
          break;
      case SYS_yield:
          _yield();
          c->GPRx = 0;
          break;
      case SYS_write:
          c->GPRx = do_write(a[1], (void*)(a[2]), a[3]);
          break;
      case SYS_read:
          c->GPRx = do_read(a[1], (void*)(a[2]), a[3]);
          break;
      case SYS_lseek:
          c->GPRx = do_lseek(a[1], a[2], a[3]);
          break;
      case SYS_open:
          c->GPRx = do_open((const char *)a[1], a[2], a[3]);
          break;
      case SYS_close:
          c->GPRx = do_close(a[1]);
          break;
      case SYS_brk:
          c->GPRx = do_brk(a[1]);
          break;
      case SYS_execve:
          printf("%s\n", a[1]);
          naive_uload(NULL, (const char*)a[1]);
          c->GPR2 = SYS_exit;
          do_syscall(c);
          break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
