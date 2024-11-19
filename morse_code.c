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
char word[4] = "";
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

	buzzer_init();

	while (keepActive) {
		pressed = 0;
		while (getButtonPress()) {
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
		strcat(word, checkTimeout());
		notPressed++;
		sleep_ms(1);
		if (word[3] != ""){
			
		}
	}


	//endProgram();


}

char* checkButton() {
	char* temp;
	if(pressedInitial) {
		if (pressed < 250) {
			printf("Button short\n");
			temp = ".";
			buzzer_signal(1);
		} else if (pressed >= 250 && pressed <= 700) {
			printf("Button long\n");
			temp = "-";
			buzzer_signal(2);
		} else {
			temp = ","; // should be blank instead of comma??? used to be comma
			buzzer_signal(3);
		}
	}
	return temp;
}

char* checkTimeout() {
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
			return alphabet[index];
			//correct
		}
		memset(morse, 0, strlen(morse));
		pressedInitial = false;
		sleep_ms(500);
		seven_segment_off();
	}
	return "";
}

int decoder(int range) {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) {
		if(strcmp(morseCode[i], morse) == 0 && notPressed < (range + 1)) {
			return i;
		}
	}
	return -1;
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

void buddy_holly(){
	buzzer_init();
	buzzer_enable(400);
	sleep_ms(100);
}