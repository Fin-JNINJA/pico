#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include "includes/buzzer.h"
#include "includes/LED.h"
#include "includes/potentiometer.h"
#include "includes/button.h"
#include "includes/morse_code.h"

int pressed;
int notPressed = 0;
bool pressedInitial = false;
char morse[5] = "";
bool keepActive = true;

char morseCode[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

int main() {

	timer_hw->dbgpause = 0;

	stdio_init_all();

	button_init();

	seven_segment_init();

	potentiometer_init();

	setup_rgb();

	printf("hello!\n");

	seven_segment_off();

	while (keepActive) {
		pressed = 0;
		while (getButtonPress()){
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


	//endProgram();


}

char* checkButton() {
	char* temp;
	if(pressedInitial) {
		if (pressed < 250) {
		printf("Button short\n");
		temp = ".";
		} else if (pressed >= 250 && pressed <= 700) {
			printf("Button long\n");
			temp = "-";
		} else {
			temp = ",";
		}
	}
	return temp;
}

void checkTimeout() {
	int range = 40 * potentiometer_read(5,10);
	if (notPressed >= range && pressedInitial) {
		int index = decoder(range);
		if(index < 0) {
			printf("8\n");
			LED(2);
			seven_segment_show(27);
			//error
		}else {
			printf("%s\n", alphabet[index]);
			LED(1);
			seven_segment_show(index + 1);
			//correct
		}
		memset(morse, 0, strlen(morse));
		pressedInitial = false;
		sleep_ms(500);
		seven_segment_off();
	}
}

int decoder(int range) {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) {
		if(strcmp(morseCode[i], morse) == 0 && notPressed < (range + 1)) {
			return i;
		}
	}
	return -1;
}
