#include "instructions.h"
#include <stdint.h>
#include <stdio.h>
// WARN: SAIR INCREMENTANDO O DUPLICATED COUNTER EM TD MUNDO
void checkDuplicated(Cpu *cpu, uint8_t instructionCode, bool *print) {

  for (int i = 0; i < cpu->duplicateCounter[instructionCode]; i++) {
    if (cpu->duplicate[instructionCode][i] == cpu->pc) {
      *print = false;
    }
  }
}

void mov0(Cpu *cpu, uint8_t firstField, uint16_t thirdField) {
  cpu->instructionsCounter[0x00]++;
  cpu->registers[firstField] = (int32_t)(int16_t)thirdField;
  bool print = true;
  checkDuplicated(cpu, 0x00, &print);
  if (print) {
    cpu->duplicateCounter[0x00]++;
    fprintf(cpu->output, "0x%04X->MOV_R%d=0x%08X\n", cpu->pc, firstField,
            cpu->registers[firstField]);
  }
}

void mov01(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  cpu->instructionsCounter[0x01]++;
  cpu->registers[firstField] = cpu->registers[secondField];
  bool print = false;
  checkDuplicated(cpu, 0x01, &print);
  if (print) {
    cpu->duplicateCounter[0x01]++;
    fprintf(cpu->output, "0x%04X->MOV_R%d=R%d=0x%08X\n", cpu->pc, firstField,
            secondField, cpu->registers[firstField]);
  }
}

void mov03(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  printf("\nDEBUGG: mov03 first field: %d, secondField: %d \n", firstField,
         secondField);
  cpu->mem[cpu->registers[firstField] + 3] =
      (uint8_t)cpu->registers[secondField];
  cpu->mem[cpu->registers[firstField] + 2] =
      (uint8_t)(cpu->registers[secondField] >> 8);
  cpu->mem[cpu->registers[firstField] + 1] =
      (uint8_t)(cpu->registers[secondField] >> 16);
  cpu->mem[cpu->registers[firstField]] =
      (uint32_t)(cpu->registers[secondField] >> 24);

  cpu->instructionsCounter[0x03]++;
  bool print = false;
  checkDuplicated(cpu, 0x03, &print);
  if (print) {
    cpu->duplicateCounter[0x03]++;
    fprintf(
        cpu->output,
        "0x%04X->MOV_MEM[0x%02X,0x%02X,0x%02X,0x%02X]=R%d=[0x%02X,0x%02X,0x%"
        "02X,0x%02X]\n",
        cpu->pc, cpu->registers[firstField], cpu->registers[firstField] + 1,
        cpu->registers[firstField] + 2, cpu->registers[firstField] + 3,
        secondField, cpu->mem[cpu->registers[firstField] + 3],
        cpu->mem[cpu->registers[firstField] + 2],
        cpu->mem[cpu->registers[firstField] + 1],
        cpu->mem[cpu->registers[firstField]]);
  }
}

void mov02(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  cpu->registers[firstField] =
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 3]) << 24 |
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 2]) << 16 |
      ((uint32_t)cpu->mem[cpu->registers[secondField] + 1]) << 8 |
      ((uint32_t)cpu->mem[cpu->registers[secondField]]);
  cpu->instructionsCounter[0x02]++;
  bool print = false;
  checkDuplicated(cpu, 0x02, &print);
  if (print) {
    cpu->duplicateCounter[0x02]++;
    fprintf(
        cpu->output,
        "0x%04X->MOV_R%d=MEM[0x%02X,0x%02X,0x%02X,0x%02X]=[0x%02X,0x%02X,0x%"
        "02X,0x%02X]\n",
        cpu->pc, firstField, cpu->registers[secondField],
        cpu->registers[secondField] + 1, cpu->registers[secondField] + 2,
        cpu->registers[secondField] + 3, cpu->mem[cpu->registers[secondField]],
        cpu->mem[cpu->registers[secondField] + 1],
        cpu->mem[cpu->registers[secondField] + 2],
        cpu->mem[cpu->registers[secondField] + 3]);
  }
}

void cmp(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  cpu->instructionsCounter[0x04]++;
  cpu->e = false;
  cpu->g = false;
  cpu->l = false;
  int rx = (int32_t)cpu->registers[firstField],
      ry = (int32_t)cpu->registers[secondField];
  if (rx > ry) {
    cpu->g = true;
  }
  if (rx < ry) {
    cpu->l = true;
  }
  if (rx == ry) {
    cpu->e = true;
  }
  bool print = false;
  checkDuplicated(cpu, 0x04, &print);
  if (print) {
    cpu->duplicateCounter[0x04]++;
    fprintf(cpu->output, "0x%04X->CMP_R%d<=>R%d(G=%d,L=%d,E=%d)\n", cpu->pc,
            firstField, secondField, cpu->g, cpu->l, cpu->e);
  }
}

void jmp(Cpu *cpu, uint16_t thirdField) {
  bool print = false;
  checkDuplicated(cpu, 0x05, &print);
  cpu->instructionsCounter[0x05]++;
  if (print) {
    cpu->duplicateCounter[0x05]++;
    fprintf(cpu->output, "0x%04X->JMP_0x%04X\n", cpu->pc,
            cpu->pc + (int16_t)thirdField + 4);
    cpu->pc += (int16_t)thirdField;
  }
}

void jmp_g(Cpu *cpu, uint16_t thirdField) {
  bool print = false;
  checkDuplicated(cpu, 0x06, &print);
  cpu->instructionsCounter[0x06]++;
  if (cpu->g) {
    if (print) {
      cpu->duplicateCounter[0x06]++;
      fprintf(cpu->output, "0x%04X->JG_0x%04X\n", cpu->pc,
              cpu->pc + (int16_t)thirdField + 4);
    }
    cpu->pc += (int16_t)thirdField;
  } else {
    if (print) {
      cpu->duplicateCounter[0x06]++;
      fprintf(cpu->output, "0x%04X->JG_0x%04X\n", cpu->pc, cpu->pc + 4);
    }
  }
  return;
}

void jmp_l(Cpu *cpu, uint16_t thirdField) {
  bool print = false;
  checkDuplicated(cpu, 0x07, &print);
  cpu->instructionsCounter[0x07]++;
  if (cpu->l) {
    if (print) {
      cpu->duplicateCounter[0x07]++;
      fprintf(cpu->output, "0x%04X->JL_0x%04X\n", cpu->pc,
              cpu->pc + (int16_t)thirdField + 4);
    }
    cpu->pc += (int16_t)thirdField;
  } else {
    if (print) {
      cpu->duplicateCounter[0x07]++;
      fprintf(cpu->output, "0x%04X->JL_0x%04X\n", cpu->pc, cpu->pc + 4);
    }
  }
  return;
}

void jmp_e(Cpu *cpu, uint16_t thirdField) {
  bool print = false;
  checkDuplicated(cpu, 0x08, &print);
  cpu->instructionsCounter[0x08]++;
  if (cpu->e) {
    if (print) {
      cpu->duplicateCounter[0x08]++;
      fprintf(cpu->output, "0x%04X->JE_0x%04X\n", cpu->pc,
              cpu->pc + (int16_t)thirdField + 4);
    }
    cpu->pc += (int16_t)thirdField;
  } else {
    if (print) {
      cpu->duplicateCounter[0x08]++;
      fprintf(cpu->output, "0x%04X->JE_0x%04X\n", cpu->pc, cpu->pc + 4);
    }
  }
  return;
}

void add(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] += cpu->registers[secondField];
  cpu->instructionsCounter[0x09]++;
  bool print = false;
  checkDuplicated(cpu, 0x09, &print);
  if (print) {
    cpu->duplicateCounter[0x09]++;
    fprintf(cpu->output, "0x%04X->ADD_R%d+=R%d=0x%08X+0x%08X=0x%08X\n", cpu->pc,
            firstField, secondField, rx, ry, cpu->registers[firstField]);
  }
}

void sub(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] -= cpu->registers[secondField];
  cpu->instructionsCounter[0x0A]++;
  bool print = false;
  checkDuplicated(cpu, 0x0a, &print);
  if (print) {
    cpu->duplicateCounter[0x0a]++;
    fprintf(cpu->output, "0x%04X->SUB_R%d-=R%d=0x%08X-0x%08X=0x%08X\n", cpu->pc,
            firstField, secondField, rx, ry, cpu->registers[firstField]);
  }
}

void aNd(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] =
      cpu->registers[firstField] & cpu->registers[secondField];
  cpu->instructionsCounter[0x0B]++;
  bool print = false;
  checkDuplicated(cpu, 0x0b, &print);
  if (print) {
    cpu->duplicateCounter[0x0b]++;
    fprintf(cpu->output, "0x%04X->AND_R%d&=R%d=0x%08X&0x%08X=0x%08X\n", cpu->pc,
            firstField, secondField, rx, ry, cpu->registers[firstField]);
  }
}

void oR(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] =
      cpu->registers[firstField] | cpu->registers[secondField];
  cpu->instructionsCounter[0x0C]++;
  bool print = false;
  checkDuplicated(cpu, 0x0c, &print);
  if (print) {
    cpu->duplicateCounter[0x0c]++;
    fprintf(cpu->output, "0x%04X->OR_R%d|=R%d=0x%08X|0x%08X=0x%08X\n", cpu->pc,
            firstField, secondField, rx, ry, cpu->registers[firstField]);
  }
}

void xOr(Cpu *cpu, uint8_t firstField, uint8_t secondField) {
  uint32_t rx = cpu->registers[firstField], ry = cpu->registers[secondField];
  cpu->registers[firstField] =
      cpu->registers[firstField] ^ cpu->registers[secondField];
  cpu->instructionsCounter[0x0D]++;
  bool print = false;
  checkDuplicated(cpu, 0x0d, &print);
  if (print) {
    cpu->duplicateCounter[0x0d]++;
    fprintf(cpu->output, "0x%04X->XOR_R%d^=R%d=0x%08X^0x%08X=0x%08X\n", cpu->pc,
            firstField, secondField, rx, ry, cpu->registers[firstField]);
  }
}

void sar(Cpu *cpu, uint8_t firstField, uint16_t thirdField) {
  int i = (uint8_t)(thirdField >> 8);
  uint32_t before = cpu->registers[firstField];
  cpu->registers[firstField] = ((int32_t)cpu->registers[firstField]) >> i;
  cpu->instructionsCounter[0x0F]++;
  bool print = false;
  checkDuplicated(cpu, 0x0f, &print);
  if (print) {
    cpu->duplicateCounter[0x0f]++;
    fprintf(cpu->output, "0x%04X->SAR_R%d>>=%d=0x%08X>>%d=0x%08X\n", cpu->pc,
            firstField, i, before, i, cpu->registers[firstField]);
  }
}

void sal(Cpu *cpu, uint8_t firstField, uint16_t thirdField) {
  int i = (uint8_t)(thirdField >> 8);
  uint32_t before = cpu->registers[firstField];
  cpu->registers[firstField] = (cpu->registers[firstField]) << i;
  cpu->instructionsCounter[0x0E]++;
  bool print = false;
  checkDuplicated(cpu, 0x0e, &print);
  if (print) {
    cpu->duplicateCounter[0x0e]++;
    fprintf(cpu->output, "0x%04X->SAL_R%d<<=%d=0x%08X<<%d=0x%08X\n", cpu->pc,
            firstField, i, before, i, cpu->registers[firstField]);
  }
}

int exit_setup(Cpu *cpu) {
  fprintf(cpu->output, "0x%04X->EXIT\n", cpu->pc);
  fprintf(cpu->output, "[");
  for (int i = 0; i < 16; i++) {
    fprintf(cpu->output, "%02X:%d", i, cpu->instructionsCounter[i]);
    if (i + 1 < 16) {
      fprintf(cpu->output, ",");
    }
  }
  fprintf(cpu->output, "]\n");

  fprintf(cpu->output, "[");
  for (int i = 0; i < 16; i++) {
    fprintf(cpu->output, "R%d=0x%08X", i, cpu->registers[i]);
    if (i + 1 < 16) {
      fprintf(cpu->output, ",");
    }
  }
  fprintf(cpu->output, "]\n");
  return 0;
}
