# LED Memorization Game with IR Remote

A random sequence of LED blinks equal to the current level (1-10) is generated and the player must be able to 
press the buttons associated with each LED in the sequence in the right order. In order to level up, the player 
must play a number of games equal to the current level (1-10). If the player makes a mistake, the game ends. 
There is a start/reset button which starts a game if the game hasn't started, or erases the player's inputs so far
if the game has started.

This version of the game uses an IR Remote as input. Each IR Remote is different so the button mappings must be
changed accordingly.

### Components
- Arduino Uno
- LEDs x 3
- 220 Ohm Resistors x 3
- IR Remote (This project uses a 38kHz IR remote)
- IR Receiver (This project uses a TSOP38238 38kHz 950nm IR Receiver)

### Required Third-Party Libraries
- [Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote)
