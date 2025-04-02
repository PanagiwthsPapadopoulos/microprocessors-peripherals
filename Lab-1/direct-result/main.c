#include "platform.h"
#include "queue.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BUFF_SIZE 128 //read buffer length


int hash_init(const char *word, const uint8_t *digit_values);
int hash_interm(int num);
int fibonacci(int n);
int xor_checksum(const char *word);

int main() {
	
		
		
		char buff[6] = "A9b3 ";
		int buff_index = 5;
		
		//////////////////////////////////////////////////////////////////////////// First part
		printf("/////////////////////////////////////////////////////////////////////\n");
		printf("String input: %s\n", buff);
		// map of hash values of each number 0-9
		const uint8_t digit_values[10] = {5, 12, 7, 6, 4, 11, 6, 3, 10, 23};

		int init_hash = hash_init(buff, digit_values);	// buff_index = len(buff) + ('\0')
		
		printf("Initial Hash Value:  %d\n", init_hash );
		printf("Buffer size: %d\n", buff_index);
		//////////////////////////////////////////////////////////////////////////// Second part
		uint8_t interm_result = hash_interm(init_hash);
		
		printf("Intermediate Hash Value:  %d\n", interm_result );
		//////////////////////////////////////////////////////////////////////////// Third part
		int hash_final = fibonacci(interm_result);
		printf("Final Hash Value: %d\n", hash_final);
		//////////////////////////////////////////////////////////////////////////// Bonus part
		int checksum = xor_checksum(buff);
		
		printf("Xor checksum: %x\n", checksum);
	
	return 0;
}
