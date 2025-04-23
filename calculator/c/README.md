# Bitloop Calculator (C)

Requires GCC.

Compile
`gcc -o main main.c calc.c util.c`

Run
`./main`

Enter a bitstring, e.g. "1100". The program prints a few statistics, including the list of chains that comprise the corresponding powerset.

This version is unique in that it operates on individual bits. That is, the 0s and 1s entered by the user are treated as individual bits in the computer.

Works for values of _n_ <= 12.

