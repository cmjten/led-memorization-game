/*
 * Pin assignments, constants, and function prototypes
 */
#ifndef LEDGAME
#define LEDGAME
#define ANALOG_INPUT // ANALOG_INPUT or IR_INPUT depending on
// which version of the game you want to play

#include <Arduino.h>

// Constants and prototypes exclusive to each version
#if defined ANALOG_INPUT // analog input version
#define INPUT_PIN A0
#define RESET_PIN 2
void getInput(unsigned short inputVal);

#elif defined IR_INPUT // ir input version
#define IR_PIN 2
#define LED_1_BUTTON 0xFF30CF
#define LED_2_BUTTON 0xFF18E7
#define LED_3_BUTTON 0xFF7A85
#define RESET 0xFF6897
void getInput(unsigned long inputVal);
#endif

// Game variables and constants
#define LED_1 5
#define LED_2 6
#define LED_3 7
#define MAX_LEVEL 3
#define GAME_STOPPED 0
#define GENERATE 1
#define INPUT_START 2
#define INPUT_DONE 3
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
void playerCorrect();
void playerLevelUp();
void playerWin();
void playerWrong();
#endif
