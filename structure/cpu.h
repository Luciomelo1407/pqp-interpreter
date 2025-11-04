#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef CPU_H
#define CPU_H

typedef struct {
    int16_t registers[16];
    uint8_t mem[256];
    bool g, l, e;
    int32_t pc;
    int instructionsCounter[16];
    FILE *output;
} Cpu;

void initcpu(Cpu *cpu, char* outputPath);
void loadMemory(char *inputPath, Cpu *cpu);
int mainloop(Cpu *cpu);

#endif
