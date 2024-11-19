#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "includes/potentiometer.h"

#define POTENTIOMETER_MIN       2025
#define POTENTIOMETER_MAX       4050

void potentiometer_init() {
    adc_init();
    adc_gpio_init(POTENTIOMETER_PIN);
}

unsigned int potentiometer_read_raw() {

    adc_select_input(POTENTIOMETER_PIN - 26);

    return adc_read();
}

double map(
    double value,
    int original_min,
    int original_max,
    int min,
    int max
) {
    int original_range = original_max - original_min;
    int range = max - min;
    return (value - original_min) * range / original_range + min;
}

double clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

unsigned int potentiometer_read(int min, int max) {
    return map(clamp(potentiometer_read_raw(), POTENTIOMETER_MIN, POTENTIOMETER_MAX), POTENTIOMETER_MIN, POTENTIOMETER_MAX, min, max);
}