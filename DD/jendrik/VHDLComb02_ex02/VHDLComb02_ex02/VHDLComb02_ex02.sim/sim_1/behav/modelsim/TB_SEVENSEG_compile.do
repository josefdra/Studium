######################################################################
#
# File name : TB_SEVENSEG_compile.do
# Created on: Sat Nov 04 13:10:37 CET 2023
#
# Auto generated by Vivado for 'behavioral' simulation
#
######################################################################
vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xil_defaultlib

vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib

vcom -64 -93 -work xil_defaultlib  \
"../../../../VHDLComb02_ex02.srcs/sources_1/imports/hdl/sevenseg.vhd" \
"../../../../VHDLComb02_ex02.srcs/sim_1/imports/hdl/tb_sevenseg.vhd" \


