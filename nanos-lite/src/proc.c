#include "proc.h"

#define MAX_NR_PROC 4

extern void naive_uload(PCB*, const char*);
extern void context_kload(PCB *pcb, void *entry);
extern void context_uload(PCB *pcb, const char *filename);

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

// static int fg_pcb = 1;
// void set_pcb_id(int id){
//     fg_pcb = id;
//     Log("chosen=%d", id);
// }

void init_proc() {
  // context_kload(&pcb[0], (void *)hello_fun);
  // context_kload(&pcb[0], (void *)hello_fun);
  // context_uload(&pcb[0], "/bin/dummy");
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/pal");
  // context_uload(&pcb[2], "/bin/pal");
  // context_uload(&pcb[3], "/bin/pal");
  switch_boot_pcb();
}

_Context* schedule(_Context *prev) {
  // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  // current = &pcb[0];
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);

  // then return the new context
  return current->cp;
}
