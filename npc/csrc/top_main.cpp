#include <stdio.h>
#include <stdlib.h>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

vluint64_t main_time = 0; 

int main(int argc, char** argv) {
  printf("Hello, ysyx!\n");
  VerilatedContext* contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtop* top = new Vtop{contextp};
  VerilatedVcdC* tfp = new VerilatedVcdC;
  Verilated::traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("top.vcd");
  while (!contextp->gotFinish()) {
    int a = rand() & 1;
    int b = rand() & 1;
    top->a = a;
    top->b = b;
    top->eval();
    tfp->dump(main_time); //dump wave
    main_time++;
    printf("a = %d, b = %d, f = %d\n", a, b, top->f);
    assert(top->f == (a ^ b));
  }
  tfp->close();
  delete top;
  delete contextp;
  return 0;
}
