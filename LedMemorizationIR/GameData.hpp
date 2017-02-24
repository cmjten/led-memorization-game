/*
 * Pin assignments, constants, and function prototypes
 */
#ifndef LEDGAME
#define LEDGAME

#include <Arduino.h>
#define IR_PIN 2
// LED pins must be next to each other
#define LED_1 5
#define LED_2 6
#define LED_3 7
// Change the hex codes according to your IR remote mapping
#define LED_1_BUTTON 0xFF30CF
#define LED_2_BUTTON 0xFF18E7
#define LED_3_BUTTON 0xFF7A85
#define RESET 0xFF6897
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
void getInput(unsigned long buttonVal);
void playerCorrect();
void playerLevelUp();
void playerWin();
void playerWrong();
#endif
