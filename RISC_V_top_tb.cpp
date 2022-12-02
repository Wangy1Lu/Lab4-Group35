#include "verilated.h"
#include "verilated_vcd_c.h"
#include "VRISC_V_top.h"

#include "vbuddy.cpp"     // include vbuddy code
#define MAX_SIM_CYC 100000

int main(int argc, char **argv, char **env) {
  int simcyc;     // simulation clock count
  int tick;       // each clk cycle has two ticks for two edges
  
  Verilated::commandArgs(argc, argv);
  // init top verilog instance
  Vtimed_f1_fsm * top = new Vtimed_f1_fsm;
  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("timed_f1_fsm.vcd");
 
  // init Vbuddy
  if (vbdOpen()!=1) return(-1);
  vbdHeader("L3T3:timed_f1_fsm");
  vbdSetMode(1);        // Flag mode set to one-shot

  // initialize simulation inputs
  top->clk = 1;
  top->rst = 0;
  top->a1=0;
  
  
  // run simulation for MAX_SIM_CYC clock cycles
  for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
    // dump variables into VCD file and toggle clock
    for (tick=0; tick<2; tick++) {
      tfp->dump (2*simcyc+tick);
      top->clk = !top->clk;
      top->eval ();
    }

    // Display toggle neopixel
    vbdBar(top->a1);
    // set up input signals of testbench

    vbdCycle(simcyc);

    if (Verilated::gotFinish())  exit(0);
  }

  vbdClose();     // ++++
  tfp->close(); 
  exit(0);
}