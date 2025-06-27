#ifndef TOUCH_H
#define TOUCH_H

#include "gpio.h"

void initialize_touch(Pin pin);
int read_touch(Pin pin);
char test_touch(Pin pin);
Pin initialize_button();

#endif // TOUCH_H
