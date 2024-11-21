#include "hardware/pwm.h"
#include "pico/stdlib.h"

//Define pin numbers
#define R 13 
#define G 12 
#define B 11 

void setup_rgb()
{
    // Tell the LED pins that the PWM is in charge of its value.
    gpio_set_function(R, GPIO_FUNC_PWM);
    gpio_set_function(G, GPIO_FUNC_PWM);
    gpio_set_function(B, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin, this is done for the other colors below
    uint slice_num = pwm_gpio_to_slice_num(R);
    // Get the defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range

    pwm_config config = pwm_get_default_config();
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(G);
    pwm_init(slice_num, &config, true);

    slice_num = pwm_gpio_to_slice_num(B);
    pwm_init(slice_num, &config, true);

}

void show_rgb(int r, int g)
{
    pwm_set_gpio_level(R, ~r*65535);
    pwm_set_gpio_level(G, ~g*65535);
    pwm_set_gpio_level(B, ~b*65535);
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
}