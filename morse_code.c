/**
 * The given template is a guideline for your coursework only.
 * You are free to edit/create any functions and variables.
 * You can add extra C files if required.
*/


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "LED.c"
#include "buzzer.c"
#include "includes/seven_segment.h"

#define BUTTON_PIN			16	// Pin 21 (GPIO 16)

// declare global variables e.g., the time when the button is pressed 
int pressed;
int notPressed = 0;
int attempts = 0;
bool pressedInitial = false;
char morse[4] = "";
TaskCompletionSource<bool> IsSomethingLoading = new TaskCompletionSource<bool>();

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

char morseCode[26][4] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};


// --------------------------------------------------------------------
// declare the function definitions, e.g, decoder(...); and ther functions
// given the user input, you can decode if the input is a character
int decoder();

// check if the button press is a dot or a dash
char* checkButton();
void checkTimeout();

void checkProgram();

int main() {

	timer_hw->dbgpause = 0;
	stdio_init_all();

	printf("hello!!!!!!!\n");
	// Initialise the seven segment display.
	seven_segment_init();

	// Turn the seven segment display off when the program starts.
	seven_segment_off();

	// Initialise the button"s GPIO pin.
	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_down(BUTTON_PIN); // Pull the button pin towards ground (with an internal pull-down resistor).

	while (true) {
		
		pressed = 0;
		while (gpio_get(BUTTON_PIN)){
            // record how long the button is pressed
            // .....
			//printf("This line is a test\n");  // you can remove this line
			sleep_ms(1); // adjust the sleep_ms as required
			pressed++;
			notPressed = 0;
			pressedInitial = true;
		} 
        // check if the button press is a dot or a dash
		if (notPressed == 0 && pressedInitial) {
			char* addition = checkButton();
			strcat(morse, addition);
			printf("%s\n", morse);
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
			printf("press error\n");
		}
	}
	return temp;

}

void checkProgram() {
	
	IsSomethingLoading.SetResult(true);
	attempts = 0;
}
SomeData TheData;

public async Task<SomeData> checkProgram() {
   await IsSomethingLoading.Task;
   return TheData;
}

void checkTimeout() {
	if ((notPressed >= 400 && pressedInitial) || strlen(morse) == 4) {
		int index = decoder();
		if(index < 0) {
			printf("8\n");
			//error
		}else {
			attempts++;
			printf("%s\n", alphabet[index]);
			if(attempts == 4) {
				checkProgram();
			}
			//correct

		}
		memset(morse, 0, strlen(morse));
		pressedInitial = false;
	}
}

int decoder() {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) {
		printf("%d %d\n", strcmp(morseCode[i], morse), sizeof(morseCode) / 4);
		if(strcmp(morseCode[i], morse) == 0 && notPressed < 401) {
			return i;
		}
	}
	return -1;
}
