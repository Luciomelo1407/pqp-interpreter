#include "cpu.h"
#include "../instructions/instructions.h"
#include <stdint.h>
#include <stdio.h>

FILE *input;

void initcpu(Cpu *cpu, char* outputPath) {
  cpu->e = false;
  cpu->l = false;
  cpu->g = false;
  cpu->pc = 0;
  cpu->output = fopen(outputPath, "w+");
}

void loadMemory(char *inputPath, Cpu *cpu) {
  input = fopen(inputPath, "r");
  if (!input) {
    perror("Error opening input file");
    return;
  }

  int32_t counter = 0;
  while (counter < 40000 && fscanf(input, "%hhx", &cpu->mem[counter]) == 1) {
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
  while(true){
    uint8_t opcode = cpu->mem[cpu->pc];
    uint8_t firstField = (cpu->mem[cpu->pc+1]&0b11110000)>>4;
    uint8_t secondField = cpu->mem[cpu->pc+1]&0b00001111;
    int16_t thirdField = ((int16_t)cpu->mem[cpu->pc+3]<<8) 
                        | ((uint16_t)cpu->mem[cpu->pc+2]);

    printf("opcode: 0x%02X\n",opcode);
    printf("firstField: 0x%X\n",firstField);
    printf("secondField: 0x%X\n",secondField);
    printf("thirdField: 0x%04X\n",thirdField);
    
    switch (opcode) {
      case 0x00:
        break;
      case 0x01:
        break;
      case 0x02:
        break;
      case 0x03:
        break;
      case 0x04:
        break;
      case 0x05:
        jmp(cpu,thirdField);
        break;
      default:
        return -1;
        break;
    }
    cpu->pc+=4;
  }
  return 0;
}
