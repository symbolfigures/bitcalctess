# Bitloop Calculator and Tessellator

A bitloop is a bit string whose ends are connected to form a loop.

Consequences of this simple concept are elaborated in detail [here](https://symbolfigures.io/bitloops.html).

[Bitloop Calculator and Tesselator](https://symbolfigures.io/bitloops/bitcalctess.html) is a tool to help interpret various properties of bitloops, and is written in Python.

The original [Java version](calculator/java) is faster and useful for producing the properties of power sets, which can take some time. The results are stored in JSON and referenced by the calculator, while the other properties are calculated on the spot.

The [C](calculator/c) and [C++](calculator/cpp) versions are not part of the tool, but are novel in that they operate on individual bits in memory. They may be run from the command line.