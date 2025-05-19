# Word Search in Binary Search Tree

Language: C

## Description

This project is a C program that processes a text file to extract all words, filters out common stop words, and stores the remaining words in a Binary Search Tree (BST). It then allows the user to search for a specific word and returns the depth of that word in the tree. 

Project was tested using the text of "Sir Thaddeus, or the Last Lithuanian Foray: A Nobleman's Tale from the Years of 1811 and 1812 in Twelve Books of Verse" (in Polish "Pan Tadeusz, czyli ostatni zajazd na Litwie. Historia szlachecka z roku 1811 i 1812 we dwunastu księgach wierszem") - a long poem written by great Polish romantic poet Adam Mickiewicz.

## Input files

pan-tadeusz.txt: A sample input file (in this case the text of "Sir Thaddeus, or the Last Lithuanian Foray: A Nobleman's Tale from the Years of 1811 and 1812 in Twelve Books of Verse") to extract words from.
stop.txt: A list of stop words (e.g., "a", "ale", "bo") to exclude from tree structure
utf8_word.txt: Contains a word (possibly with UTF-8 characters) that will be automatically searched in the tree.

## Key concepts

- Linked Lists for storing sequences of words
- Binary Search Tree for fast word lookup
- Memory management using dynamic allocation
