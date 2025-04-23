# Bitloop Calculator and Tessellator

A bitloop is a bit string whose ends are connected to form a loop.

Consequences of this simple concept are elaborated in detail [here](https://symbolfigures.io/bitloops.html).

[Bitloop Calculator and Tesselator](https://symbolfigures.io/bitloops/bitcalctess.html) is a tool to help interpret various properties of bitloops.

The front end utilizes AWS S3, Cloudfront, API Gateway and Lambda.

The calculator and tessellator each have their own Lambda functions, written in Python.

The Java version is used to produce properties of power sets. This is saved in .csv files, and those are aggregated into one .json file, which is bundled with and referenced by the python version.

The C version prints results to the console and is limited to _n_ <= 12. It's novel in that it operates on individual bits.
