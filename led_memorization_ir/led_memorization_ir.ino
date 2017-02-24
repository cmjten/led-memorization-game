#include <IRremote.h>

/*
LED Memorization Game

The game generates a random sequence of an amount of LED blinks
and the player must be able to press the buttons associated with
each LED in the right order. The game goes from levels 1 to 10 and
the number of LEDs in the sequence is equal to the current level. 
The amount of games the player must play in order to level up is
also equal to the current level.

This version of the game uses an IR Remote as input. The values
for the buttons must be changed according to the IR Remote's 
hexadecimal mappings.

In the code, "Game" refers to the whole game itself, while "Current
Game" refers to the current random sequence being played.

Third-Party Libraries:
- Arduino-IRremote: https://github.com/z3t0/Arduino-IRremote
*/

// IR Receiver variables
int irPin = 2;
IRrecv irRecv(irPin);
decode_results results;

// Stores the randonmly generated sequence and the player's
// button presses
int buttonPresses[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sequence[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// LED pins
// *****The pins must be next to each other*****
const int led1Pin = 5;
const int led2Pin = 6;
const int led3Pin = 7;

// Button mappings on the remote. 
// *****Change this to match your IR Remote's button mappings*****
const long led1Button = 0xFF30CF;
const long led2Button = 0xFF18E7;
const long led3Button = 0xFF7A85;
const long resetButton = 0xFF6897;

// Game variables
bool started = false;
int gamesInCurrentLevel = 0;
int level = 1;

// Current game variables
bool generated = false;
int buttonPressCount = 0;
long buttonPressed = 0;

// Switch state check
int currentRemoteState = 0;
int previousRemoteState = 0;

void setup() {
  // put your setup code here, to run once 
  // LEDs
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  irRecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Gets the value of the button pressed. If no button has been
  // pressed, the value is set to 0.
  if (irRecv.decode(&results)) {
    buttonPressed = results.value;
    irRecv.resume();
  }  
  else {
    buttonPressed = 0;
  }
  delay(100);
  
  // Switch state check
  if (buttonPressed != 0) { currentRemoteState = 1; }
  else { currentRemoteState = 0; }
  
  if (!started) {
    // Starts the game by pressing the reset button
    if (buttonPressed == resetButton) {
      started = true;
    }
  }
  
  else {
    // Game has started
    if (!generated) {
      // Generates a random LED sequence based on the player's
      // level and lights up the LEDs
      generateSequence();
    }
    
    else {
      // A sequence has been generated
      if (buttonPressCount < level) {
        // Record the user's button presses
        if (currentRemoteState != previousRemoteState) {
          // Only sends a signal when the program detects a
          // change in state, which prevents from sending
          // multiple signals when a button is held down
          processPlayerInput();
        }
        previousRemoteState = currentRemoteState;
      }  
      
      else {
        // After an amount of presses equal to the current level
        // has been recorded, the game determines whether the
        // player is right or wrong     
        if (playerIsCorrect()) {
          // Notify if the player is correct
          notifyPlayerCorrect();
          
          if (gamesInCurrentLevel == level) {
            // Level up when the player has played a number of
            // games equal to the current level.
            levelUp();
            
            if (level > 10) {
              // Notify if the player wins and reset the game
              notifyPlayerWon();
              resetGame();
            }
            
            else {
              // Notify if the player levels up
              notifyPlayerLevelUp();
            }
          }
          delay(1000);
        }

        else {
          // Notify if the player is wrong and reset the game 
          notifyPlayerWrong();
          resetGame();
        }
        // Reset the variables after the "Current Game" has
        // been played 
        resetCurrentGameVariables();
      }
    }
  }
}

void allLedBlink(int ms) {
  // All LEDs blink for the specified amount of time
  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, HIGH);
  digitalWrite(led3Pin, HIGH);
  delay(ms);
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
  delay(ms);
}

void generateSequence() {
  // Generates a random sequence of LEDs based on the player's
  // level and lights up the LEDs
  delay(500);
      
  for (int a = 0; a < level; a++) {
  // The randomly generated numbers correspond to the
  // pins to which the LEDs are connected
    sequence[a] = int(random(led1Pin, led3Pin+1));
  }
  generated = true;
      
  for (int b = 0; b < level; b++) {
    ledBlink(sequence[b], 250);
  }
}

void ledBlink(int led, int ms) {
  // The specified LED blinks for the specified amount of time
  digitalWrite(led, HIGH);
  delay(ms);
  digitalWrite(led, LOW);
  delay(ms);
}

void levelUp() {
  // Levels up
  level += 1;
  gamesInCurrentLevel = 0;
}

void notifyPlayerCorrect() {
  // Every LED blinks in succession with a 50 ms delay 
  // in between for a total of 5 times
  quickSuccessionBlink(5);
  gamesInCurrentLevel += 1;
}

void notifyPlayerLevelUp() {
  // All LEDs lighting up twice with a 150 ms delay
  // in between signifies leveling up
  delay(100);
  for (int c = 0; c < 2; c++) {
    allLedBlink(150);
  }
}

void notifyPlayerWon() {
  // When the player finishes the game, light up
  // the LEDs as they would after getting a sequence
  // right, but for a longer amount of time
  delay(50);
  quickSuccessionBlink(10);
}

void notifyPlayerWrong() {
  // If the player is incorrect, light up all LEDs for 
  // half a second.
  allLedBlink(500);
}

bool playerIsCorrect() {
  // Determines whether the player is correct by comparing the
  // arrays sequence and buttonPresses
  for (int d = 0; d < level + 1; d++) {
    if (sequence[d] != buttonPresses[d]) {
      return false;
    }
  }
  return true;
}

void processPlayerInput() {
  // Adds an LED to buttonSequence and lights up that LED 
  // based on the value of the analog input
  switch (buttonPressed) {
    case led1Button:
      // LED 1 Button
      ledBlink(led1Pin, 120);
      buttonPresses[buttonPressCount] = led1Pin;
      buttonPressCount += 1;
      break;
  
    case led2Button:
      // LED 2 Button
      ledBlink(led2Pin, 120);
      buttonPresses[buttonPressCount] = led2Pin;
      buttonPressCount += 1;
      break;
  
    case led3Button:
      // LED 3 Button
      ledBlink(led3Pin, 120);
      buttonPresses[buttonPressCount] = led3Pin;
      buttonPressCount += 1;
      break;
    
    case resetButton:
      // If a mistake was made, reset button erases the user's
      // inputs so far
      resetArray(buttonPresses);
      buttonPressCount = 0;
      break;
  }
}

void quickSuccessionBlink(int cycles) {
// Every LED blinks in succession with a 50 ms delay 
// in between for a total amount times specified in the 
// parameter cycles
  for (int f = 0; f < cycles; f++) {
    ledBlink(led1Pin, 50);
    ledBlink(led2Pin, 50);
    ledBlink(led3Pin, 50);
  }
}

void resetArray(int array[]) {
  // Resets the values of the arrays sequence and buttonPresses
  for (int e = 0; e < 10; e++) {
    array[e] = 0;
  }
}

void resetGame() {
  // Resets the whole game
  level = 1;
  gamesInCurrentLevel = 0;
  started = false;
}

void resetCurrentGameVariables() {
  // Resets the variables that store information about the 
  // current game
  buttonPressCount = 0;
  buttonPressed = 0;
  generated = false;
  resetArray(buttonPresses);
  resetArray(sequence);
}


