# *OS MINI PROJECT - Synopsis*
***

## *Title:* Sudoku Solution Validator

## *Objective:*

This project consists of designing a multithreaded application that determines whether the solution to a _Sudoku_ puzzle is valid. A _Sudoku_ puzzle uses a 9 × 9 grid in which each column and row, as well as each of the nine 3 × 3 subgrids, must contain all of the digits 1 · · · 9.

This application can be multithreaded in several different ways. The strategy is to create threads that check the following requirements:

* A thread to check that each column contains the digits 1 through 9
* A thread to check that each row contains the digits 1 through 9
* Nine threads to check that each of the 3 × 3 subgrids contains the digits 1 through 9

This would result in a minimum of 11 (or more) separate threads for validating a Sudoku puzzle.


## *Group Member(s):*

Venkat Tummala (190905137)
Rajat Mathew Mammen Roshan (190905087) Diya Patel (190905136)
Srishti Soubhagya (190905110)
