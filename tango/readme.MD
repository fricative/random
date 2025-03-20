A solver for the LinkedIn game tango

### To build:

run ```make clean && make```.

### To add a new game to be solved:

append the game to the end of ```game.txt```. Use "1" to represent sun, "2" to represent moon. "!" to represent not equal constraint.

append ```===========``` at the end of the new game you add.

### To solve the games

run ```./tango games.txt```