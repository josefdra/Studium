######################################################################
#
# File name : TB_SEVENSEG_simulate.do
# Created on: Mon Nov 20 20:03:39 CET 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vsim -voptargs="+acc" -L xil_defaultlib -L secureip -lib xil_defaultlib xil_defaultlib.TB_SEVENSEG

set NumericStdNoWarnings 1
set StdArithNoWarnings 1

do {TB_SEVENSEG_wave.do}

view wave
view structure
view signals

do {TB_SEVENSEG.udo}

run 1000ns
