
# AntGenerator

AntGenerator is a project that allows generation of arbitrarily long, pseudo-random data. This data is generated using the cellular automata, Langton's Ant.

 - [Langton's Ant](#Langtons-Ant)
 - [Utilizing Langton's Ant](#Utilizing-Langtons-Ant)git

## Langton's Ant

[The wiki](https://en.wikipedia.org/wiki/Langton%27s_ant)

If we have a grid of black and white squares, the ant(s) can be on any of the squares and facing towards an adjacent square.

1. The ant will invert the colour of the square it's currently stood on
2. The ant will move in the direction it's currently facing by one unit
3. If the square under the ant is white then the ant will rotate clockwise 90º and 90º anti-clockwise otherwise.

## Utilizing Langton's Ant

When describing the cellular automata we reference the boolean grid that the _Ants_ exist on, if we run the automata on this _grid_ for some amount, n, ticks with several ants. Then the chaotic noise generated on the _grid_ is a source of _"randomness"_. I'm unable to comment on how predictable the generated data is however.
