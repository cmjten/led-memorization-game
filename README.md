# LED Memorization Game with Analog Inputs

A random sequence of LED blinks equal to the current level (1-10) is generated and the player must be able to 
press the buttons associated with each LED in the sequence in the right order. In order to level up, the player 
must play a number of games equal to the current level (1-10). If the player makes a mistake, the game ends. 
There is a start/reset button which starts a game if the game hasn't started, or the player's inputs so far
if the game has started.

This version of the game uses voltage dividers with resistors of different values connected to a single
analog pin in order to send signals of different values through one pin. 

###[Demo Video](https://vid.me/yvs7)

### Components
- Arduino Uno
- LEDs x 3
- 220 Ohm Resistors x 4
- 10k Ohm Resistors x 3
- Pushbuttons x 4

### Known Issues
- The voltage values for the first and second buttons are so close (5 V and ~4.88 V, respectively) that sometimes, when the first button is pressed, it doesn't deliver all 5 V but rather a value closer to 4.88 V. This results in the second LED being recorded in the player's sequence rather than the first LED. A different resistor value can be used for the second button, but the code must reflect this change as well.

### Images

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/led_memorization_analog_setup.png width=600/>

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/led_memorization_analog_breadboard.png width=600/>
