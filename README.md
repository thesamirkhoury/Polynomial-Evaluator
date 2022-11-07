# Polynomial Evaluator

---

## Description

The program takes a polynomial equation from the user and evaluates the result.

## Main Functions
1- scanPol - takes an input and breaks the numbers in a  special data structure to be used to evaluate the data.
	DATA STRUCTURE: Data at index 0-2 are polynomial coefficients according to power. Data at index 3 is the free number with no variables. Data at index 4 is the number to be evaluated. Data at index 5 is the evaluation result.

2- dataReset - deletes the data structure and resets it to zeros.

3- calculate - evaluates the polynomial power depending on the iterator passed by the main function.

## Versions
1- PolEval1 - This file contains a polynomial evaluator using threads.
2- PolEval2 - This file contains a polynomial evaluator using shared memory.

## How to run

To compile:
### PolEval1
in a linux/unix terminal run: `gcc PolEval1.c -o PolEval1 -lm -lpthread`
a compiled file will be created with the name proxy.

To run:
in a linux/unix terminal run: `./PolEval1`

### PolEval2
in a linux/unix terminal run: `gcc PolEval2.c -o PolEval2 -lm`
a compiled file will be created with the name proxy.

To run:
in a linux/unix terminal run: `./PolEval2`