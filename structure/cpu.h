#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef CPU_H
#define CPU_H

typedef struct {
    uint32_t* registers;
    uint8_t* mem;
    bool g, l, e;
    int32_t pc;
    int* instructionsCounter;
    int* duplicateCounter;
    int** duplicate;
    FILE *output;
} Cpu;

void initcpu(Cpu *cpu, char* outputPath);
void loadMemory(char *inputPath, Cpu *cpu);
int mainloop(Cpu *cpu);

#endif
