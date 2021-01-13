#include "cpu/exec.h"

extern void raise_intr(uint32_t, vaddr_t);
int32_t readcsr(int i){
	switch(i){
	  case 0x105:
      return cpu.stvec.val;
	  case 0x142:
      return cpu.scause.val;
	  case 0x100:
      return cpu.sstatus.val;
	  case 0x141:
      return cpu.sepc.val;
      case 0x180:
      return cpu.satp.val;
      break;
	  default:
      assert(0 && "Unfinished readcsr");
	}
}

void writecsr(int i, int32_t val){
	switch(i){
	  case 0x105:
      cpu.stvec.val = val;
      break;
	  case 0x142:
      cpu.scause.val = val;
      break;
	  case 0x100:
      cpu.sstatus.val = val;
      break;
	  case 0x141:
      cpu.sepc.val = val;
      break;
      case 0x180:
      cpu.satp.val = val;
      break;
	  default:
      assert(0 && "Unfinished writecsr");
	}
}

make_EHelper(system){
	switch(decinfo.isa.instr.funct3){
		/* ecall & sret */
		case 0b0:
	    if((decinfo.isa.instr.val & ~(0x7f))==0){
	        raise_intr(reg_l(17), decinfo.seq_pc - 4);
	    }
	    else if(decinfo.isa.instr.val == 0x10200073){
	    	cpu.sstatus.SIE = cpu.sstatus.SPIE;
	    	cpu.sstatus.SPIE = 1;
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
	    /* csrrs */
		case 0b010:
	    s0 = readcsr(decinfo.isa.instr.csr);
	    writecsr(decinfo.isa.instr.csr, s0 | id_src->val);
	    rtl_sr(id_dest->reg, &s0, 4);
	    break;
		default:
	    assert(0 && "Unfinished system op");
	}
}
