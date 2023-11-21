######################################################################
#
# File name : TB_MODULE_simulate.do
# Created on: Mon Nov 20 19:49:00 CET 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vsim -voptargs="+acc" -L xil_defaultlib -L secureip -lib xil_defaultlib xil_defaultlib.TB_MODULE

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {TB_MODULE_wave.do}

view wave
view structure
view signals

do {TB_MODULE.udo}

run 1000ns