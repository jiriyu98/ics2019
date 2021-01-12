#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if(current->max_brk<brk){
    void *va = NULL,*pa = NULL;
    void *begin =(void *) PGROUNDUP(current->max_brk);
    void *end = (void*)PGROUNDUP(brk);
    for(void*i = begin;i<=end;i+=PGSIZE){
      va=(void*)i;
      pa = new_page(1);
      _map(&current->as,va,pa,0);
    }
    current->max_brk = brk;
  }

  while(current->max_brk<=brk){
    void *pa = new_page(1);
    _map(&current->as,(void*)current->max_brk,pa,0);
    current->max_brk+=PGSIZE;
  }
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
