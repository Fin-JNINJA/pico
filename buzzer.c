#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "includes/buzzer.h"

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
}
// disable the buzzer pin to stop the sound
void buzzer_disable() {
    gpio_deinit(BUZZER_PIN);
}

void buzzer_enable(unsigned int frequency) {
    // Set the frequency and duty cycle for the specified pin to the specified frequency.
    pwm_set_freq_duty(
        pwm_gpio_to_slice_num(BUZZER_PIN),          // PWM Slice (comptued from pin)
        pwm_gpio_to_channel(BUZZER_PIN),            // PWM Channel (computed from pin)
        frequency,                                  // Frequency in Hz (as specified)
        0.1                                        // Duty cycle = 0.1 (max volume = 0.5)
    );

    pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN), true);
}

// --------------------------------------------------------------------

unsigned int pwm_set_freq_duty(
    unsigned int slice,
    unsigned int channel,
    unsigned int frequency,
    double duty
) {

    unsigned int clock = 125000000;
    
    unsigned int divider16 = ceil(clock / (frequency * (double) 4096));

    if (divider16 < 16) divider16 = 16;
    
    unsigned int wrap = (clock * 16 / divider16 / frequency) - 1;

    pwm_set_clkdiv_int_frac(slice, divider16 / 16, divider16 & 0xF);

    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, channel, wrap * duty);

    return wrap;

}

void buzzer_signal(int code){
	buzzer_init();
	switch (code){
		case 1:
			buzzer_enable(1047);
			sleep_ms(100);
			break;
		case 2:
			buzzer_enable(1047);
			sleep_ms(250);
			break;
		case 3:
			buzzer_enable(200);
			sleep_ms(100);
			buzzer_disable();
			sleep_ms(50);
			buzzer_init();
			buzzer_enable(100);
			sleep_ms(100);
			break;
	}
	buzzer_disable();
}