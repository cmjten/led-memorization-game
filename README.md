<<<<<<< HEAD
# LED Memorization Game with Analog Inputs

The goal of the game is to be able to repeat the randomly generated sequence of LEDs. A player must play games
equal to the current level in order to level up. The player wins once the player finishes the maximum level specified. 

The circuit uses voltage dividers with different resistor values in parallel, all connected to a single analog input
pin.

###[Demo Video](https://vid.me/yvs7)

### Controls
- Three push buttons connected to a single analop input pin for game input
- Start/reset button connected to a digital input pin
  - Starts game if game hasn't started
  - Resets player's inputs in the current sequence if game has started
=======
# LED Memorization Game with IR Remote

A random sequence of LED blinks equal to the current level (1-10) is generated and the player must be able to 
press the buttons associated with each LED in the sequence in the right order. In order to level up, the player 
must play a number of games equal to the current level (1-10). If the player makes a mistake, the game ends. 
There is a start/reset button which starts a game if the game hasn't started, or erases the player's inputs so far
if the game has started.

This version of the game uses an IR Remote as input. Each IR Remote is different so the button mappings must be
changed accordingly.

###[Demo Video](https://vid.me/cejJ)
>>>>>>> ir_master

### Components
- Arduino Uno
- LEDs x 3
<<<<<<< HEAD
- 220 Ohm Resistors x 4
- 10k Ohm Resistors x 3
- Pushbuttons x 4

### Images

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/images/led_memorization_analog_setup.png width=600/>

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/images/led_memorization_analog_breadboard.png width=600/>
=======
- 220 Ohm Resistors x 3
- IR Remote (This project uses a 38kHz IR remote)
- IR Receiver (This project uses a TSOP38238 38kHz 950nm IR Receiver)

### Required Third-Party Libraries
- [Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote)

### Notes
- Any IR Receiver and IR Remote may be used, so long as they both operate at the same frequency. 
- The hexadecimal values for the buttons must be changed according to your IR Remote's mapping. The Arduino-IRremote
library contains an Arduino sketch for decoding the value of each button.

### Images

<img src=https://github.com/cmjten/led-memorization-game-ir/blob/master/led_memorization_game_ir_setup.png width=600/>

<img src=https://github.com/cmjten/led-memorization-game-ir/blob/master/led_memorization_ir_breadboard.png width=600/>
>>>>>>> ir_master
