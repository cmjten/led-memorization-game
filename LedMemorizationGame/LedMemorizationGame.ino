/*
 * LED Memorization Game
 *
 * The goal of the game is to to repeat the randomly
 * generated sequence of LEDs. A player must play sequences
 * equal to the current level in order to level up, and once
 * all the sequences in the maximum level have been played, 
 * the player wins.
 *
 * Controls:
 * - Three push buttons for the three LEDs
 * - One isolated push button for start/reset
 *    - Starts game if game hasn't started
 *    - Resets player's inputs for current sequence if game
 *      has started
 */
#include "GameData.h"

#ifdef IR_INPUT
#include <IRremote.h>
IRrecv ir(IR_PIN);
#endif

byte sequence[MAX_LEVEL] = {0};
byte pressCount;
byte games;
byte level;
byte gameState;
boolean mistakeFound;
byte currentState = 0;
byte previousState = 0;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
#if defined ANALOG_INPUT
  pinMode(INPUT_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
#elif defined IR_INPUT
  ir.enableIRIn();
#endif
}

void loop() {
#if defined ANALOG_INPUT
  unsigned short inputVal = analogRead(INPUT_PIN);
  byte reset = digitalRead(RESET_PIN);
#elif defined IR_INPUT
  decode_results results;
  unsigned long inputVal;
  if (ir.decode(&results)) {
    inputVal = results.value;
    ir.resume();
  }
  else inputVal = 0;
  delay(100);
#endif
  
  // Creates a "clock" that only reads an input during a
  // rising edge (when a button is pressed the first time)
  if (inputVal > 0) currentState = 1;
  else currentState = 0;
  boolean risingEdge = (currentState != previousState &&
    currentState == 1);

  switch (gameState) {
    case GAME_STOPPED:
      // Waits for player to start
      level = 1;
      games = 0;
#if defined ANALOG_INPUT
      if (reset == HIGH) gameState = GENERATE; 
#elif defined IR_INPUT
      if (inputVal == RESET) gameState = GENERATE;
#endif
      break;

    case GENERATE:
      // Generates a new sequence
      generateSequence();
      pressCount = 0;
      mistakeFound = false;
      gameState = INPUT_START; 
      break;

    case INPUT_START:
      // Gets player input during a rising edge, or
      // resets the player's inputs
      if (pressCount < level && risingEdge)
        getInput(inputVal);

#ifdef ANALOG_INPUT  
// In the analog version, reset is not handled in getInput()
      else if (pressCount < level && reset == HIGH) {
        mistakeFound = false;
        pressCount = 0;
      }
#endif
      
      previousState = currentState;
      if (pressCount == level) gameState = INPUT_DONE; 
      break;
      
    case INPUT_DONE:
      // If no mistake was found, moves on to the next
      // sequence. Levels up/wins when certain conditions
      // are met. If a mistake was found, stops the game
      // right away.
      if (!mistakeFound) { // Right sequence
        playerCorrect();
        gameState = GENERATE; 

        if (games == level && level < MAX_LEVEL) 
          playerLevelUp(); // Level up 
        
        else if (games == level && level == MAX_LEVEL) {
          // Player wins after MAX_LEVEL
          playerWin();
          gameState = GAME_STOPPED;
        }
      }
      else if (mistakeFound) { // Wrong sequence
        playerWrong();
        gameState = GAME_STOPPED;
      }
      delay(1000);
      break;
  }
}
