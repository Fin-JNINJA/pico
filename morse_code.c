//imports
#include <stdio.h>
#include <string.h>
#include <pthread.h>
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

	setup(); // initializes circuits 


	while (keepActive) { // main loop
		
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
			printf("%s\n", morse);
		}

		notPressed++;
		checkTimeout();
		sleep_ms(1);

		if (valid_inputs >= 4) {
			playExitSong();
			printf("Your word is:  %s\nWould you like to leave?\n",word);
		}

		while (valid_inputs >= 4) { // end loop
			if(getButtonPress()) {
				valid_inputs = 0;
				pthread_t LED_id;
				pthread_create(&LED_id, NULL, LED, 1);
				
				memset(word, 0, strlen(word));
			}

			if(getButtonPressSecond()) {
				LED(2);
				exit(1);
			}
		}
	}
}

char* checkButton() {
	char* temp;

	if(pressedInitial) {
		pthread_t buzzer_id;
		if (pressed < 250) {
			printf("Button short\n");
			temp = ".";
			pthread_create(&buzzer_id, NULL, buzzer_signal, 1);
		}
		
		else if (pressed >= 250 && pressed <= 700) {
			printf("Button long\n");
			temp = "-";
			pthread_create(&buzzer_id, NULL, buzzer_signal, 2);
		}
		
		else {
			temp = ",";
			pthread_create(&buzzer_id, NULL, buzzer_signal, 3);
		}
	}

	return temp;
}

void checkTimeout() {
	int range = 40 * potentiometer_read(5,10);
	if (notPressed >= range && pressedInitial) {

		int index = decoder(range);
		pthread_t LED_id;
		pthread_t seven_id;
		if(index < 0) {
			printf("8\n");
			pthread_create(&LED_id, NULL, LED, 2);
			pthread_create(&LED_id, NULL, display_screen, 27);
			//handles error
		}
		
		else {
			printf("%s\n", alphabet[index]);
			strcat(word,alphabet[index]);
			pthread_t LED_id;
			pthread_t seven_id;
			pthread_create(&LED_id, NULL, LED, 1);
			pthread_create(&LED_id, NULL, display_screen, index + 1);
			//handles response
		}

		memset(morse, 0, strlen(morse));
		pressedInitial = false;
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

	button_init();

	seven_segment_init();

	potentiometer_init();

	setup_rgb();

	seven_segment_off();

	buzzer_init();

    printf("Hello!\nplease set potentiometer level and press any button to continue\n");

    while(temp) {
		int read = potentiometer_read(5,10);

        if(!(temp_int > read - 1) && !(temp_int < read + 1)) {
            temp_int = read;
            printf("timeout is set to: %dms \n",temp_int * 40);
        }

        if(getButtonPress() || getButtonPressSecond()) {
            temp = false;
        }
    }
}





