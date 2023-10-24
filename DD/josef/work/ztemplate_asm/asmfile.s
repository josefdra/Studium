
.section .data
poutstring: 
.asciz "Hello World!\n"


.section .text

.global myasmfn
myasmfn:

add r0,r0,r1

bx lr

.global mymain
mymain:
	stmfd	sp!, {lr} @push on full descending stack

	ldr	r0, =poutstring  
	bl	print_uart0
	
  mov	r0, #1
	mov	r1, #2
	bl	myasmfn

	@ldmfd	sp!, {pc} @pop off full descending stack
  ldmfd	sp!, {lr} @pop off full descending stack
  bx lr
