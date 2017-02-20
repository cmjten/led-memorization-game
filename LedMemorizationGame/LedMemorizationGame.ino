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
 * - Three push buttons for the three LEDs
 * - One isolated push button for start/reset
 *    - Start games if game hasn't started
 *    - Resets player's inputs for current sequence if game
 *      has started
 *
 * Game states:
 * 0 - Game hasn't started
 * 1 - Generate a new sequence
 * 2 - Play
 */
#include "GameData.hpp"
// Pins and maximum level are defined in GameData.hpp file

// Game variables
byte sequence[MAX_LEVEL] = {0};
byte pressCount = 0;
byte games;
byte level;
byte gameState = 0;
boolean mistakeFound = false;
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
      // Resets game values and waits for player to start
      level = 1;
      games = 0;
      if (resetVal == HIGH) gameState = 1; // Start
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
      if (pressCount < level && risingEdge)
        getInput(analogVal);
      // Processes input when there's a rising edge
      else if (pressCount < level && resetVal == HIGH) {
        // Resets player's inputs
        mistakeFound = false;
        pressCount = 0;
      }
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
