#include "rtl/rtl.h"
#include "cpu/exec.h"

#define IRQ_TIMER 0x80000005  // for riscv32

void raise_intr(uint32_t NO, vaddr_t epc) {
	// Bad implemention. It should be in cpu! My fault!

	cpu.sstatus.SPIE = cpu.sstatus.SIE;
	cpu.sstatus.SIE = 0;
	cpu.sepc.val = epc;
	cpu.scause.val = NO;
	decinfo.jmp_pc = cpu.stvec.val;
	decinfo_set_jmp(true);
}

bool isa_query_intr(void) {
  if (cpu.INTR & cpu.sstatus.SIE) {
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }
  return false;
}
