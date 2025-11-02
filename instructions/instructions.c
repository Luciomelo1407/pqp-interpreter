#include "instructions.h"
#include <stdio.h>

void jmp(Cpu* cpu, int16_t thirdField){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X", cpu->pc,cpu->pc+thirdField+4);
  cpu->pc+=thirdField;
}
