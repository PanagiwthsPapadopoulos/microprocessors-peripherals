.section .text,"x"

.global xor_checksum
	
	xor_checksum:
// r0 = word_size
// r1 = &word[0]
// r2 = &checksum
	PUSH{r4,r5,lr}
	MOV r3,#0			// r3 <--- value of checksum
	
	MOV r4,#0			// r4 <--- i
	
	LOOP:
	CMP r4,r0		
	BGE EXIT			// If i>= word_size goto EXIT
	
	LDRB r5,[r1,r4]		// r5 <--- word[i] = *(&word + i)
	ADD r4,r4,#1		// i = i + 1
	EOR r3,r3,r5		// checksum = checksum^word[i]	
	B LOOP
	
	EXIT:
	STRB r3,[r2]
	POP {r4,r5,pc}
.end