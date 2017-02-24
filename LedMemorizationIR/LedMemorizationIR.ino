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
 * - Three IR buttons for the three LEDs
 * - One IR button for start/reset
 *    - Starts game if game hasn't started
 *    - Resets player's inputs for current sequence if game
 *      has started
 *      
 * Hex mappings are defined in the GameData.hpp file.
 */

#include <IRremote.h>
#include "GameData.hpp"

IRrecv ir(IR_PIN);
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
  ir.enableIRIn();
}

void loop() {
  // Gets the player's input
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
      // Sets/resets game values and waits for player to 
      // start
      level = 1;
      games = 0;
      if (buttonVal == RESET) gameState = 1; // Start
      break;

    case 1:
      // Sets/resets sequence values and generates a new 
      // sequence
      generateSequence();
      pressCount = 0;
      mistakeFound = false;
      gameState = 2; // Play
      break;

    case 2:
      if (pressCount < level && buttonVal == RESET) {
        // Resets player's inputs
        mistakeFound = false;
        pressCount = 0;
      }
      else if (pressCount < level && risingEdge)
        getInput(buttonVal);
        // Processes input when there's a rising edge
     
      else if (!mistakeFound && pressCount == level) {
        // Right sequence
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
