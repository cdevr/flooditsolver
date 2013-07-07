#!/usr/bin/python
import os
import time

# readfromscreenshot : will take a screenshot and extract the floodit board
#   then it reads the board, produces a colorized version on stdout and writes
#   a numerical version to the file "q"
# solve : solves the puzzle, needs the file (e.g. "q") as first parameter, and
#   the beam-width as a second parameter
# transmitclicks : sends the clicks to the X server to solve the puzzle

#while true; do make; ./readfromscreenshot.py; ./solve q 1000 | tail -n 1 | python transmitclicks.py; done
def getMoves(cmd):
	os.system(cmd)
	sol = open("sol").readlines()[-1].strip()
	sol = map(int, sol.split(" "))

	return sol

if __name__ == "__main__":
	import readfromscreenshot

	while 1:
		readfromscreenshot.read_screenshot_and_write("q")

		bestsol = None
		for beamwidth in (100, 500, 250, 750, 1000, 1005, 995, 2000, 5000, 5500, 6000, 15000, 20000, 50000, 100000):
			import random
			fudge = random.randint(1,20)

			beamwidth = beamwidth + fudge
			if beamwidth < 150:
				os.system("./solve2 q %s 2>&1 > sol" % beamwidth)
			else:
				os.system("./solve q %s 2>&1 > sol" % beamwidth)

			sol = open("sol").readlines()[-1].strip()
			sol = map(int, sol.split(" "))
			print "moves at beamwidth %s : %s" % (beamwidth, len(sol))

			if bestsol == None:
				bestsol = sol
			elif len(sol) < len(bestsol):
				bestsol = sol

			if beamwidth >= 950 and len(bestsol) <= 25:
				from transmitclicks import transmit
				transmit(bestsol)
				break # jump back into the while loop
			else:
				#print "Couldn't find solution with beamwidth %s (best sol: %s)" % (beamwidth, len(sol))
				pass
		time.sleep(0.5) # give the game the chance to remove "you've won" text
