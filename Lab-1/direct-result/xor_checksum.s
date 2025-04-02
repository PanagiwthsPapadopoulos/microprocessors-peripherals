.section .text,"x"

.global xor_checksum
	
	xor_checksum:
// r0 = &word[0]
	PUSH{lr}
	MOV r1, r0			// transfer word to r1
	MOV r0, #0			// initialise checksum to 0
	
	LOOP:
	CMP r2, #0	
	BEQ EXIT			// If i>= word_size goto EXIT
	
	LDRB r2,[r1], #1	// r2 <- word[i] = (&word++)
	EOR r0,r0,r2		// checksum = checksum^word[i]	
	B LOOP
	
	EXIT:
	POP {pc}
.end