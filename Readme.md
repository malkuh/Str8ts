
# Str8ts Solver

This is a Str8ts puzzle solver using bitmaps.
(Visit [str8ts.com] for more information about Str8ts.)
It will solve an average puzzle in under a millisecond on most computers.

### Installation

The program is easily compiled with
```sh
$ cd build
$ cmake ../
$ make
```
and can be used with
```sh
$ ./solver ../problems/example.txt
```
### Options

- *-v*: be verbose, some more messages.
- *-t*: activate tracing and produce final statistics.

### Input

An input textfile has to have the following format:

```
#f9.#..##
.........
..ai..#.6
1...#..##
#.......#
h#..#...2
..#..##..
..3...8..
##..#..g#
```

(Example is taken from str8ts.com 9. Feb. 2016)

That is '.' for an empty field, '#' for a black field, and
'123456789' and 'abcdefghi' for digits on white or black fields, respectively.
The line breaks are not necessary.


### Implementation Details

To find the correct solution a number of heuristics are implemented.
(Mostly inspired by [James Austin]'s Str8ts solver].)
If none of the heuristics work anymore, backtracking is used.

For the representation of possible digits in the puzzle bitmaps are used because most heuristics are then computed using a few bit operations.
E.g. removing a few numbers from the set of possible numbers in a field is as easy and fast as
```
field &= ~numbers;
```
Furthermore, look-up tables are used to find the number of possible digits or the highest or lowest possible digit, or just to test whether only one digit remains possible.


### Possible Extensions

It can be easily modified to find all possible solutions if the puzzle has no unique solution.

Heuristics could be added to make the solver faster for very difficult problems on which otherwise backtracking would have to be engaged.

Malte Kuhlmann


[str8ts.com]:http://www.str8ts.com
[James Austin]:https://github.com/jamesaustin/str8ts
