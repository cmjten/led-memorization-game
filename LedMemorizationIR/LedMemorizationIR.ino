/* 
 * LED Memorization Game
 *
 * The goal of the game is to be able to repeat the randomly
 * generates sequence of LEDs. A player must play games
 * equal to the current level in order to level up. The
 * player wins once the player finishes the level specified 
 * by MAX_LEVEL.
 *
 * Controls:
 * - Three IR buttons for the three LEDs
 * - One IR button for start/reset
 *    - Start games if game hasn't started
 *    - Resets player's inputs for current sequence if game
 *      has started
 *      
 * Hex mappings are defined in the GameData.hpp file.
 *
 * Game states:
 * 0 - Game hasn't started
 * 1 - Generate a new sequence
 * 2 - Play
 */

#include <IRremote.h>
#include "GameData.hpp"

IRrecv ir(IR_PIN);
byte sequence[MAX_LEVEL] = {0};
byte pressCount = 0;
byte games;
byte level;
byte gameState = 0;
boolean mistakeFound = false;
byte currentState = 0;
byte previousState = 0;

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  ir.enableIRIn();
}

void loop() {
  // Processes the player's input
  decode_results results;
  unsigned long buttonVal;
  if (ir.decode(&results)) {
    buttonVal = results.value;
    ir.resume();
  }
  else buttonVal = 0;
  delay(100);

  // High when a button is pressed
  if (buttonVal != 0) currentState = 1;
  else currentState = 0;

  // Checks for rising edge in input signal (low to high)
  boolean risingEdge = (currentState != previousState &&
    currentState == 1);

  switch (gameState) {
    case 0:
      // Resets game values and waits for player to start
      level = 1;
      games = 0;
      if (buttonVal == RESET) gameState = 1; // Start
      break;

    case 1:
      // Resets sequence values and generates a new sequence
      generateSequence();
      pressCount = 0;
      mistakeFound = false;
      gameState = 2; // Play
      break;

    case 2:
      // Player hasn't finished inputting a full sequence
      if (pressCount < level && buttonVal == RESET) {
        // Resets player's inputs
        mistakeFound = false;
        pressCount = 0;
      }
      else if (pressCount < level && risingEdge)
        getInput(buttonVal);
      // Processes input when there's a rising edge
     
      // Player has finished inputting a full sequence
      else if (!mistakeFound && pressCount == level) {
        playerCorrect();
        gameState = 1; // Next sequence

        if (games == level && level < MAX_LEVEL) {
          // Level up if player is at levels 1 to
          // MAX_LEVEL-1
          playerLevelUp();
        }
        else if (games == level && level == MAX_LEVEL) {
          // Player wins if all sequences in MAX_LEVEL
          // have been played
          playerWin();
          gameState = 0; // End game
        }
        delay(1000); // 1s delay between sequences
      }
      else if (mistakeFound && pressCount == level) {
        // Wrong sequence
        playerWrong();
        gameState = 0; // End game
      }
      previousState = currentState;
      break;
  }
}
