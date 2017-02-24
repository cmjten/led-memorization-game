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
#include "GameData.hpp"
// Pins and maximum level are defined in GameData.hpp file

// Game variables
byte sequence[MAX_LEVEL] = {0};
byte pressCount;
byte games;
byte level;
byte gameState;
boolean mistakeFound;
byte currentState = 0;
byte previousState = 0;

void setup() {
  pinMode(INPUT_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
}

void loop() {
  unsigned short analogVal = analogRead(INPUT_PIN);
  byte resetVal = digitalRead(RESET_PIN);

  // High when a button is pressed
  if (analogVal > 0) currentState = 1; // > 5 for noise
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
      if (resetVal == HIGH) gameState = 1; // Start
      break;

    case 1:
      // Sets/eesets sequence values and generates a new 
      // sequence
      generateSequence();
      pressCount = 0;
      mistakeFound = false;
      gameState = 2; // Play
      break;

    case 2:
      if (pressCount < level && risingEdge)
        getInput(analogVal);
        // Processes input when there's a rising edge
      else if (pressCount < level && resetVal == HIGH) {
        // Resets player's inputs
        mistakeFound = false;
        pressCount = 0;
      }
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
