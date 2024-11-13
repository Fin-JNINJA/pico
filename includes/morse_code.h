#pragma once

#define BUTTON_PIN 16

int decoder(int range);

void setup_rgb();

char* checkButton();

void checkTimeout();