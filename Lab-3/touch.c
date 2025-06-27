#include "gpio.h"

#define BUTTON_PIN P_SW

void initialize_touch(Pin pin){
	gpio_set_mode(pin, PullDown);
	gpio_set_trigger(pin, Rising); 
}


char test_touch(Pin pin){
	if(!gpio_get(pin)){
		return '1';
	} else return '0';
}


Pin initialize_button(){
	gpio_set_mode(BUTTON_PIN, PullDown);            // Configure as input
	gpio_set_trigger(BUTTON_PIN, Rising);           // Interrupt on falling edge
	return BUTTON_PIN;
}