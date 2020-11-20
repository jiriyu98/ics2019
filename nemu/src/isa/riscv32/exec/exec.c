#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry imm_table [8] = {
  EX(addi), EX(slli), EX(slti), EX(sltiu), EX(xori), EX(srli_srai), EX(ori), EX(andi)
};

static make_EHelper(imm) {
  decinfo.width = imm_table[decinfo.isa.instr.funct3].width;
  idex(pc, &imm_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry load_table [8] = {
  EXW(lb, 1), EXW(lh, 2), EXW(ld, 4), EMPTY, EXW(ld, 1), EXW(ld, 2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st, 1), EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry r_table [8] = {
   EX(sub_add), EX(sll), EX(slt), EX(sltu), EX(xor), EX(srl_sra), EX(or), EX(and)
};

static make_EHelper(r) { /* static void exec_r(vaddr_t *pc) */
  decinfo.width = r_table[decinfo.isa.instr.funct3].width;
  idex(pc, &r_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry br_table [8] = {
   EX(beq), EX(bne), EMPTY, EMPTY, EX(blt), EX(bge), EX(bltu), EX(bgeu)
};

static make_EHelper(br) { /* static void exec_r(vaddr_t *pc) */
  decinfo.width = br_table[decinfo.isa.instr.funct3].width;
  idex(pc, &br_table[decinfo.isa.instr.funct3]);
}


static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, imm), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R, r), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B, br), IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
