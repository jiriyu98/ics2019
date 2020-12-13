#include "common.h"

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
  case -1:
    case _EVENT_YIELD:
        printf("Self trap!\n");
        break;
    case _EVENT_SYSCALL:
        //printf("Syscall!\n");
        do_syscall(c);
        break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
