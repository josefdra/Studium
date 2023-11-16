puts "start script ..."
puts "compile sources ..."

##Input argument
#set FALL $1

#Path to sources 
set SRCDIR    ../hdl

#create library 
vlib testlib
vmap testlib testlib

#compile sources
foreach FILE { 
              sevenseg.vhd
              tb_sevenseg.vhd
              } {
   vcom -93 -work testlib $SRCDIR/$FILE -source
}
puts "Compile finished ..."
puts "Start Simulation ..."

#view structure
#view signals
#view -undock wave

# Attention!!! xilinx testbench has the suffix vhd in the entity !!!
# vsim benötigt "libary.entity" des testbenchs als parameter !
#vsim -novopt -t ns testlib.testbenchentity
vsim -novopt -t ns testlib.tb_sevenseg

#do wave[subst $FALL].do
do wave.do

#run -all
#quit -sim

