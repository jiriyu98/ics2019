#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc) {
  rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(auipc);
}

make_EHelper(addi) {
  rtl_add(&id_dest->val, &id_src->val, &id_src2->val); 
  rtl_sr(id_dest->reg, &id_dest->val, 4); 
  print_asm_template2(addi);
}

make_EHelper(slli) {
  rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(slli); 
}

make_EHelper(slti) {
  rtl_li(&id_dest->val, interpret_relop(RELOP_LT, id_src->val, id_src2->val));
  rtl_sr(id_dest->reg, &id_dest->val, 4);
	print_asm_template2(slti);
}

make_EHelper(sltiu) {
  rtl_li(&id_dest->val, interpret_relop(RELOP_LTU, id_src->val, id_src2->val));
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(sltiu); 
}

make_EHelper(xori){
	rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(xori); 
}

make_EHelper(srli_srai){
  switch(decinfo.isa.instr.funct7) {
    case 0b0000000: /* srli */
      rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template2(srli); 
      break;
 
    case 0b0100000: /* srai */
		  rtl_sar(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template2(srai);
		  break;

    default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}


make_EHelper(ori){
	rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(ori); 
}
make_EHelper(andi){
	rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &id_dest->val, 4);
  print_asm_template2(andi); 
}

make_EHelper(sub_add){
	  switch(decinfo.isa.instr.funct7) {
    case 0b0000000: /* add */
      rtl_add(&id_dest->val, &id_src->val, &id_src2->val); 
      print_asm_template3(add);
      break;
    case 0b0100000: /* sub */ 
      rtl_sub(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sub);
      break;
    case 0b0000001: /* mul */
      rtl_imul_lo(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mul);
      break;

    default: 
			assert(0);
  } 
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(sll){
  switch(decinfo.isa.instr.funct7) {
    case 0b0000000: /* sll */
      rtl_shl(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(sll); 
      break;
    case 0b0000001: /* mulh */
      rtl_imul_hi(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mulh);
      break;
    default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(slt){
  switch(decinfo.isa.instr.funct7) {
    case 0b0000000: /* slt */
      rtl_li(&id_dest->val, interpret_relop(RELOP_LT, id_src->val, id_src2->val));
      print_asm_template3(slt); 
      break;
    case 0b0000001: /* mulhsu */
      rtl_mul_hsu(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mulhsu);
      break;
    default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(sltu){
  switch(decinfo.isa.instr.funct7) {
    case 0b0000000: /* sltu */
      rtl_li(&id_dest->val, interpret_relop(RELOP_LTU, id_src->val, id_src2->val));
      print_asm_template3(sltu); 
      break;
    case 0b0000001: /* mulhu */
      rtl_mul_hi(&id_dest->val, &id_src->val, &id_src2->val);
      print_asm_template3(mulhu);
      break;
    default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(xor){
  switch(decinfo.isa.instr.funct7) {
		case 0b0000000: /* xor */
		  rtl_xor(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(xor); 
		  break;
		case 0b0000001: /* div */
		  rtl_idiv_q(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(div);
		  break;
		default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(srl_sra){
  switch(decinfo.isa.instr.funct7) {
		case 0b0000000: /* srl */
		  rtl_shr(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(srl); 
		  break;
		case 0b0000001: /* divu */
		  rtl_div_q(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(divu);
		  break;
		default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(or){
  switch(decinfo.isa.instr.funct7) {
		case 0b0000000: /* or */
		  rtl_or(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(or); 
		  break;
		case 0b0000001: /* rem */
		  rtl_idiv_r(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(rem);
		  break;
		default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}

make_EHelper(and){
  switch(decinfo.isa.instr.funct7) {
		case 0b0000000: /* and */
		  rtl_and(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(and); 
		  break;
		case 0b0000001: /* remu */
		  rtl_div_r(&id_dest->val, &id_src->val, &id_src2->val);
		  print_asm_template3(remu);
		  break;
		default: 
			assert(0);
  }
  rtl_sr(id_dest->reg, &id_dest->val, 4);
}
