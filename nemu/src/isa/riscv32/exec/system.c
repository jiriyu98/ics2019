#include "cpu/exec.h"

extern void raise_intr(uint32_t, vaddr_t);
int32_t readcsr(int i){
	switch(i){
	  case 0x105:
      return decinfo.isa.stvec;
	  case 0x142:
      return decinfo.isa.scause;
	  case 0x100:
      return decinfo.isa.sstatus;
	  case 0x141:
      return decinfo.isa.sepc;
	  default:
      assert(0 && "Unfinished readcsr");
	}
}

void writecsr(int i, int32_t val){
	switch(i){
	  case 0x105:
      decinfo.isa.stvec = val;
      break;
	  case 0x142:
      decinfo.isa.scause = val;
      break;
	  case 0x100:
      decinfo.isa.sstatus = val;
      break;
	  case 0x141:
      decinfo.isa.sepc = val;
      break;
	  default:
      assert(0 && "Unfinished readcsr");
	}
}

make_EHelper(system){
	switch(decinfo.isa.instr.funct3){
		/* ecall */
		case 0b0:
	    if((decinfo.isa.instr.val & ~(0x7f))==0){
	        raise_intr(reg_l(17), decinfo.seq_pc - 4);
	    }
	    else if(decinfo.isa.instr.val == 0x10200073){
	        decinfo.is_jmp = 1;
	    }
	    else{
	        assert(0 && "system code unfinish");
	    }
	    break;
		/* csrrw */
		case 0b001:
	    s0 = readcsr(decinfo.isa.instr.csr);
	    writecsr(decinfo.isa.instr.csr, id_src->val);
	    rtl_sr(id_dest->reg, &s0, 4);
	    break;
		case 0b010:
	    s0 = readcsr(decinfo.isa.instr.csr);
	    writecsr(decinfo.isa.instr.csr, s0 | id_src->val);
	    rtl_sr(id_dest->reg, &s0, 4);
	    break;
		default:
	    assert(0 && "Unfinished system op");
	}
}
