######################################################################
#
# File name : TB_MODULE_compile.do
# Created on: Mon Nov 20 19:48:59 CET 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xil_defaultlib

vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vcom -64 -93 -work xil_defaultlib  \
"../../../../VHDLComb02_ex03.srcs/sources_1/imports/hdl/module.vhd" \
"../../../../VHDLComb02_ex03.srcs/sources_1/imports/SwapEx02Ex03/sevenseg.vhd" \
"../../../../VHDLComb02_ex03.srcs/sim_1/imports/hdl/tb_module.vhd" \


