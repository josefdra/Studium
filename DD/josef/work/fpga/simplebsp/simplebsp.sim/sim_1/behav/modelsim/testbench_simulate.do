######################################################################
#
# File name : testbench_simulate.do
# Created on: Tue Oct 24 11:18:28 CEST 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vsim -voptargs="+acc" -L xil_defaultlib -L secureip -lib xil_defaultlib xil_defaultlib.testbench

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {testbench_wave.do}

view wave
view structure
view signals

do {testbench.udo}

run 1000ns
