
#pragma once

#define POTENTIOMETER_PIN       26      // Pin 31 (GPIO 26, ADC 0)

void potentiometer_init();
unsigned int potentiometer_read_raw();
unsigned int potentiometer_read(int min, int max);
