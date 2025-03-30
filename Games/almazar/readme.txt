ALMAZAR - an "adventure" like game

// The Search for Almazar was written in BASIC by Winston Llamas, 
// a computer science student at RPI, circa 1981. 
// -------------------------------------------------------------------------------------------
// Port of Almazar in C by Arthur O'Dwyer, in May 2021. 
// -------------------------------------------------------------------------------------------
// The current version was adapted to HiTech C by Ladislau Szilagyi, in October 2023
// It will run on any Z80 machine under CP/M

To build the executable, use:

c -v -o almazar.c attempt.c saverest.as rand.as

But, you MUST use my enhanced HiTech C compiler (https://github.com/Laci1953/HiTech-C-compiler-enhanced) & Z80AS (https://github.com/Laci1953/Z80AS), otherwise the original HiTech C compiler fails to compile/assemble the source files...
