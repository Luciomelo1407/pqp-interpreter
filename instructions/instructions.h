#include "../structure/cpu.h"
#include <stdint.h>
#ifndef instructions_h
#define instructions_h
/*
 *  Ana    -> 4*movs
 *  Lúcio  -> *cmp*, -jmp-, *sar* // TODO revisar asteriscos
 *  Maiara -> *add*, *sub*, *and*
 *  Paulo  -> *jmpg*, *jmpl*, *jmpe* //TODO revisar isso aqui assing: Lúcio
 *  Rafa   -> xor, or, sal
 * */

void cmp(Cpu* cpu, uint8_t firstField, uint8_t secondField );
void jmp(Cpu* cpu,   uint16_t thirdField);
void jmp_g(Cpu* cpu, uint16_t thirdField);
void jmp_l(Cpu* cpu, uint16_t thirdField);
void jmp_e(Cpu* cpu, uint16_t thirdField);
void add(Cpu* cpu, uint8_t firstField, uint8_t secondField);
void sub(Cpu* cpu, uint8_t firstField, uint8_t secondField);
void aNd(Cpu* cpu, uint8_t firstField, uint8_t secondField);
void sar(Cpu* cpu, uint8_t firstField, uint16_t thirdField);
void xOr(Cpu* cpu, uint8_t firstField, uint8_t secondField);
void oR(Cpu* cpu,  uint8_t firstField, uint8_t secondField);
void sal(Cpu* cpu, uint8_t firstField, uint16_t thirdField);

int exit_setup(Cpu* cpu);

#endif
