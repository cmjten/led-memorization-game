/*
 * Pin assignments, constants, and function prototypes
 */
#ifndef LEDGAME
#define LEDGAME

#include <Arduino.h>
#define INPUT_PIN A0
#define RESET_PIN 2
// LED pins must be next to each other
#define LED_1 5
#define LED_2 6
#define LED_3 7
#define MAX_LEVEL 3

// Game variables
extern byte sequence[MAX_LEVEL];
extern byte pressCount;
extern byte games;
extern byte level;
extern boolean mistakeFound;

// Game functions
void blinkLed(byte led, unsigned short ms);
void blinkLedAll(unsigned short ms);
void blinkQuickSuccession(byte cycles);
void generateSequence();
void getInput(unsigned short analogVal);
void playerCorrect();
void playerLevelUp();
void playerWin();
void playerWrong();
#endif
