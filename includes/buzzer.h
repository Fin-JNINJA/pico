#pragma once

#define BUZZER_PIN      17      // Pin 22 (GPIO 17)

#define E3              165
#define F3              175
#define G3              196     
#define A3              220    
#define B3              247    
#define C4              262 

unsigned int pwm_set_freq_duty(

    unsigned int slice,

    unsigned int channel,

    unsigned int frequency,

    double duty
);

void buzzer_init();

void buzzer_disable();

void buzzer_enable(unsigned int frequency);

void buzzer_signal(int code);

void playExitSong();