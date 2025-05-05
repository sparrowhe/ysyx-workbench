#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Vencode42.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

Vencode42* top = nullptr;
VerilatedContext* contextp = nullptr;

void init_sim(int argc, char** argv) {
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);

    top = new Vencode42{contextp};
    // if (enable_waveform) {
    //     Verilated::traceEverOn(true);
    //     tfp = new VerilatedVcdC;
    //     top->trace(tfp, 0);
    //     tfp->open("light.vcd");
    // }
}

void cleanup() {
    // if (enable_waveform && tfp) {
    //     tfp->close();
    //     delete tfp;
    // }
    delete top;
    delete contextp;
}

int encode42(int x) {
    top->x = x;
    top->eval();
    int result = top->y;
    return result;
}

int main(int argc, char** argv) {
    init_sim(argc, argv);

    // stage 1: EN = 1

    top->en = 1;

    for (int i = 0; i < 4; i++) {
        int x = 1 << i;
        int result = encode42(x);
        printf("x = 0x%X, expected y = %d, got y = %d\n", x, i, result);
        assert(result == i);
    }

    // stage 2: EN = 0
    top->en = 0;
    for (int i = 0; i < 4; i++) {
        int x = 1 << i;
        int result = encode42(x);
        printf("x = 0x%X, expected y = 0, got y = %d\n", x, result);
        assert(result == 0);
    }
    printf("All tests passed!\n");
    cleanup();
    return 0;
}
