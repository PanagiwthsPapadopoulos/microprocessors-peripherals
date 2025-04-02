#include "platform.h"
#include "queue.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BUFF_SIZE 128 //read buffer length


void hash_init(const char *word, const uint8_t *digit_values, int* result);
void hash_interm(int num, uint8_t *result);
int fibonacci(int n);
void xor_checksum(uint8_t word_size, const char *word, uint8_t *checksum);


/*	// hash_init.s OLD START
void hash_init(uint8_t word_size, const char *word, const uint8_t *digit_values, int *result)
{
	int hash_val = word_size - 1; // Initial value of hash equal with length of string (\0 excluded)
	for (int i=0; i<word_size; i++)
	{
		if (word[i] >= 0x30 && word[i] <= 0x39)
		{
			hash_val += digit_values[word[i] - 48];							// 48 is the ASCII decimal value of '0'
		}
		else if (word[i] >= 0x41 && word[i] <= 0x5A)					// if Capital letter, add to hash (decimal ASCII value * 2)
		{
			hash_val += ((int)word[i] * 2 ); 	 
		}
		else if (word[i] >= 0x61 && word[i] <= 0x7A	)			// if Lowercase letter, add to hash (decimal ASCII value - 97)^2
		{
			hash_val += (int)pow( ((int)word[i] - 97 ), 2);			
		}
	}
	
	*result = hash_val;
}
*/ // hash_init.s OLD END

/*	// hash_init.s LOGIC START
void hash_init(const char *word, const uint8_t *digit_values, int *result)
{
	int hash_val = 0;
	uint8_t i=0;
	while(word[i] != '\0')
	{
		if (word[i] >= '0' && word[i] <= '9')
		{
			hash_val += digit_values[word[i] - 48];							// 48 is the ASCII decimal value of '0'
		}
		else if (word[i] >= 'A' && word[i] <= 'Z')					// if Capital letter, add to hash (decimal ASCII value * 2)
		{
			hash_val += ((int)word[i] * 2 ); 	 
		}
		else if (word[i] >= 'a' && word[i] <= 'z'	)			// if Lowercase letter, add to hash (decimal ASCII value - 97)^2
		{
			hash_val += (int)pow( ((int)word[i] - 97 ), 2);			
		}
		i++;
	}
	hash_val += i;
	*result = hash_val;
}
*/ // hash_init.s LOGIC END

/*	// hash_interm.s LOGIC START
void hash_interm(int num, uint8_t *result)
{
	int sum = 0;
	while (num > 0) {
			sum += num % 10;  // Get the last digit and add to sum
			num /= 10;        // Remove the last digit
	}
	sum %= 7;
	*result = sum;
}
*/	// hash_interm.s LOGIC END

/*		// xor_checksum.s LOGIC START
void xor_checksum(uint8_t word_size, const char *word, uint8_t *checksum)
{
	*checksum = 0;
	for (int i=0; i<word_size; i++)
	{
		*checksum ^= word[i];
	}
}
*/		// xor_checksum.s LOGIC END

Queue rx_queue; // Queue for storing received characters
// Interrupt Service Routine for UART receive
void uart_rx_isr(uint8_t rx) {
	// Check if the received character is a printable ASCII character
	if (rx >= 0x0 && rx <= 0x7F ) {
		// Store the received character
		queue_enqueue(&rx_queue, rx);
	}
}

int main() {
		
	// Variables to help with UART read
	uint8_t rx_char = 0;
	char buff[BUFF_SIZE]; // The UART read string will be stored here
	uint32_t buff_index;
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
	__enable_irq(); // Enable interrupts
	
	uart_print("\r\n");// Print newline
	
	while(1) {

		// Prompt the user to enter their full name
		uart_print("Enter your full name:");
		buff_index = 0; // Reset buffer index
		
		do {
			// Wait until a character is received in the queue
			while (!queue_dequeue(&rx_queue, &rx_char))
				__WFI(); // Wait for Interrupt

			if (rx_char == 0x7F) { // Handle backspace character
				if (buff_index > 0) {
					buff_index--; // Move buffer index back
					uart_tx(rx_char); // Send backspace character to erase on terminal
				}
			} else {
				// Store and echo the received character back
				buff[buff_index++] = (char)rx_char; // Store character in buffer
				uart_tx(rx_char); // Echo character back to terminal
			}
		} while (rx_char != '\r' && buff_index < BUFF_SIZE); // Continue until Enter key or buffer full
		
		// Replace the last character with null terminator to make it a valid C string
		buff[buff_index - 1] = '\0';
		uart_print("\r\n"); // Print newline
		
		// Check if buffer overflow occurred
		if (buff_index >= BUFF_SIZE) {
			uart_print("Stop trying to overflow my buffer! I resent that!\r\n");
		}
		
		
		
		//////////////////////////////////////////////////////////////////////////// First part
		printf("/////////////////////////////////////////////////////////////////////\n");
		printf("String input: %s\n", buff);
		// map of hash values of each number 0-9
		const uint8_t digit_values[10] = {5, 12, 7, 6, 4, 11, 6, 3, 10, 23};

		int init_result;
		hash_init(buff, digit_values, &init_result);	// buff_index = len(buff) + ('\0')
		
		printf("Initial Hash Value:  %d\n", init_result );
		printf("Buffer size: %d\n", buff_index);
		//////////////////////////////////////////////////////////////////////////// Second part
		uint8_t interm_result;
		hash_interm(init_result, &interm_result);
		
		printf("Intermediate Hash Value:  %d\n", interm_result );
		//////////////////////////////////////////////////////////////////////////// Third part
		int hash_final = fibonacci(interm_result);
		printf("Final Hash Value: %d\n", hash_final);
		//////////////////////////////////////////////////////////////////////////// Bonus part
		uint8_t checksum;
		xor_checksum(buff_index, buff, &checksum);
		
		printf("Xor checksum: %x\n", checksum);
	}
	return 0;
}
