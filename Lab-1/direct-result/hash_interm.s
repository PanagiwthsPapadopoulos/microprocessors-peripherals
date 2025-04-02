.section .text,"x"

.global hash_interm

	hash_interm:
// r0 = num
	PUSH {r4,lr}				// Save to stack r4-r6 and return address
	MOV r1, r0					// r1 <- number
	MOV r0, #0 					// r0 <- result
	
	MOV r3, #10		 			// store 10 to r3
	
	DIGIT_SUM:
	CMP r2, #0
	BEQ EXIT
	UDIV r2, r1, r3				// divide r1(num) by 10 and store in r2
	MLS r4, r2, r3, r1			// calculate digit and store in r4
	ADD r0, r0, r4				// update r0
	UDIV r1, r1, r3 			// update r1 with /10
	B DIGIT_SUM
	
	EXIT:
	MOV r3, #7	 				// store 7 to r3
	UDIV r2, r0, r3				// divide r0(result) by 7 and store in r2
	MLS r0, r2, r3, r0			// calculate digit and store in r0
		
	POP {r4,pc}
.end