.section .text,"x"

.global fibonacci
	
	fibonacci:
// r0 = n
	PUSH {lr}		// Push Link register to stack

	B0:
	CMP r0,#0		
	BNE B1			// If r0 = n != 0, goto B1
	MOV r0,#0		// If n = 0 ---> r0=0, will be returned on function end
	POP {pc}		
	
	B1:
	CMP r0,#1
	BNE B2			// Else if n != 1, go to B2
	MOV r0,#1		// If n = 1 ---> r0=1, will be return on function end
	POP {pc}
	
	B2:
	PUSH {r4,r5}
	MOV r4,r0		// Save current n before recursion, to use it in fibonacci(n-2)
	SUB r0,r0,#1	// r0 <--- n = n - 1
	BL fibonacci	// fibonacci(n-1)
	MOV r5,r0		// move result of fibonacci(n-1) return value to r5
	MOV r0,r4		// Get current n from stack, store it in r0
	SUB r0,r0,#2	// ro <--- n = n - 2
	BL fibonacci	// fibonacci (n-2)
	ADD r0,r0,r5	// r0 <--- fibonacci(n-1)+fibonacci(n-2)
	POP{r4,r5,pc}
.end	