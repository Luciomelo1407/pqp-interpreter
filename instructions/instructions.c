#include "instructions.h"
#include <stdint.h>
#include <stdio.h>


void cmp(Cpu* cpu,uint8_t firstField, uint8_t secondField ){
  cpu->e = true;
  cpu->g = true;
  cpu->l = true;
  int rx = (int32_t)cpu->registers[firstField],
  ry = (int32_t)cpu->registers[secondField];
  
  if(rx > ry){
    cpu->g = true;
  }
  if(rx < ry){
    cpu->l = true;
  }
  if(rx == ry){
    cpu->e = true;
  }
  fprintf(cpu->output,"0x%04X->CMP_R%d<=>R%d(G=%d,L=%d,E=%d)\n",  cpu->pc,firstField,secondField,cpu->g,cpu->l,cpu->e);
  
}

void jmp(Cpu* cpu, uint16_t thirdField){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x05]++;
  cpu->pc+=thirdField;
}

void jmp_g(Cpu* cpu, uint16_t thirdField){
  if(cpu->g && !(cpu->e) && !(cpu->l)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x06]++;
  cpu->pc+=thirdField;
}
  else return;
}

void jmp_l(Cpu* cpu, uint16_t thirdField){
  if(cpu->l && !(cpu->g) && !(cpu->e)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x07]++;
  cpu->pc+=thirdField;
}
  else return;
}

void jmp_e(Cpu* cpu, uint16_t thirdField){
  if(cpu->e && !(cpu->g) && !(cpu->l)){
  fprintf(cpu->output,"0x%04X->JMP_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x08]++;
  cpu->pc+=thirdField;
}
  else return;
}

void sar(Cpu* cpu,uint8_t firstField, uint16_t thirdField){
  int i = ((uint8_t)thirdField)>>4;
  uint32_t  before = cpu->registers[firstField];
  cpu->registers[firstField] = ((int32_t)cpu->registers[firstField])>>i;
  cpu->instructionsCounter[0x0F]++;
  fprintf(cpu->output,"0x%04X->SAR_R%d>>=%d=0x%08X>>=%d=0x%08X\n", cpu->pc,
          firstField,i,before,i,cpu->registers[firstField]);
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
