#include <am.h>
#include <riscv32.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

_Context* __am_irq_handle(_Context *c) {
  extern void __am_get_cur_as(_Context *c);
  __am_get_cur_as(c);
  
  _Context *next = c;
  if (user_handler) {
    _Event ev = {0};
    switch (c->cause) {
      case -1:
          ev.event = _EVENT_YIELD;
          break;
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 7:
      case 8:
      case 9:
      case 13:
          ev.event = _EVENT_SYSCALL;
          break;
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  extern void __am_switch(_Context *c);
  __am_switch(next);

  return next;
}

extern void __am_asm_trap(void);

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  _Context *context = stack.end - sizeof(_Context);
  memset(context, 0x00, sizeof(_Context));
  context->epc = (uint32_t)entry;
  context->as = NULL;

  return context;
}

void _yield() {
  asm volatile("li a7, -1; ecall");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
