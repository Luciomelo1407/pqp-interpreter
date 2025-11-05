#include "../structure/cpu.h"
#ifndef instructions_h
#define instructions_h
/*
 *  Ana    -> 4*movs
 *  Lúcio  -> cmp, -jmp-, sar
 *  Maiara -> add, sub, and
 *  Paulo  -> jmpg, jmpl, jmpe
 *  Rafa   -> xor, or, sal
 * */

void jmp(Cpu* cpu, uint16_t thirdField);
int exit_setup(Cpu* cpu);

#endif
