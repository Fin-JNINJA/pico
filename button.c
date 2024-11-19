//#include <studio.h>
#include "pico/stdlib.h"
#include "includes/button.h"

void button_init() {
    gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_down(BUTTON_PIN);



    //for the exit button 
    
        gpio_init(BUTTON_PIN_SECOND);
        gpio_set_dir(BUTTON_PIN_SECOND, GPIO_IN);
        gpio_pull_down(BUTTON_PIN_SECOND);
    
}

bool getButtonPress() {
    return gpio_get(BUTTON_PIN);
}

bool getButtonPressSecond() {
    return gpio_get(BUTTON_PIN_SECOND);
}