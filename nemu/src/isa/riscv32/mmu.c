#include "nemu.h"

#define PTE_V 0x01
#define PTE_R 0x02
#define PTE_W 0x04
#define PTE_X 0x08
#define PTE_U 0x10

// Page directory and page table constants
#define NR_PDE    1024    // # directory entries per page directory
#define NR_PTE    1024    // # PTEs per page table
#define PGSHFT    12      // log2(PGSIZE)
#define PTXSHFT   12      // Offset of PTX in a linear address
#define PDXSHFT   22      // Offset of PDX in a linear address

// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/\------ OFF(va) ------/
typedef uint32_t PTE;
typedef uint32_t PDE;
#define PDX(va)     (((uint32_t)(va) >> PDXSHFT) & 0x3ff)
#define PTX(va)     (((uint32_t)(va) >> PTXSHFT) & 0x3ff)
#define OFF(va)     ((uint32_t)(va) & 0xfff)

// Address in page table or page directory entry
#define PTE_ADDR(pte)   (((uint32_t)(pte) & ~0x3ff) << 2)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((uint32_t)((d) << PDXSHFT | (t) << PTXSHFT | (o)))

static inline paddr_t page_translate(vaddr_t va) {
  paddr_t ptab = paddr_read(cpu.satp.ppn * 4096 + 4 * PDX(va), 4);
  if(!(ptab & PTE_V) | (!(ptab & PTE_R) && (ptab & PTE_W))){
  	printf("PTE is invalid!\n");
  	assert(0);
  }
  paddr_t page = paddr_read(PTE_ADDR(ptab) + 4 * PTX(va), 4);
  return PTE_ADDR(page) + OFF(va);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if(cpu.satp.mode){
  	if (PTE_ADDR(addr) != PTE_ADDR(addr + len - 1)) {
      uint8_t byte[4];
      for (int i = 0; i < len; i++)
        byte[i] = isa_vaddr_read(addr + i, 1);
      if (len == 2)
        return *(uint16_t *)byte;
      else
        return *(uint32_t *)byte;
  	}else{
  	  paddr_t paddr = page_translate(addr);
      return paddr_read(paddr, len);
  	}
  }else{
  	  return paddr_read(addr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(cpu.satp.mode){
  	if (PTE_ADDR(addr) != PTE_ADDR(addr + len - 1)) {
      uint8_t byte[4];
      if (len == 2)
        *(uint16_t *)byte = data;
      else
        *(uint32_t *)byte = data;
      for (int i = 0; i < len; i++)
        isa_vaddr_write(addr + i, byte[i], 1);
  	}else{
  	  paddr_t paddr = page_translate(addr);
      return paddr_write(paddr, data, len);
  	}
  }else{
  	 return paddr_write(addr, data, len);
  }
}
