/*
 * Game functions
 */
#include "GameData.h"

#if defined ANALOG_INPUT
void getInput(unsigned short inputVal) {
  // Processes player input and determines whether player
  // is right or wrong
  byte button = 0;
  if (inputVal <= 1023 && inputVal >= 1020) {
    button = LED_1;
    blinkLed(LED_1, 120);
  }
  else if (inputVal <= 1005 && inputVal >= 990) {
    button = LED_2;
    blinkLed(LED_2, 120);
  }
  else if (inputVal <= 520 && inputVal >= 500) {
    button = LED_3;
    blinkLed(LED_3, 120);
  }
  else {
    return;
  }
  if (sequence[pressCount] != button) mistakeFound = true;
  pressCount++;
}

#elif defined IR_INPUT
void getInput(unsigned long inputVal) {
  // Processes player input and determines whether player
  // is right or wrong
  byte button = 0;
  switch (inputVal) {
    case LED_1_BUTTON:
      button = LED_1;
      blinkLed(LED_1, 120);
      break;
      
    case LED_2_BUTTON:
      button = LED_2;
      blinkLed(LED_2, 120);
      break;
      
    case LED_3_BUTTON:
      button = LED_3;
      blinkLed(LED_3, 120);
      break;

    case RESET:
      mistakeFound = false;
      pressCount = 0;
      return;
      
    default:
      return;
  }
  if (sequence[pressCount] != button) mistakeFound = true;
  pressCount++;
}
#endif

void blinkLed(byte led, unsigned short ms) {
  // LED blinks for ms amount of time
  digitalWrite(led, HIGH);
  delay(ms);
  digitalWrite(led, LOW);
  delay(ms);
}

void blinkLedAll(unsigned short ms) {
  // All LEDs blinks for ms amount of time
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
  delay(ms);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
  delay(ms);
}

void generateSequence() {
  // Generates a random sequence
  for (byte i = 0; i < level; i++) {
    sequence[i] = byte(random(LED_1, LED_3+1));
  }
  for (byte i = 0; i < level; i++) {
    blinkLed(sequence[i], 250);
  }
}

void blinkQuickSuccession(byte cycles) {
  // LEDs blink in succession with 50 ms delay for an
  // amount of cycles. 
  // WARNING: Only 0-255 cycles are allowed because of byte
  // type to save memory
  for (byte i = 0; i < cycles; i++) {
    blinkLed(LED_1, 50);
    blinkLed(LED_2, 50);
    blinkLed(LED_3, 50);
  }
}

void playerCorrect() {
  // Correct
  games++;
  pressCount = 0;
  // Notification: 5 cycles of quick succession blinks
  blinkQuickSuccession(5);
}

void playerLevelUp() {
  // Level up
  level++;
  games = 0;
  // Notification: blink all LEDs twice with a 150 ms delay
  delay(100);
  blinkLedAll(150);
  blinkLedAll(150);
}

void playerWin() {
  // Win
  // Notification: 10 cycles of quick succession blinks
  delay(50);
  blinkQuickSuccession(10);
}

void playerWrong() {
  // Wrong
  // Notification: all LEDs blink for half a second
  blinkLedAll(500);
}
