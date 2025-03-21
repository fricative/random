## To build and run

```make clean && make && ./queens games.txt```

## expected output
```
successfully solved:

11*233333
11123433*
51122*663
5511146*3
5*7111611
557*81111
557789*11
5555*9911
*55559111

successfully solved:

*11111111
12*111111
223313*11
22333331*
2553*5444
2*3333347
5533833*7
599*87777
99988*777
```

## To add more games

append your game to ```game.txt```. use 1 to N to represent different colors, do NOT use 0.

append a line of ```===========``` to the end of your new game

## Algorithm

Simple backtracking algorithm with a bit of heuristic of always picking the color with 
the least number of options. This helps to reduce the potential search space.  