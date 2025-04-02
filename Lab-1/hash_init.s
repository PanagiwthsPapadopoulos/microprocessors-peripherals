
.section .text,"x"

.global hash_init
	
	hash_init:
// r0 = &word[0]
// r1 = &digit_values[0]
// r2 = &result
	PUSH {r4-r6,lr}
	MOV r5,#0				// r5 <--- i (word count)
	MOV r3,#0			// hash_value = 0

	LOOP:
	LDRB r4,[r0],#1			// r4 <--- word[i], then r0 points to next byte
	CMP r4,#0				// If word[i] = '\0'
	BEQ EXIT				// Branch to EXIT			
	ADD r5,r5,#1			// i += 1
	
	
	NUMBER_START:
	CMP r4,#48				// Compare word[i] to ASCII value 48 (first ASCII number value)
	BLT LOOP				// If ASCII value of word[i] is less than 48, then it isn't a valid character, check next char

	NUMBER_END:
	CMP r4,#57				// Similar to above (last ASCII number value)
	BGT CAPITAL_START		// If ASCII val of word[i] isn't a number, procceed check if capital

	NUMBER_MAPPING:
//CREATE INDEX
	SUB r6,r4,#48			// Offset for digit_values
//LOAD digital_values[]
	LDRB r6,[r1,r6]			// r6 = *(&digit_values + offset)
//ADD TO HASH
	ADD r3,r3,r6			// hash_value += digit_values[word[i]] (word[i]--> the actual number, not ASCII value)
	B LOOP
	
	
	CAPITAL_START:
	CMP r4,#65				// Compare word[i] to ASCII value 65 (first ASCII capital letter value)
	BLT LOOP				// If ASCII val is less than 65, then it isn't valid, check next char

	CAPITAL_END:
	CMP r4,#90				// Similar to above (last ASCII capital letter value)
	BGT LOWERCASE_START		// If ASCII val of word[i] isn't a capital letter, procceed to check if lowercase
	
	CAPITAL_HASH:
	LSL r6,r4,#1			// r6 = word[i] * 2 
	ADD r3,r3,r6			// hash_value += word[i] * 2
	B LOOP
	
	
	LOWERCASE_START:
	CMP r4,#97				// Compare word[i] to ASCII value 97 (first ASCII lowercase letter value)
	BLT LOOP				// If ASCII val is less than 97, then it isn't valid, check next char

	LOWERCASE_END:
	CMP r4,#122				// Similar to above (last ASCII capital letter value)
	BGT EXIT				// If ASCII val of word[i] isn't a lowercase letter, exit
	
	LOWERCASE_HASH:
	SUB r6,r4,#97			// r7 = word[i] - 97
	MUL r6,r6,r6			// r7 = (word[i]-97)^2
	ADD r3,r3,r6			// hash_value += (word[i]-97)^2
	B LOOP
	
	
	EXIT:
	ADD r3,r3,r5			// hash_value += i
	STR r3,[r2]				// *result = hash_value
	POP{r4-r6,pc}			// Retrieve from stack r4-r6 and set pc=return address 
.end