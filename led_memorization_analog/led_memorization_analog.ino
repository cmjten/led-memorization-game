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
*/

// Stores the randonmly generated sequence and the player's
// button presses
int sequence[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int buttonPresses[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Pin location and value of the game buttons
int analogInputPin = A0;
int analogInputValue = 0;

// Pin location and value of the reset button
int resetButtonPin = 2;
int resetButtonValue = 0;

// Game values
int buttonPressCount = 0;
int level = 0;
int gamesInCurrentLevel = 0;
bool started = false;
bool generated = false;

// Switch state check for analog input buttons
int previousSwitchState = 0;
int currentSwitchState = 0;

void setup() {
  // put your setup code here, to run once 
  // Reset button resets your button sequence
  pinMode(2, INPUT); 
  
  // LEDs
  for (int ledPin = 5; ledPin < 8; ledPin++) {
    pinMode(ledPin, OUTPUT);
  }
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
      delay(500);
      
      for (int a = 0; a <= level; a++) {
        // The randomly generated numbers correspond to the
        // pins to which the LEDs are connected
        sequence[a] = int(random(5, 8));
      }
      generated = true;
      
      for (int b = 0; b <= level; b++) {
        ledBlink(sequence[b], 250);
      }
    }
    
    else {
      // A sequence has been generated
      
      if (buttonPressCount < level + 1) {
        // Record the user's button presses
        
        if (currentSwitchState != previousSwitchState) { 
          // Only sends a signal when the program detects a
          // change in state, which prevents from sending
          // multiple signals when the button is held down
          
          if (analogInputValue <= 1023 &&
            analogInputValue >= 1020) {
            // First button is associated with LED connected to 
            // digital pin 5
            ledBlink(5, 120);
            buttonPresses[buttonPressCount] = 5;
            buttonPressCount += 1;
          }
          
          else if (analogInputValue >= 990 && 
            analogInputValue <= 1005) {
            // Second button is associated with LED connected to
            // digital pin 6
            ledBlink(6, 120);
            buttonPresses[buttonPressCount] = 6;
            buttonPressCount += 1;
          }
          
          else if (analogInputValue >= 500 &&
            analogInputValue <= 520) {
            // Third button is associated with LED connected to
            // digital pin 7
            ledBlink(7, 120);
            buttonPresses[buttonPressCount] = 7;
            buttonPressCount += 1;
          }
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
          // Every LED blinks in succession with a 50 ms delay 
          // in between for a total of 5 times
          quickSuccessionBlink(5);
          gamesInCurrentLevel += 1;
          
          if (gamesInCurrentLevel == level + 1) {
            // The number of games in the current level is equal 
            // to current level plus one. Level up once that
            // number of games is reached and reset the variable
            // gamesInCurrentLevel
            level += 1;
            gamesInCurrentLevel = 0;
            
            if (level > 9) {
              // When the player finishes the game, light up
              // the LEDs as they would after getting a sequence
              // right, but for a longer amount of time
              delay(50);
              quickSuccessionBlink(10);
              level = 0;
              gamesInCurrentLevel = 0;
              started = false;
            }
            
            else {
              // All LEDs lighting up twice with a 150 ms delay
              // in between signifies leveling up
              delay(100);
              for (int c = 0; c < 2; c++) {
                allLedBlink(150);
              }
            }
          }
          delay(1000);
        }
        
        else {
          // If the player is incorrect, light up all LEDs for 
          // half a second, reset the number of games and levels,
          // then stop the game.
          allLedBlink(500);
          level = 0;
          gamesInCurrentLevel = 0;
          started = false;
        }
        // Resets the values of most of the global variables to
        // the original value
        analogInputValue = 0;
        buttonPressCount = 0;
        generated = false;
        
        // Resets sequence and buttonPresses to their original
        // state
        resetArray(sequence);
        resetArray(buttonPresses);
      }
    }
  }
}

void allLedBlink(int ms) {
  // All LEDs blink for the specified amount of time
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(ms);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(ms);
}

void ledBlink(int led, int ms) {
  // The specified LED blinks for the specified amount of time
  digitalWrite(led, HIGH);
  delay(ms);
  digitalWrite(led, LOW);
  delay(ms);
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

void resetArray(int array[]) {
  // Resets the values of the arrays sequence and buttonPresses
  for (int e = 0; e < 10; e++) {
    array[e] = 0;
  }
}


