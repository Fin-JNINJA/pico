#pragma once

#define SEGMENT_A            7  // Pin 10 (GPIO 07)
#define SEGMENT_B            6  // Pin 09 (GPIO 06)
#define SEGMENT_C           20  // Pin 26 (GPIO 20)
#define SEGMENT_D           19  // Pin 25 (GPIO 19)
#define SEGMENT_E           18  // Pin 24 (GPIO 18)
#define SEGMENT_F            8  // Pin 11 (GPIO 08)
#define SEGMENT_G            9  // Pin 12 (GPIO 09)
#define ALL_SEGMENTS_COUNT  7

static const unsigned int ALL_SEGMENTS[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G};
uint8_t values[] = {
    0b00000000, // Nothing
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
    0b11011010, // Z
    0b11111110  // 8
};

void seven_segment_init() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }
}
void seven_segment_off() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}
void seven_segment_show(unsigned int number) {
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int segmentBit = 1 << (7 - i);
        bool illuminateSegment = (segmentBit & values[number]);
        gpio_put(
            ALL_SEGMENTS[i],
            !illuminateSegment
        );
    }
}