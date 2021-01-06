#include "cpu/exec.h"

make_EHelper(jal){
   s0 = cpu.pc + 4;
   rtl_sr(id_dest->reg, &s0, 4);
   rtl_add(&decinfo.jmp_pc, &id_src->val, &cpu.pc);
   interpret_rtl_j(decinfo.jmp_pc); 
   print_asm_template2(jal);
}

make_EHelper(jalr){
    s0 = cpu.pc + 4;  
    rtl_sr(id_dest->reg, &s0, 4);
	rtl_add(&decinfo.jmp_pc, &id_src->val, &id_src2->val);  
    interpret_rtl_j(decinfo.jmp_pc); 
    print_asm_template2(jalr);   
}

make_EHelper(beq){
	rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(beq);
}

make_EHelper(bne){
	rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(bne);
}

make_EHelper(blt){
	rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(blt);
}

make_EHelper(bge){
	rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(bge);
}

make_EHelper(bltu){
	rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(bltu);
}

make_EHelper(bgeu){
	rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
	print_asm_template3(bgeu);
}

