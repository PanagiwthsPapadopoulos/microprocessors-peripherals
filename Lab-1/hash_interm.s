.section .text,"x"

.global hash_interm

	hash_interm:
// r0 = num
// r1 = &result
	PUSH {r4-r5,lr}				// Save to stack r4-r6 and return address
	MOV r4,#0					// r4 = sum = 0

	LOOP:
	CMP r0,#0		
	BLE EXIT					// if num<=0, stop looping

	MOV r3,#10
	UDIV r2,r0,r3				// r2 = r0/10
	MLS r5, r2, r3, r0  		// r5 = num - (r2 * 10) --> num % 10
	ADD r4,r4,r5				// sum = sum + num%10
	UDIV r0,r0,r3				// num = num / 10
	B LOOP



	EXIT:
	MOV r3,#7
	UDIV r5,r4,r3				//(int)r5=sum/7
	MLS r4, r5, r3, r4  		// r4 = sum - (r5 * 7) --> sum % 7

	STRB r4,[r1]				// *result = sum
	POP {r4-r5,pc}
.end