#include "instructions.h"
#include <stdio.h>

void jmp(Cpu* cpu, int16_t thirdField){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}


void jmp_g(Cpu* cpu, int16_t thirdField){
  if(cpu->g && !(cpu->e) && !(cpu->l)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}
  else return;
}
void jmp_l(Cpu* cpu, int16_t thirdField){
  if(cpu->l && !(cpu->g) && !(cpu->e)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}
  else return;
}
void jmp_e(Cpu* cpu, int16_t thirdField){
  if(cpu->e && !(cpu->g) && !(cpu->l)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}
  else return;
}

