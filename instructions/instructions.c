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
