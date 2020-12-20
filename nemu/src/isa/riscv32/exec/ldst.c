#include "cpu/exec.h"

make_EHelper(ld) {
  rtl_lm(&s0, &id_src->addr, decinfo.width);
  rtl_sr(id_dest->reg, &s0, 4);

  printf("load : 0x%x", s0);

  switch (decinfo.width) {
    case 4: print_asm_template2(lw); break;
    case 2: print_asm_template2(lhu); break;
    case 1: print_asm_template2(lbu); break;
    default: assert(0);
  }
}

make_EHelper(lh) {
  rtl_lm(&s0, &id_src->addr, decinfo.width);
  rtl_sext(&s1, &s0, 2); print_asm_template2(lh);
  rtl_sr(id_dest->reg, &s1, 4);  
}

make_EHelper(lb) {
  rtl_lm(&s0, &id_src->addr, decinfo.width);
  rtl_sext(&s1, &s0, 1); print_asm_template2(lb);
  rtl_sr(id_dest->reg, &s1, 4);  
}

make_EHelper(st) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);

  switch (decinfo.width) {
    case 4: print_asm_template2(sw); break;
    case 2: print_asm_template2(sh); break;
    case 1: print_asm_template2(sb); break;
    default: assert(0);
  }
}
