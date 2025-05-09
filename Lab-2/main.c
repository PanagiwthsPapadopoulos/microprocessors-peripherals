#include "platform.h"
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "queue.h"
#include "leds.h"
#include "delay.h"
#include "timer.h"
#include "gpio.h"

// === Constants ===
#define BUFF_SIZE 64
#define BUTTON_PIN P_SW

Queue rx_queue; // Queue for storing received characters

// === Global State ===
char input_buffer[BUFF_SIZE];
int input_length = 0;
int current_digit_index = 0;
volatile int blink_in_progress = 0;
volatile int led_locked = 0;
volatile int digit_ready = 0;
volatile int new_input_received = 0;
volatile int input_active = 0;
volatile int button_press_count = 0;

int buff_index = 0;  // for ongoing UART input
char input_staging[BUFF_SIZE]; // separate buffer for typing
int staging_index = 0;         // index into staging
int led_status = 0;

void handle_uart_input(void) {
    uint8_t rx_char;
    while (queue_dequeue(&rx_queue, &rx_char)) {
        if (rx_char == 0x7F) {
            if (staging_index  > 0) {
                staging_index --;
                uart_tx(rx_char);
            }
        } else if (rx_char == '\r') {
            input_staging[staging_index ] = '\0';
            new_input_received = 1;
            staging_index  = 0;
        } else if (staging_index < BUFF_SIZE - 1) {
            input_staging[staging_index++] = rx_char;
            uart_tx(rx_char);
        }
    }
}

// Interrupt Service Routine for UART receive
void uart_rx_isr(uint8_t rx) {
	// Check if the received character is a printable ASCII character
	if (rx >= 0x0 && rx <= 0x7F ) {
		// Store the received character
		queue_enqueue(&rx_queue, rx);
	}
}


void led_blink(void) {
    leds_set(1,1,1);
    delay_ms(200);
    leds_set(0,0,0);
    delay_ms(200);
}

void led_toggle(void) {
		led_status = !led_status;
    leds_set(led_status,led_status,led_status);
}






void button_callback(int num) {
    

		led_locked = !led_locked;
		button_press_count++;


    if (led_locked)
        uart_print("Interrupt: Button pressed. LED locked. Count = ");
    else
        uart_print("Interrupt: Button pressed. LED unlocked. Count = ");

		char str[12];
		sprintf(str, "%d", button_press_count);
		uart_print(str);
		uart_print("\r\n");
}


// === Digit Handler ===
void process_digit(char digit_char)
{
    int digit = digit_char - '0';
    char msg[32];
    sprintf(msg, "Digit %d -> ", digit);

    if (led_locked) {
        strcat(msg, "Skipped LED action\r\n");
        uart_print(msg);
        return;
    }

    if (digit % 2 == 0) {
        strcat(msg, "Blink LED\r\n");
        blink_in_progress = 1;
    } else {
        strcat(msg, "Toggle LED\r\n");
        led_toggle();
    }

    uart_print(msg);
}


void timer_callback(void){
	digit_ready = 1;
}


int main() {
		
	
	// Initialize the receive queue and UART
	queue_init(&rx_queue, 128);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr); // Set the UART receive callback function
	uart_enable(); // Enable UART module
	
	__enable_irq(); // Enable interrupts
	
	// Initiate leds
	leds_init();
	
	// Initialize timer
	timer_set_callback(timer_callback);
	timer_init(500000);  // 500ms
	timer_disable();
	
	// Initialize button
	gpio_set_mode(BUTTON_PIN, PullDown);                // Configure as input
	gpio_set_trigger(BUTTON_PIN, Rising);           // Interrupt on falling edge
	gpio_set_callback(BUTTON_PIN, button_callback);  // Register your callback
	
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_SetPriority(USART2_IRQn, 1);
	
	uart_print("\r\n");// Print newline
	
	while (1) {
    handle_uart_input();   // Checks and updates input_buffer, sets new_input_received

    if (new_input_received) {
			strcpy(input_buffer, input_staging);         // ?? copy typed data to analysis buffer
			input_length = strlen(input_buffer);
			current_digit_index = 0;
			blink_in_progress = 0;
			digit_ready = 0;
			new_input_received = 0;
			input_active = 1;

			uart_print("\r\nInput: ");
			uart_print(input_buffer);
			uart_print("\r\n");

			timer_enable();  // ?? restart processing
	}


    if (input_active && digit_ready && !blink_in_progress) {
        digit_ready = 0;

        if (current_digit_index < input_length) {
            char digit = input_buffer[current_digit_index++];
            if (digit >= '0' && digit <= '9') {
                process_digit(digit);
            } else if (digit == '-') {
                current_digit_index = 0; // loop
            }
        } else {
            input_active = 0;
            uart_print("End of sequence. Waiting for new number...\r\n");
        }
    }

    if (blink_in_progress && !led_locked) {
        led_blink();  // 200ms ON, 200ms OFF
        blink_in_progress = 0;
    }

	}
		
		
	
}
