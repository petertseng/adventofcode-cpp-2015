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

* 4 (Advent Coins): Pass the secret key in ARGV.
* 10 (Look and Say): Pass the seed sequence in ARGV.
* 11 (Passwords): Pass the initial password in ARGV.
* 20 (Factors): Pass the target number of gifts in ARGV.
* 21 (RPG): Pass the boss's HP, damage, and armour in ARGV, in that order.
* 22 (Wizard): Pass the boss's HP and damage in ARGV, in that order.
* 25 (Triangular): Pass the row and column number in ARGV, in that order.

## Language notes

### String splitting

Not psrticularly impressed with the abilities of splitting a string by a delimiter.
As I see it, the options here are:

* `find` and `substr`
* `getline`
* `scanf`
* Regular expressions

I could have written a utility function that does it, but for the most part I just intermingled parsing and solving.

### `pair`s and `tuple`s

I appreciate that they exist at all, but typing out the type is a little inconvenient compared to something like `(int, int)`.
No pattern-matching either, as far as I can tell.

### Regular expressions

I like them as much as the next person but they do increase the compilation time by a lot.
So for the sake of faster compilation I usually go without.

### Range-based `for`

They're nice, but when iterating over a string, do I really have to use `for (const char& c : s)`?
Why not just `for (char c : s)`?
The latter does compile; is there some performance reason not to do that?

### `variant`s (discriminated union, sum type)

Usable, but `visit` is a little unergonomic, specifically having to do that `overload` thing.

### Vectors

Really wish there were a more convenient way to print the contents of a vector instead of having to write out the `for` loop.

### References

Not ideal for me.
It's too easy to forget to write the `&` in the function signature and thereby fail to modify what you wanted to modify (if non-const) or accidentally copy (if const).
Since the call is same either way (doesn't require `&` or anything at the call site), there isn't really anything that prevents this.
It also means you can't tell from the call site whether the arguments will be modified.

### `enum class`

Helpful, but I wish I could associate data with each.
