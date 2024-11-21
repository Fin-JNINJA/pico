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
char morseCode[26][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
char* alphabet[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};


int main() {

	setup(); // initializes circuits and allows user to change potentiometer


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
			strcat(morse, addition); // concatinates the morse string with the output from checkbutton()
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
				LED(2); // LED flashes red
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
			temp = ".";
		}
		
		else if (pressed >= 250 && pressed <= 700) {
			printf("Button long\n");
			temp = "-";
		}
		
		else {
			temp = ",";
			buzzer_signal(3);
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
			buzzer_signal(3);
			seven_segment_show(27);
			//handles error
		}
		
		else {
			printf("%s\n", alphabet[index]);
			strcat(word,alphabet[index]);
			LED(1);
			seven_segment_show(index + 1);
			//handles response
		}

		memset(morse, 0, strlen(morse));
		pressedInitial = false;
		sleep_ms(400);
		seven_segment_off();
	}

}

int decoder(int range) {
	for(int i = 0; i < sizeof(morseCode) / 4; i++) {
		if(strcmp(morseCode[i], morse) == 0 && notPressed < (range + 1)) {
			valid_inputs++;
			return i;
		}
	}
	return -1;
}

void setup() {

    bool temp = true;
    int temp_int = 0;

    timer_hw->dbgpause = 0;

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

    printf("Hello!\n\n\n\nPlease set potentiometer level and press any button to continue\n");

    while(temp) {
		int read = potentiometer_read(5,10);

        if(temp_int != read) {
            temp_int = read;
            printf("Timeout is set to: %dms \n",temp_int * 40);
        }

        if(getButtonPress() || getButtonPressSecond()) {
            temp = false;
        }
    }
}





