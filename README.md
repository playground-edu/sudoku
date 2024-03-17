# sudoku

## Description

A small C++ algorithm to solve the game of sudoku.

It is a example of brute force search and backtrack algorithms, suitable for classroom exercises.

## Algorithm

The algorithm employs a recursive approach to incrementally fill cells. During each iteration, it selects a cell with the minimum number of possible alternatives and backtracks as soon as it determines that a solution cannot be found.

Upon finding a solution, the algorithm displays it alongside an 'Undo' metric, indicating the number of backtrack operations performed.

A potential improvement of this algorithm could consist of finding a more efficient strategy to minimize the backtracking metric.

## Usage

```
./sudoku < input.txt
```

Input.txt is a text file consisting of 9 lines of 9 integers each in the range 1 to 9, separated by space. Integer 0 is used to mark empty cells that need to be filled to complete the puzzle.
See _Arto Inkala_'s [hardest.txt](./hardest.txt) sudoku as an example.

As shown in the example, you can enter any number of spaces to separate rows and columns, and may want to take advantage of that to represent 
the grid as _3x3_ blocks of 3x3 cells each, which is easier to visualize.

You also have the option to input negative integers to validate a Sudoku puzzle without revealing the full solution. In this case, the algorithm verifies that the negative values match those in the solution. Any discrepancies are indicated as 'X', without divulging additional information about the solution. Try it with [val-hardest.txt](./val-hardest.txt) for an example.

## Build

You need a c++17 compatible compiler.

`git clone` this repos and `make` will generate the `./sudoku` executable.

