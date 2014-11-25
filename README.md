# Gaussian Elimination (Treppennormalform)

## Description

A simple command-line cross-platform implementation of the Gaussian elimination algorithm in C/C++. You can find some more details in the [wiki](https://github.com/LeonardKoenig/GaussianElimination/wiki) (German only).

## Download

Checkout the [releases](https://github.com/LeonardKoenig/GaussianElimination/releases) section on GitHub.

## License

GPL Version 3 from 29 June 2007. You can find the license text [here](http://www.gnu.org/copyleft/gpl.html). Please read it before using the program.

License Header: <br>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

## Building

No special changes needed to build on Windows or Linux. The program is completely C compatible.
Tested with:
 -  Windows: MS Visual Studio C/C++ Compiler, Cygwin, Pelles C Compiler
 -  Linux: Cross GCC, Linux GCC

## Sourcecode

main.cpp (main sourcecode)
 - all needed methods (including main method)
 - uses "main.h"

main.h (headerfile)
 - all used includes
 - all method-declarations
 - all used global variables
 - comments

## Algorithm

If you only want to use the algorithm in your own project, you can do this of course. So that you don't have to extract the pure algorithm from the source code, there is a special [LIB-Version](https://github.com/LeonardKoenig/GaussianElimination/releases/tag/LIB-v1.0.0-pre.1) of the algorithm. At the moment, the LIB-version is windows only. With the LIB-Version you can use the algorithm without additional dependencies in the final program. All description you can find in the Internet ("add a static library to program") or in our [wiki of the LIB-Version](https://github.com/LeonardKoenig/GaussianElimination/wiki/Algorithmus-in-eingenen-Projekten) (German only).
