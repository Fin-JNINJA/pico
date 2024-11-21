#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define RED 13 
#define GREEN 12 
#define BLUE 11 


#define BRIGHTNESS 50
#define LOOP_SLEEP 10
#define MAX_COLOUR_VALUE 255
#define MAX_PWM_LEVEL 65535


void setup_rgb()
{
    // Tell the LED pins that the PWM is in charge of its value.
    gpio_set_function(RED, GPIO_FUNC_PWM);
    gpio_set_function(GREEN, GPIO_FUNC_PWM);
    gpio_set_function(BLUE, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin, this is done for the other colors below
    uint slice_num = pwm_gpio_to_slice_num(RED);
    // Get the defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(GREEN);
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(BLUE);
    pwm_init(slice_num, &config, true);
}

void show_rgb(int r, int g, int b)
{
    pwm_set_gpio_level(RED, ~(MAX_PWM_LEVEL * r / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(GREEN, ~(MAX_PWM_LEVEL * g / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
    pwm_set_gpio_level(BLUE, ~(MAX_PWM_LEVEL * b / MAX_COLOUR_VALUE * BRIGHTNESS / 100));
}

void LED(int num) {
    //led input validation colours
    switch(num) {
        case 1: //A valid input turns the LED green
            show_rgb(0,1,0);
            sleep_ms(20);
            LED(0);
            break;
        case 2://An invalid input turns the LED RED
            show_rgb(1,0,0);
            sleep_ms(20);
            LED(0);
            break;
        case 3://The exit LED stays on for longer
            show_rgb(1,0,0);
            sleep_ms(500);
            LED(0);
            break;
        default:// Is called to turn off the RGB
            show_rgb(0,0,0);
            break;
    }
    return;
}