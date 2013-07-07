#!/bin/bash

# readfromscreenshot : will take a screenshot and extract the floodit board
#   then it reads the board, produces a colorized version on stdout and writes
#   a numerical version to the file "q"
# solve : solves the puzzle, needs the file (e.g. "q") as first parameter, and
#   the beam-width as a second parameter
# transmitclicks : sends the clicks to the X server to solve the puzzle
while true; do make; ./readfromscreenshot.py; ./solve q 1000 | tail -n 1 | python transmitclicks.py; done
