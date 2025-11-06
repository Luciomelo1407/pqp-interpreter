#include "instructions.h"
#include <stdint.h>
#include <stdio.h>

void mov02(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  cpu->registers[firstField] =
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 3]) << 24 |
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 2]) << 16 |
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 1]) << 8 |
      ((uint32_t)cpu->mem[cpu->registers[secondField]]);
  fprintf(cpu->output,
          "0x%04X->MOV_R%d=MEM[0x%02X,0x%02X,0x%02X,0x%02X]=[0x%02X,0x%02X,0x%"
          "02X,0x%02X]\n",
          cpu->pc, firstField, cpu->registers[secondField],
          cpu->registers[secondField] + 1, cpu->registers[secondField] + 2,
          cpu->registers[secondField] + 3,
          cpu->mem[cpu->registers[secondField]],
          cpu->mem[cpu->registers[secondField] + 1],
          cpu->mem[cpu->registers[secondField] + 2],
          cpu->mem[cpu->registers[secondField] + 3]);
  cpu->instructionsCounter[0x02]++;
}

  void mov00(Cpu* cpu, uint8_t firstField, uint8_t secondField){
    uint8_t highByte = cpu->mem[cpu->pc +2];
    uint8_t lowByte = cpu->mem[cpu->pc +3];
    uint16_t i16 = ((uint16_t)highByte << 8) | lowByte;
    uint16_t signed_i16 = (int16_t)i16;
    cpu->registers[firstField] = (uint32_t)signed_i16;

    fprintf(cpu ->output, 
        "ox%o4X->MOV_R%d=0x%08X\n",
        cpu->pc,
        firstField,
        cpu->registers[firstField]);
        cpu->instructionsCounter[0x00]++;
  }

  void mov01(Cpu* cpu, uint8_t firstField, uint8_t secondField){
      uint32_t valorOrigem = cpu->registers[secondField];
      cpu->registers[firstField] = valorOrigem;
      fprintf(cpu->output,
          "0x%04X->MOV_R%d=0x%08X\n",
          cpu->pc,
          firstField,
          secondField,
          valorOrigem); 
        cpu->instructionsCounter[0x01]++;
    }

  void mov03(Cpu* cpu, uint8_t firstField, uint8_t secondField){
    uint32_t endereco = cpu->registers[firstField];
    uint32_t valor = cpu->registers[secondField];
    uint8_t byte0_lsb = (valor >> 0) & 0xFF;
    uint8_t byte1 = (valor >> 8) & 0xFF;
    uint8_t byte2 = (valor >>16) & 0xFF;
    uint8_t byte3_msb = (valor >> 24) & 0xFF;

    cpu->mem[endereco + 0] = byte0_lsb;
    cpu->mem[endereco + 1] = byte1;
    cpu->mem[endereco + 2] = byte2;
    cpu->mem[endereco + 3] = byte3_msb;

    fprintf(
      cpu->output,
      "0x%04X->MOV_MEM[0x%02X,0x%02X,0x%02X,0x%02X]=R%d=[0x%02X,%0x2X,%0x2X,%0x2X]\n",
      cpu->pc,
      endereco + 1,
      endereco + 2,
      endereco + 3,
      secondField,
      byte0_lsb,
      byte1,
      byte2,
      byte3_msb
    );
    cpu->instructionsCounter[0x03]++;
  }


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
  if(cpu->g){
  fprintf(cpu->output,"0x%04X->JG_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x06]++;
  cpu->pc+=thirdField;
}
  else return;
}

void jmp_l(Cpu* cpu, uint16_t thirdField){
  if(cpu->l){
  fprintf(cpu->output,"0x%04X->JL_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x07]++;
  cpu->pc+=thirdField;
}
  else return;
}

void jmp_e(Cpu* cpu, uint16_t thirdField){
  if(cpu->e){
  fprintf(cpu->output,"0x%04X->JE_0x%04X\n", cpu->pc,cpu->pc+thirdField+4);
  cpu->instructionsCounter[0x08]++;
  cpu->pc+=thirdField;
}
  else return;
}

void add(Cpu* cpu, uint8_t firstField, uint8_t secondField){
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] += cpu->registers[secondField];
  fprintf(cpu->output,"0x%04X->ADD_R%d+=R%d=0x%08X+0x%08X=0x%08X", cpu->pc,firstField,secondField,rx,ry,cpu->registers[firstField]);
  cpu->instructionsCounter[0x09]++;
}

void sub(Cpu* cpu, uint8_t firstField, uint8_t secondField){
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] -= cpu->registers[secondField];
  fprintf(cpu->output,"0x%04X->SUB_R%d-=R%d=0x%08X-0x%08X=0x%08X", cpu->pc,firstField,secondField,rx,ry,cpu->registers[firstField]);
  cpu->instructionsCounter[0x0A]++;
}


void aNd(Cpu* cpu, uint8_t firstField, uint8_t secondField){
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] = cpu->registers[firstField] & cpu->registers[secondField];
  fprintf(cpu->output,"0x%04X->SUB_R%d&=R%d=0x%08X&0x%08X=0x%08X", cpu->pc,firstField,secondField,rx,ry,cpu->registers[firstField]);
  cpu->instructionsCounter[0x0B]++;
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
