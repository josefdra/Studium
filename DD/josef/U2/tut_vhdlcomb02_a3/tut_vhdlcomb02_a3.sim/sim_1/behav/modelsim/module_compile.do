######################################################################
#
# File name : module_compile.do
# Created on: Mon Nov 20 20:35:38 CET 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xil_defaultlib

vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vcom -64 -93 -work xil_defaultlib  \
"../../../../tut_vhdlcomb02_a3.srcs/sources_1/imports/imports/SwapEx02Ex03/sevenseg.vhd" \
"../../../../tut_vhdlcomb02_a3.srcs/sources_1/imports/imports/hdl/module.vhd" \


