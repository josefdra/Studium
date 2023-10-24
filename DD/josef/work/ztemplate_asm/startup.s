.section .data
endtext:
.asciz "emulator reaches end of your code ... press ctrl+a and then x to end emulator ...\n"

.section .text
.global _Reset
_Reset:
 LDR sp, =stack_top
 BL mymain

 LDR r0, =endtext
 BL print_uart0

 B .
 @SWI 0x11
 @SWI 0x0
