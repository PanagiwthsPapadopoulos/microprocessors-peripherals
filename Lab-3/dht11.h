#ifndef DHT11_H
#define DHT11_H

#include "platform.h"
#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "delay.h"
#include "uart.h"
#include "platform.h"

#define VCC PB_5
#define GND PB_4

extern uint8_t dht_data[5];

/*! \brief Initializes VCC and GND pins
*/
void DHT11_init();

/*! \brief Stores data from sensor into data array, returns 0 if not parsed correctly
 */
int DHT11_read_data(Pin SENSOR_PIN);

/*! \brief Gets temperature from data array
*		\param SENSOR_PIN: The pin that the signal Pin of DHT11 is connected to
*/	
uint8_t DHT11_get_temperature();

/*! \brief Gets humidity from data array
*/
uint8_t DHT11_get_humidity();

#endif // DHT11_H
