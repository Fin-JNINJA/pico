#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include "includes/buzzer.h"
#include "includes/LED.h"
#include "includes/morse_code.h"

int pressed;
int notPressed = 0;
bool pressedInitial = false;
char morse[5] = "";

char morseCode[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

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
			notPressed = 0;
			pressedInitial = true;
			pressed++;
			sleep_ms(1);
		}

		if (notPressed == 0 && pressedInitial) {
			char* addition = checkButton();
			strcat(morse, addition);
			//printf("%s\n", morse);
		}
		checkTimeout();
		notPressed++;
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
			printf("%s\n", alphabet[index]);
			
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
