#!/bin/sh
#arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o
#arm-none-eabi-gcc -c -mcpu=arm926ej-s -g mylib.c cfile.c 
#arm-none-eabi-ld -T linkerscript.ld cfile.o mylib.o startup.o -o run.x
#arm-none-eabi-objcopy -O binary run.x run.bin

#further interesting options
#-no-pie
#-fverbose-asm
#--specs=nosys.specs 

##with debug
arm-none-eabi-as -mcpu=arm926ej-s -ggdb startup.s -o startup.o 
arm-none-eabi-as -mcpu=arm926ej-s -ggdb asmfile.s -o asmfile.o
arm-none-eabi-gcc -mcpu=arm926ej-s -c -ggdb -fomit-frame-pointer -fno-stack-protector -z execstack -fno-asynchronous-unwind-tables -fno-dwarf2-cfi-asm -fno-exceptions mylib.c -o mylib.o 
arm-none-eabi-ld -T linkerscript.ld mylib.o asmfile.o startup.o -o run.x
arm-none-eabi-objcopy -O binary run.x run.bin

##without debug 
#arm-none-eabi-as -mcpu=arm926ej-s startup.s -o startup.o
#arm-none-eabi-as -mcpu=arm926ej-s asmfile.s -o asmfile.o
#arm-none-eabi-gcc -mcpu=arm926ej-s -c -fomit-frame-pointer -fno-stack-protector -z execstack -fno-asynchronous-unwind-tables -fno-dwarf2-cfi-asm -fno-exceptions mylib.c -o mylib.o  
#arm-none-eabi-ld -T linkerscript.ld mylib.o asmfile.o startup.o -o run.x
#arm-none-eabi-objcopy -O binary run.x run.bin
#arm-none-eabi-objdump -d -S -j.text -j.data -j .rodata -j.bss run.x > run.x.d
