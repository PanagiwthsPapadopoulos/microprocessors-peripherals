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
#include <stdbool.h>
#include "touch.h"
#include "dht11.h"

#define SENSOR_PIN PB_3
#define TOUCH_SENSOR_PIN PB_9
#define CRITICAL_HUMIDITY 80
#define CRITICAL_TEMPERATURE 35
#define BUFF_SIZE 64
#define PASSWORD "a"
#define MENU_TEXT "\r\r\n======= MENU =======\r\n" \
                  "a)Increase read and print frequency by 1s (min 2s)\r\n" \
                  "b)Reduce read and print frequency by 1s (max 10s)\r\n" \
                  "c)Print Temperature/Humidity/Both.\r\n" \
                  "d)Print system status and last values.\r\n"
						

void calculate_sensor_frequency(int change);
void print_status(bool full_status);
bool check_password();
bool check_AEM();
bool check_option();
void print_sensor_data();
void change_read_sensor_status();


char AEM[6];
Queue rx_queue;
uint8_t buff_index;
char buffer[BUFF_SIZE];

int button_presses = 0;
uint8_t mode = 0;		// 0: normal, 1: alert
int mode_changes = 0;
int sensor_frequency = 2;

volatile uint8_t humidity;
volatile uint8_t temperature;
char dht_print_buffer[44];

uint8_t panic_counter = 0;
bool panic_reset = false;

//volatile uint8_t normal_counter = 0;
//volatile bool flash_led = false; // If declared here it resets immediately
//bool led_status = false;

typedef enum {
    STATE_PASSWORD,
    STATE_AEM,
    STATE_MENU
} InputState;

typedef enum {
	TEMPERATURE,
	HUMIDITY,
	BOTH
} SensorReadStatus;

SensorReadStatus current_sensor_read_status = TEMPERATURE;
InputState current_state = STATE_PASSWORD;
bool input_ready = false;
bool input_validated = false;

void uart_rx_isr(uint8_t rx) {
    if (rx <= 0x7F) {
        queue_enqueue(&rx_queue, rx);
    }
}

void prompt_for_state(InputState state) {
    buff_index = 0;
    memset(buffer, 0, BUFF_SIZE);

    switch (state) {
        case STATE_PASSWORD:
            uart_print("Enter password: ");
            break;
        case STATE_AEM:
            uart_print("Enter AEM: ");
            break;
        case STATE_MENU:
            uart_print(MENU_TEXT);
            uart_print("Select an option: \r\n");
            break;
    }
}

void handle_input(void) {
    uint8_t rx;
    while (queue_dequeue(&rx_queue, &rx)) {
        if (rx == 0x7F) {
					if (buff_index > 0) {
						buff_index--;
						buffer[buff_index] = '\0'; 
						uart_tx(rx);
					}
				} else if (rx == '\r') {
            uart_print("\r\n");
            input_ready = true;
            return;
        } else if (buff_index < BUFF_SIZE - 1) {
            buffer[buff_index++] = (char)rx;
            uart_tx(rx);
        }
    }
}

bool check_password() {
		if(strcmp(buffer, "clear")==0){
			uart_print("\033[2J\033[H");
			return false;
		}
		
    bool ok = (strcmp(buffer, PASSWORD) == 0);
    uart_print(ok ? "Correct password.\r\n" : "Wrong password.\r\n");
    memset(buffer, 0, BUFF_SIZE);
    buff_index = 0;
    return ok;
}

bool check_AEM() {
    if(strcmp(buffer, "clear") == 0) {
        uart_print("\033[2J\033[H");
        return false;
    }

    size_t len = strlen(buffer);

    // check length
    if (len < 2 || len > 5) {
        uart_print("Invalid AEM length. Must be 2 to 5 digits.\r\n");
        memset(buffer, 0, BUFF_SIZE);
        buff_index = 0;
        return false;
    }

    // check for digits
    for (size_t i = 0; i < len; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            uart_print("Invalid AEM. Only digits 0-9 allowed.\r\n");
            memset(buffer, 0, BUFF_SIZE);
            buff_index = 0;
            return false;
        }
    }

    // store AEM
    strncpy(AEM, buffer, sizeof(AEM) - 1);
    AEM[sizeof(AEM) - 1] = '\0';

    uart_print("AEM accepted.\r\n");
    memset(buffer, 0, BUFF_SIZE);
    buff_index = 0;
    return true;
}


bool check_option() {
	
	if(strcmp(buffer, "status")==0){
		print_status(true);
		return true;
	}
	
	if(strcmp(buffer, "clear")==0){
		uart_print("\033[2J\033[H");
		return true;
	}
	

	if (buff_index != 1 || buffer[0] < 'a' || buffer[0] > 'd') {
			uart_print("Invalid selection (a-d).\r\n\r\n");
			return false;
	}
	
	
	char opt = buffer[0];
	memset(buffer, 0, BUFF_SIZE);
	buff_index = 0;

	switch (opt) {
			case 'a': calculate_sensor_frequency(-1); break;
			case 'b': calculate_sensor_frequency(1); break;
			case 'c': change_read_sensor_status(); break;
			case 'd': print_status(false); break;
	}
	return true;
}

void change_read_sensor_status() {
	const char* labels[] = { "Temperature", "Humidity", "Both"};
	char print_option[33];
	
	// Change to temp/humidity/both
	if (current_sensor_read_status == BOTH)
		current_sensor_read_status = TEMPERATURE;
	else
		current_sensor_read_status++;
	
	// Print sensor's new reading status
	sprintf(print_option, "\r\nChanged to reading %s\r\n", labels[current_sensor_read_status]);
	uart_print(print_option);
}

void calculate_sensor_frequency(int change) {
	if (button_presses == 3) {
			int len = strlen(AEM), count = 0;
			change = 0;
			for (int i = len - 1; i >= 0 && count < 2; i--) {
					if (AEM[i] >= '0' && AEM[i] <= '9') {
							change += AEM[i] - '0';
							count++;
					}
			}
			sensor_frequency = change;
			button_presses = 0;
	} else {
			sensor_frequency += change;
	}

	if (sensor_frequency > 10) sensor_frequency = 10;
	if (sensor_frequency < 2) sensor_frequency = 2;

	char str[50], buffer[10];
	strcpy(str, "Changed sensor reading period to ");
	sprintf(buffer, "%d", sensor_frequency);
	strcat(str, buffer);
	strcat(str, " seconds\r\n");
	uart_print(str);
}

void button_callback() {
	if (current_state == STATE_MENU) {
		char str[100], buffer[10];
		strcpy(str, "\r\nMode selected: ");

		if (mode == 0) {
			mode = 1;
			strcat(str, "alert (MODE B)");
		} else {
			mode = 0;
				strcat(str, "normal (MODE A)");
		}

		mode_changes++;
		button_presses++;
		strcat(str, ", Button presses: ");
		sprintf(buffer, "%d\r\n", button_presses);
		strcat(str, buffer);
		uart_print(str);

		if (button_presses == 3) {
			calculate_sensor_frequency(0);
			button_presses = 0;
		}
	}
}


void timer_isr(void) {
	static uint8_t timer = 0;
	static uint8_t normal_counter = 0;
	static bool flash_led = false; // For some reason if declared globally (volatile bool flash_led) it resets when reaching led_blinking temp/hum
	static bool led_status = false;
	
	if(current_state == STATE_MENU) {
			if(timer >= sensor_frequency) { 
				timer = 0;
				
				// Read DHT data
				if(DHT11_read_data(SENSOR_PIN)) {
					temperature = DHT11_get_temperature();
					humidity = DHT11_get_humidity();
					
					switch(current_sensor_read_status) {
						case TEMPERATURE:
							sprintf(dht_print_buffer, "Temperature: %d*C\r\n", temperature);
							break;
						case HUMIDITY:
							sprintf(dht_print_buffer, "Humidity: %d%%\r\n", humidity);
							break;
						case BOTH:
							sprintf(dht_print_buffer, "Temperature: %d*C, Humidity: %d%%\r\n", temperature, humidity);
							break;
					}
					uart_print(dht_print_buffer);
					
					// check variables for panic reset
					if (temperature >= CRITICAL_TEMPERATURE || humidity >= CRITICAL_HUMIDITY) {
						if (++panic_counter == 3) {
							uart_print("Critical readings! Resetting!\r\n\r\n\r\n");
							NVIC_SystemReset();
						}
					}
					
					// check variables for led
					if (mode == 0) {
						flash_led = false;
						leds_set(0,0,0);
					} else {
						//abnormal reading
						if ((temperature >= 30 || humidity >= 49 )) {
							// If  in normal mode, don't flash led
								if (!flash_led) {
									uart_print("Detected abnormal readings! Enabling LED...\r\n");
									flash_led = true;
									normal_counter = 0;
								}
							//normal reading
						} else {
							if (++normal_counter == 5 && flash_led) {
								normal_counter = 0;
								uart_print("System returned to normal settings. Disabling LED...\r\n");
								flash_led = false;
								leds_set(0,0,0);
							}
						}
					}
					
				} else {
					uart_print("Checksum error! Data invalid.\r\n");
				}
			}		
			timer++;
	}
	
	if(flash_led){
		led_status = !led_status;
		leds_set(led_status, 0,0);
	}

}

void print_status(bool full_status) {
    char final_str[200], mode_str[10];

    if (full_status) {
        strcpy(final_str, "\r\nSYSTEM STATUS\r\nCurrent Mode: ");
        if (mode == 0)
            strcat(final_str, "normal (Mode A)");
        else
            strcat(final_str, "alert (Mode B)");

        strcat(final_str, "\r\nLast Temperature/Humidity readings: ");
        strcat(final_str, dht_print_buffer);
        strcat(final_str, "\r\nNumber of profile changes: ");
        sprintf(mode_str, "%d\r\n", mode_changes);
        strcat(final_str, mode_str);
        uart_print(final_str);
    } else {
        uart_print("\r\nSystem Status: ");
        switch (current_sensor_read_status) {
            case TEMPERATURE:
                sprintf(final_str, "Reading Temperature\r\nTemperature: %d*C\r\n", temperature);
                break;
            case HUMIDITY:
                sprintf(final_str, "Reading Humidity\r\nHumidity: %d%%\r\n", humidity);
                break;
            case BOTH:
                sprintf(final_str, "Reading Temperature & Humidity\r\nTemperature: %d*C, Humidity: %d%%\r\n", temperature, humidity);
                break;
        }
        uart_print(final_str);
    }
}




int main() {
	
	
	queue_init(&rx_queue, 64);
	
	// initiate DHT
	DHT11_init();
	
	// Initiate timer
	timer_init(1000000); // 1sec
	timer_set_callback(timer_isr);
	timer_enable();

	// Initiate uart
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr);
	uart_enable();
	
	// Initiate leds
	leds_init();
	 
	// initiate touch button
	initialize_touch(TOUCH_SENSOR_PIN);
	//Pin btn_pin = initialize_button();
	gpio_set_callback(TOUCH_SENSOR_PIN, button_callback);
	
	
	NVIC_SetPriority(EXTI9_5_IRQn, 1);
	NVIC_SetPriority(EXTI15_10_IRQn, 1);
	NVIC_SetPriority(USART2_IRQn, 2);

	__enable_irq();
	
	prompt_for_state(current_state);
	uart_print("\r\r\n");
	
	while (1) {
		handle_input();

		if (input_ready) {
			input_ready = false;
			switch (current_state) {
				case STATE_PASSWORD:
					input_validated = check_password();
					break;
				case STATE_AEM:
					input_validated = check_AEM();
					break;
				case STATE_MENU:
					input_validated = check_option();
					break;
			}

			if (input_validated) {
				if (current_state != STATE_MENU)
						current_state++;
				prompt_for_state(current_state);
			} else {
					prompt_for_state(current_state);
			}
			
		} // if(input_ready) END
	} // while(1) END
	
}
