# adventofcode-cpp-2015

These are my solutions to http://adventofcode.com

All solutions are written in C++.

[![Build Status](https://travis-ci.org/petertseng/adventofcode-cpp-2015.svg?branch=master)](https://travis-ci.org/petertseng/adventofcode-cpp-2015)

# Input

In general, all solutions can be invoked in both of the following ways:

* Without command-line arguments, takes input on standard input.
* With 1+ command-line arguments, reads input from the first, which must be the path to an input file.
  Arguments beyond the first are ignored.

Some may additionally support other ways:

* None yet.

## Language notes

### String splitting

Not psrticularly impressed with the abilities of splitting a string by a delimiter.
As I see it, the options here are:

* `find` and `substr`
* `getline`
* `scanf`
* Regular expressions

I could have written a utility function that does it, but for the most part I just intermingled parsing and solving.

### `pair`s

I appreciate that they exist at all, but typing out the type is a little inconvenient compared to something like `(int, int)`.
No pattern-matching either, as far as I can tell.
