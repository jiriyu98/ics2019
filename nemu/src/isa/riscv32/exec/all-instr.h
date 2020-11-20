#include "cpu/exec.h"

make_EHelper(lui);
make_EHelper(auipc);

/* J */
make_EHelper(jal);
make_EHelper(jalr);

/* B */
make_EHelper(beq);
make_EHelper(bne);
make_EHelper(blt);
make_EHelper(bge);
make_EHelper(bltu);
make_EHelper(bgeu);

/* I */
make_EHelper(addi);     /* 000 */
make_EHelper(slli);     /* 001 */
make_EHelper(slti);     /* 010 */ 
make_EHelper(sltiu);    /* 011 */
make_EHelper(xori);     /* 100 */
make_EHelper(srli_srai);/* 101 */
make_EHelper(ori);      /* 110 */
make_EHelper(andi);     /* 111 */


/* R */
make_EHelper(sub_add);
make_EHelper(sll);     /* 001 */
make_EHelper(slt);     /* 010 */ 
make_EHelper(sltu);    /* 011 */
make_EHelper(xor);     /* 100 */
make_EHelper(srl_sra);/* 101 */
make_EHelper(or);      /* 110 */
make_EHelper(and);     /* 111 */

/* ldst */
make_EHelper(ld);
make_EHelper(lb);
make_EHelper(lh);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);
