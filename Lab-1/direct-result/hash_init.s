
.section .text,"x"

.global hash_init
	
	hash_init:
// r0 = &word[0]
// r1 = &digit_values[0]

	PUSH {r4-r6,lr}
	MOV r2,r0				// r2 <--- word[0] (word count)
	MOV r0,#0				// hash_value = 0
	
	LOOP:
	LDRB r3, [r2], #1		// get next char and increment word[i]
	CMP r3, #0
	BEQ EXIT
	ADD r0, r0, #1			// increment hash due to initialization of word length
	
	NUMBER_START:
	CMP r3, #48 			// check if number is lower than 48
	BLT LOOP				// if less skip number

	NUMBER_END:
	CMP r3, #57				// check if number is higher than Z
	BGT CAPITAL_START		// if more go to next condition
	SUB r3, r3, #48			// offset number to get actual value
	LDRB r4, [r1, r3]		// load digit value from r1 with offset of r3 ot r4
	ADD r0, r0, r4			// hash = hash + digit_values[word[i]]
	B LOOP
	
	CAPITAL_START:
	CMP r3, #65				// check if number is lower than A
	BLT LOOP				// if less skip number

	CAPITAL_END:
	CMP r3, #90				// check if number is higher than Z
	BGT LOWERCASE_START		// if more go to next condition
	LSL r4, r3, #1			// 2*char to r4
	ADD r0, r0, r4			// add to counter the r4 value
	B LOOP
	
	LOWERCASE_START:
	CMP r3, #97				// check if number is lower than a
	BLT LOOP				// if less skip number

	LOWERCASE_END:
	CMP r3, #122			// check if number is higher than a
	BGT EXIT				// check next condition
	SUB r4, r3, #97			// subtract 97 from ASCII num of letter
	MUL r4, r4, r4			// power of 2
	ADD r0, r0, r4			// add to counter the r4 value
	B LOOP
	
	
	EXIT:
	POP{r4-r6,pc}			// Retrieve from stack r4-r6 and set pc=return address 
.end