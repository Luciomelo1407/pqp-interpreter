#include "cpu.h"
#include "../instructions/instructions.h"
#include <iso646.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *input;

void initcpu(Cpu *cpu, char *outputPath) {
  cpu->e = false;
  cpu->l = false;
  cpu->g = false;
  cpu->pc = 0;
  cpu->instructionsCounter = calloc(16, sizeof(int));
  cpu->registers = calloc(16, sizeof(uint32_t));
  cpu->mem = calloc(256, sizeof(uint8_t));
  cpu->output = fopen(outputPath, "w+");
  cpu->duplicateCounter = calloc(16, sizeof(uint32_t));
  cpu->duplicate = calloc(16, sizeof(int *));
  for (int i = 0; i < 16; i++) {
    cpu->duplicate[i] = calloc(500000, sizeof(int));
  }
}

void loadMemory(char *inputPath, Cpu *cpu) {
  input = fopen(inputPath, "r");
  if (!input) {
    perror("Error opening input file");
    return;
  }

  int32_t counter = 0;
  while (counter < 256 && fscanf(input, "%hhx", &cpu->mem[counter]) == 1) {
    counter++;
  }

  printf("Loaded %d bytes into memory.\n", counter);
  for (int i = 0; i < counter; i++) {
    printf("0x%02X ", cpu->mem[i]);
    if ((i + 1) % 4 == 0)
      printf("\n");
  }

  fclose(input);
}

int mainloop(Cpu *cpu) {
  int counter = 0;
  while (true) {
    if (cpu->pc == 0xF0F0) {
      return exit_setup(cpu);
    }

    uint8_t opcode = cpu->mem[cpu->pc];
    uint8_t firstField = (cpu->mem[cpu->pc + 1] & 0b11110000) >> 4;
    uint8_t secondField = cpu->mem[cpu->pc + 1] & 0b00001111;

    uint16_t thirdField = ((uint16_t)cpu->mem[cpu->pc + 3] << 8) |
                          ((uint16_t)cpu->mem[cpu->pc + 2]);

    // printf("opcode: 0x%02X\n", opcode);
    // printf("firstField: 0x%X\n", firstField);
    // printf("secondField: 0x%X\n", secondField);
    // printf("thirdField: 0x%04X\n", thirdField);
    // printf("\nDEBUG: PC = 0x%X, %d\n", cpu->pc, cpu->pc);

    switch (opcode) {
    case 0x00:
      mov0(cpu, firstField, thirdField);
      break;
    case 0x01:
      mov01(cpu, firstField, secondField);
      break;
    case 0x02:
      mov02(cpu, firstField, secondField);
      break;
    case 0x03:
      mov03(cpu, firstField, secondField);
      break;
    case 0x04:
      cmp(cpu, firstField, secondField);
      break;
    case 0x05:
      jmp(cpu, thirdField);
      break;
    case 0x06:
      jmp_g(cpu, thirdField);
      break;
    case 0x07:
      jmp_l(cpu, thirdField);
      break;
    case 0x08:
      jmp_e(cpu, thirdField);
      break;
    case 0x09:
      add(cpu, firstField, secondField);
      break;
    case 0x0A:
      sub(cpu, firstField, secondField);
      break;
    case 0x0B:
      aNd(cpu, firstField, secondField);
      break;
    case 0x0C:
      oR(cpu, firstField, secondField);
      break;
    case 0x0D:
      xOr(cpu, firstField, secondField);
      break;
    case 0x0E:
      sal(cpu, firstField, thirdField);
      break;
    case 0x0F:
      sar(cpu, firstField, thirdField);
      break;
    default:
      printf("\n\nNAO ACHOU A INSTRUÇÀO\n\n");
      return -1;
      break;
    }
    cpu->pc += 4;
    counter++;
  }
  return 0;
}
