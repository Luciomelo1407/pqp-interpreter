#include "instructions.h"
#include <stdio.h>

void jmp(Cpu* cpu, uint16_t thirdField){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}


int exit_setup(Cpu* cpu){
  fprintf(cpu->output,"0x%04X->EXIT\n", cpu->pc);
  fprintf(cpu->output,"[");
  for (int i = 0; i < 16; i++) {
    fprintf(cpu->output,"%02X:%d",i,cpu->instructionsCounter[i] );
    if(i+1 < 16){
      fprintf(cpu->output,",");
    }
  }
  fprintf(cpu->output,"]\n");


  fprintf(cpu->output,"[");
  for (int i = 0; i < 16; i++) {
    fprintf(cpu->output,"R%d=0x%08X",i,cpu->registers[i]);
    if(i+1 < 16){
      fprintf(cpu->output,",");
    }
  }
  fprintf(cpu->output,"]\n");
  return 0;
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

