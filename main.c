#include "structure/cpu.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    Cpu cpu;
    initcpu(&cpu);
    loadMemory(argv[1], &cpu);
    mainloop(&cpu);

    return 0;
}

