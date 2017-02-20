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

### Components
- Arduino Uno
- LEDs x 3
- 220 Ohm Resistors x 4
- 10k Ohm Resistors x 3
- Pushbuttons x 4

### Images

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/images/led_memorization_analog_setup.png width=600/>

<img src=https://github.com/cmjten/led-memorization-game-analog/blob/master/images/led_memorization_analog_breadboard.png width=600/>
