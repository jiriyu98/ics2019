#include "proc.h"
#include "fs.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#define min(x, y)         ((x) < (y) ? (x) : (y))
#define PDX(va)     (((uint32_t)(va) >> 22) & 0x3ff)
#define PTX(va)     (((uint32_t)(va) >> 12) & 0x3ff)
#define OFF(va)     ((uint32_t)(va) & 0xfff)
#define PTE_ADDR(pte)   (((uint32_t)(pte) & ~0x3ff) << 2)

extern size_t fs_openoffset(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);

  Elf_Ehdr Ehdr;
  fs_read(fd, (void *)&Ehdr, sizeof(Elf_Ehdr));
  if (memcmp(Ehdr.e_ident, ELFMAG, SELFMAG))
    panic("file %s ELF format error!", filename);

  for (size_t i = 0; i < Ehdr.e_phnum; ++i) {
    Elf_Phdr Phdr;
    fs_lseek(fd, Ehdr.e_phoff + Ehdr.e_phentsize * i, SEEK_SET);
    fs_read(fd, (void *)&Phdr, sizeof(Elf_Phdr));
    size_t opset = fs_openoffset(fd);
    if (Phdr.p_type == PT_LOAD) {
      fs_lseek(fd, Phdr.p_offset, SEEK_SET);

#ifdef HAS_VME
      void *vaddr = (void *)Phdr.p_vaddr;
      void *paddr;
      int count=0;
      for(size_t i=0,sz = Phdr.p_memsz; i<sz; i+=PGSIZE){
          size_t read_bytes = ((sz-i)>=PGSIZE) ? PGSIZE : (sz-i);
          paddr = new_page(1);
          count++;
          _map(&pcb->as, vaddr, paddr,0);
          fs_read(fd, paddr, read_bytes);
          pcb->max_brk = (uintptr_t)vaddr + PGSIZE;
          vaddr += PGSIZE;
        }
        memset((void*)(paddr-(count-1)*PGSIZE+Phdr.p_filesz), 0, Phdr.p_memsz-Phdr.p_filesz);
#else
      fs_read(fd, (void *)Phdr.p_vaddr, Phdr.p_filesz);
      memset((void *)(Phdr.p_vaddr + Phdr.p_filesz), 0, Phdr.p_memsz - Phdr.p_filesz);
#endif
    }
    fs_lseek(fd,opset,SEEK_SET);
  }

  fs_close(fd);

  return Ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  memset(pcb, 0, sizeof(PCB)); 
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
