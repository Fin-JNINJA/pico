#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include "includes/buzzer.h"
#include "includes/LED.h"

#define BUTTON_PIN 16

int pressed;
int notPressed = 0;
int attempts = 0;
bool pressedInitial = false;
char morse[5] = "";

uint8_t valueArray[] = {
   0b11101110, // A
   0b00111110, // B
   0b10011100, // C
   0b01111010, // D
   0b10011110, // E
   0b10001110, // F
   0b11110110, // G
   0b01101110, // H
   0b00001100, // I
   0b01111000, // J
   0b01101110, // K
   0b00011100, // L
   0b10101000, // M
   0b00101010, // N
   0b11111100, // O
   0b11001110, // P
   0b11100110, // Q
   0b00001010, // R
   0b10110110, // S
   0b00011110, // T
   0b01111100, // U
   0b00111000, // V
   0b01010100, // W
   0b01101110, // X
   0b01110110, // Y
   0b11011010  // Z
};

char morseCode[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

int decoder();

void setup_rgb();

char* checkButton();

void checkTimeout();

int main() {

	timer_hw->dbgpause = 0;
	stdio_init_all();

	printf("hello!!!!!!!\n");

	seven_segment_init();

	setup_rgb();

	seven_segment_off();

	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_down(BUTTON_PIN);

	while (true) {
		
		pressed = 0;
		while (gpio_get(BUTTON_PIN)){
			sleep_ms(1);
			pressed++;
			notPressed = 0;
			pressedInitial = true;
		} 

		if (notPressed == 0 && pressedInitial) {
			char* addition = checkButton();
			strcat(morse, addition);
			//printf("%s\n", morse);
		}
		notPressed++;
		checkTimeout();
		sleep_ms(1);
	}
}


char* checkButton() {
	int lower = 250;
	char* temp;
	if(pressedInitial) {
		if (pressed < lower) {
		printf("Button small\n");
		temp = ".";
		} else if (pressed >= lower && pressed < 700) {
			printf("Button large\n");
			temp = "-";
		} else {
			temp = ",";
			printf("8\n");
		}
	}
	return temp;
}

void checkTimeout() {
	if (notPressed >= 400 && pressedInitial) {
		int index = decoder();
		if(index < 0) {
			printf("8\n");
			LED(2);
			//error
		}else {
			attempts++;
			printf("%s\n", alphabet[index]);
			if(attempts == 4) {
				//checkProgram();
			}
			LED(1);
			//correct
		}
		memset(morse, 0, strlen(morse));
		pressedInitial = false;
	}
}

int decoder() {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) {
		//printf("%d %s %s\n", strcmp(morseCode[i], morse), morse, morseCode[i]);
		if(strcmp(morseCode[i], morse) == 0 && notPressed < 401) {
			return i;
		}
	}
	return -1;
}
