#include "includes/seven_segment.h"
#include <stdio.h>
#include "pico/stdlib.h"

/**
 * Notice that we have eight pins, so we can represent the pins that
 * should be turned on as 8-bit values. The uint8_t type specifically
 * refers to an unsigned 8-bit integer. When individual bits are used
 * to represent data it's known as a bitfield.
 * 
 * We can then use bitwise operations and bitmasks to interpret these bitfields.
 * 
 * Additionally, placing these in an array allows us to define values
 * from 0 - 4 such that we can get the bitfield for a given number by
 * simply indexing the array! 
 * 
 * (These bitfields are defined in the ordering of the ALL_SEGMENTS array
 * in the header file, i.e., from A in the most-significant bit to G, then
 * DP in the least-significant bit.)
 * 
 * If it's not clear to you how these values work, check the diagram on Labsheet 6 or 
 * page 2 of the datasheet linked at the top of seven_segment.h.
 */
// You will need to extend the array to include more numbers.

// ------------------------------------------------------------------

void seven_segment_init() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }
}

void seven_segment_off() {
    // This display is common-anode, which means we need to turn a GPIO
    // pin ON for it to appear OFF on the display. 
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}

void seven_segment_on() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], false);
    }
}
/*
 * @param number The number to display on the screen.
 * @return Zero if the number has been successfully displayed, otherwise a non-zero
 * value.
 */
unsigned void seven_segment_show(unsigned int number) {
    // the number is bigger than 4, the current program cannot display
    // you need to extend the program to display more numbers & alphabets

    /** You don't need to edit the following code within the for-loop **/
	//this loop goes through the segments one by one and checks if that segment
	// should be on or off then it turns it on or off
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int segmentBit = 1 << (7 - i);
        bool illuminateSegment = (segmentBit & values[number]); // != 0;
        gpio_put(
            ALL_SEGMENTS[i],
            !illuminateSegment
        );
}
}
