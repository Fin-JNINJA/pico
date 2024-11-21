#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"

//Define pin numbers
#define R 13 
#define G 12 
#define B 11 

#define BRIGHTNESS 50
#define MAX_COLOUR_VALUE 255
#define MAX_PWM_LEVEL 65535


void setup_rgb()
{
    // Tell the LED pins that the PWM is in charge of its value.
    gpio_set_function(R, GPIO_FUNC_PWM);
    gpio_set_function(G, GPIO_FUNC_PWM);
    gpio_set_function(B, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin, this is done for the other colors below
    uint slice_num = pwm_gpio_to_slice_num(R);
    // Get the defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(G);
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(B);
    pwm_init(slice_num, &config, true);
}

// Allow a colour value [0,255] and scale it up to uint16_t [0,65535].
// Then scale it by brightness (out of 100) and invert the value (LED low is ON).
void show_rgb(int r, int g, int b)
{
    pwm_set_gpio_level(R, ~(MAX_PWM_LEVEL * r / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(G, ~(MAX_PWM_LEVEL * g / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(B, ~(MAX_PWM_LEVEL * b / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
}

void* LED(void* num) {
    int val = *(int*)num;
    switch(val) {
        case 1:
            show_rgb(0,255,0);
            sleep_ms(20);
            LED(0);
            break;
        case 2:
            show_rgb(255,0,0);
            sleep_ms(20);
            LED(0);
            break;
        default:
            show_rgb(0,0,0);
            break;
    }
    return NULL;
}