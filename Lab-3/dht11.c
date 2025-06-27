#include "dht11.h"

uint8_t dht_data[5];

void DHT11_init() {
	gpio_set_mode(VCC, Output);
	gpio_set_mode(GND,Output);
	gpio_set(VCC, 1);
	gpio_set(GND, 0);
	delay_ms(1000);
}

uint8_t DHT11_get_temperature() {
	return dht_data[2];
}

uint8_t DHT11_get_humidity() {
	return dht_data[0];
}

int DHT11_read_data(Pin SENSOR_PIN) {
	uint8_t i, j;
	
	for (j = 0; j < 5; j++)
		dht_data[j] = 0;

	// STM sends signal
	gpio_set_mode(SENSOR_PIN, Output);
	gpio_set(SENSOR_PIN,0);
	delay_ms(18); // At least 18ms
	gpio_set(SENSOR_PIN,1);
	delay_us(20);
	gpio_set_mode(SENSOR_PIN, Input);

	// Wait for DHT11 response
	delay_us(40);
	if (gpio_get(SENSOR_PIN)) {
		uart_print("DHT not low\r\n");
		return 0; // Should be LOW
	}
	delay_us(80);
	if (!gpio_get(SENSOR_PIN)) {
		uart_print("DHT not high\r\n");
		return 0; // Should be HIGH
	}
	delay_us(80);

	// Read 5 bytes (40 bits)
	for (j = 0; j < 5; j++) {
		for (i = 0; i < 8; i++) {
			while (!gpio_get(SENSOR_PIN)); // Wait for LOW to HIGH
			delay_us(40);
			if (gpio_get(SENSOR_PIN))
				dht_data[j] |= (1 << (7-i));
			while (gpio_get(SENSOR_PIN)); // Wait for end of HIGH
		}
	}

	// Checksum
	uint8_t sum = dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3];
	if (sum != dht_data[4]) return 0;

	return 1;
}