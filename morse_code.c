//imports
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "includes/seven_segment.h"
#include "includes/buzzer.h"
#include "includes/LED.h"
#include "includes/potentiometer.h"
#include "includes/button.h"
#include "includes/morse_code.h"

//global variables
int pressed;
int notPressed = 0;
bool pressedInitial = false;
char morse[5] = "";
char word[5] = "";
bool keepActive = true;
int valid_inputs = 0;
int potentiometer_value = 0;
const char morseCode[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
const char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};

int main() {

	potentiometer_value = setup(); // initializes circuits and allows user to change potentiometer


	while (keepActive) { // main loop
		buzzer_disable();
		pressed = 0; // stores time while button is pressed in ms

		while (getButtonPress()) { // starts while loop when main button is pressed
			notPressed = 0; // stores time while button is not pressed in ms
			pressedInitial = true; // sets the fact the button has been pressed for the first time
			pressed++; // increments every ms while button is pressed
			buzzer_signal(1);
			sleep_ms(1);
		}
		if (notPressed == 0 && pressedInitial) { // returns true if button has been pressed
			char* addition = checkButton(); // returns a symbol based on the time the button has been held for
			strcat(morse, addition); // concatenates the morse string with the output from checkbutton()
			printf("%s\n", morse);
		}
		notPressed++; // increments every ms while button is not being pressed
		checkTimeout(); // checks to see if the time between inputs is greater than the set timeout for the inputs to be decoded
		sleep_ms(1);

		if (valid_inputs >= 4) { // returns true if the amount of valid inputs equals or exceeds 4
			playExitSong(); //plays an exit song
			printf("Your word is:  %s\nWould you like to leave?\n",word);
		}
		while (valid_inputs >= 4) { // loop fires if the amount of valid inputs equals or exceeds 4
			if(getButtonPress()) { // returns true if the first button input is pressed
				valid_inputs = 0; // resets valid inputs for so the user can input more
				LED(1); // LED flashes green
				memset(word, 0, strlen(word)); //resets the value of word back to "" using the memory adress
			}
			if(getButtonPressSecond()) { // returns true if the second button input is pressed
				LED(3); // LED flashes red
				playExitApple();
				exit(1);
			}
		}
	}
}

char* checkButton() {
	char* temp;

	if(pressedInitial) { // prevents the other branches from being checked unless the button has been pressed at least once
		if (pressed < 250) { // checks to see if the time in ms that the button has been pressed for is less than 250 ms
			printf("Button short\n");
			temp = "."; // returns a . to be processed by the decoder
		}
		else if (pressed >= 250 && pressed <= 700) { // checks to see if the time is between 250ms and 700ms
			printf("Button long\n");
			temp = "-"; // returns a - to be processed by the decoder
		}
		else {
			temp = ","; // returns a , to be processed by the decoderwhich will fire an error code
		}
	}
	return temp;
}

void checkTimeout() {
	int range = 400 * potentiometer_value; // sets time based on potentiometer value set at beginning of program
	if (notPressed >= range && pressedInitial) { // returns true if the button hasnt been prressed for more than the range and button has been pressed at least once

		int index = decoder(range); // returns a value from 0 - 25 if decoder finds matches a morse string to a letter or returns -1 if no match is found
		if(index < 0) { // returns true if the value returned from decoder is a -1
			printf("8\n");
			LED(2); // led flashes red
			buzzer_signal(3); // buzzer plays error sound
			seven_segment_show(27); // segment shows 8
			// branch handles error
		}
		else {
			printf("%s\n", alphabet[index]);
			strcat(word,alphabet[index]); // concatinates the found letter to the total word
			LED(1); // led flashes green
			seven_segment_show(index + 1); // segment shows letter of the alphabet
			//branch handles response
		}
		memset(morse, 0, strlen(morse)); // resets the morse value to ""
		pressedInitial = false; // tells program that button hasnt been pressed at all for the next letter
		sleep_ms(400);
		seven_segment_off();
	}
}

int decoder(int range) {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) { // loops for every string in the morseCode array
		if(strcmp(morseCode[i], morse) == 0 && notPressed < (range + 1)) { // returns true if morsecode is == morse and the button hasnt been pressed for less than the potentiometer range in ms
			valid_inputs++; // increments the amount of valid inputs from the user
			return i; // returns the index of the morse code string found in the alphabet array
		}
	}
	return -1; // returns an error code
}

int setup() {

    bool temp = true;
    int temp_int = 0;
    timer_hw->dbgpause = 0;

	// turns on all components
	stdio_init_all();
	potentiometer_init();
	buzzer_init();
	button_init();
	seven_segment_init();
	seven_segment_show(27);
	sleep_ms(1000);
	seven_segment_off();
	setup_rgb();
	LED(0);

    printf("\n\n\n\n\nHello! Welcome to this morse code decoder:\nPlease set potentiometer level and press left button to continue or right button to set default (4000ms)\n");

    while(temp) { // while temp is true, loop
		int read = potentiometer_read(5,10); // returns clamped value of the potentiometer between 5 and 10

        if(temp_int != read) { // if potentiometer value and temp_int are different, then set temp_int to potentiometer
            temp_int = read;
            printf("Timeout is set to: %dms \n",temp_int * 400);
        }
        if(getButtonPress()) {
            temp = false;
        }
		if(getButtonPressSecond()) {
			temp = false;
			temp_int = 10;
			printf("Default selected (4 seconds)\n");
		}
    }
	return temp_int;
}





