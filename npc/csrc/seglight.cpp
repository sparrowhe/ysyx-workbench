#include <stdio.h>
#include <stdlib.h>
#include <nvboard.h>
#include "Vseglight.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

void nvboard_bind_all_pins(Vseglight* top);

vluint64_t main_time = 0;  // 仿真时间全局变量
bool enable_waveform = false;
Vseglight* top = nullptr;
VerilatedContext* contextp = nullptr;
VerilatedVcdC* tfp = nullptr;

void single_cycle() {
    top->eval();
    if (enable_waveform) {
        tfp->dump(main_time++);
    }
}

// void reset(int cycles) {
//     top->rst = 1;
//     for (int i = 0; i < cycles; ++i) single_cycle();
//     top->rst = 0;
// }

void init_sim(int argc, char** argv) {
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);

    top = new Vseglight{contextp};
    if (enable_waveform) {
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        top->trace(tfp, 0);
        tfp->open("seglight.vcd");
    }
}

void cleanup() {
    if (enable_waveform && tfp) {
        tfp->close();
        delete tfp;
    }
    nvboard_quit();
    delete top;
    delete contextp;
}

int main(int argc, char** argv) {
    init_sim(argc, argv);
    nvboard_bind_all_pins(top);
    nvboard_init();
    
    while (!contextp->gotFinish()) {
        nvboard_update();
        single_cycle();
    }

    cleanup();
    return 0;
}
