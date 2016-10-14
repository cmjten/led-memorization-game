/*
LED Memorization Game

The game generates a random sequence of an amount of LED blinks
and the player must be able to press the buttons associated with
each LED in the right order. The game goes from levels 0 to 9 and
the number of LEDs in the sequence is equal to the current level
plus one. The amount of games one must play in order to level up is
also equal to the current level plus one.

This version of the game uses voltage dividers with resistors of
different values and an analog input pin to send multiple input
value through one pin.

In the code, "Game" refers to the whole game itself, while "Current
Game" refers to the current random sequence being played.
*/

// Stores the randonmly generated sequence and the player's
// button presses
int buttonPresses[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sequence[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Pin location and value of the game and reset buttons
const int analogInputPin = A0;
int analogInputValue = 0;
const int resetButtonPin = 2;
int resetButtonValue = 0;

// Pin location of the LEDs
const int led1Pin = 5;
const int led2Pin = 6;
const int led3Pin = 7;

// Game variables
bool started = false;
int gamesInCurrentLevel = 0;
int level = 1;

// Current game variables
bool generated = false;
int buttonPressCount = 0;

// Switch state check for analog input buttons
int currentSwitchState = 0;
int previousSwitchState = 0;

void setup() {
  // put your setup code here, to run once 
  // Reset button resets your button sequence
  pinMode(resetButtonPin, INPUT); 
  
  // LEDs
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogInputValue = analogRead(analogInputPin);
  resetButtonValue = digitalRead(2);
  
  // Switch state check
  if (analogInputValue > 0) { currentSwitchState = 1; }
  else { currentSwitchState = 0; }
  
  if (!started) {
    // Starts the game by pressing the reset button
    if (resetButtonValue == HIGH) {
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
        if (currentSwitchState != previousSwitchState) { 
          // Only sends a signal when the program detects a
          // change in state, which prevents from sending
          // multiple signals when the button is held down
          processPlayerInput();
        }
        previousSwitchState = currentSwitchState;
          
        if (resetButtonValue == HIGH) {
          // If a mistake was made, reset button erases the user's
          // inputs so far
          resetArray(buttonPresses);
          buttonPressCount = 0;
        }
      }  
      
      else {
        // After an amount of presses equal to the current level
        // plus one has been recorded, the game determines
        // whether the player is right or wrong     
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
    sequence[a] = int(random(5, 8));
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

void quickSuccessionBlink(int cycles) {
// Every LED blinks in succession with a 50 ms delay 
// in between for a total amount times specified in the 
// parameter cycles
  for (int f = 0; f < cycles; f++) {
    for (int g = 5; g < 8; g++) {
      ledBlink(g, 50);
    }
  }
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
  if (analogInputValue <= 1023 && analogInputValue >= 1020) {
    // First button is associated with LED connected to 
    // digital pin 5
    ledBlink(led1Pin, 120);
    buttonPresses[buttonPressCount] = led1Pin;
    buttonPressCount += 1;
  }
          
  else if (analogInputValue >= 990 && analogInputValue <= 1005) {
    // Second button is associated with LED connected to
    // digital pin 6
    ledBlink(led2Pin, 120);
    buttonPresses[buttonPressCount] = led2Pin;
    buttonPressCount += 1;
  }
          
  else if (analogInputValue >= 500 && analogInputValue <= 520) {
    // Third button is associated with LED connected to
    // digital pin 7
    ledBlink(led3Pin, 120);
    buttonPresses[buttonPressCount] = led3Pin;
    buttonPressCount += 1;
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
  analogInputValue = 0;
  buttonPressCount = 0;
  generated = false;
  resetArray(buttonPresses);
  resetArray(sequence);
}


