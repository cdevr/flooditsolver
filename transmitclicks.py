import sys
import Xlib
from Xlib import X, display
from Xlib.ext import xtest
import time


coords = {
	1: (375, 285),
	2: (423, 285),
	3: (468, 285),
	4: (375, 325),
	5: (423, 325),
	6: (468, 325),
}

def transmit(sol):
	d = display.Display()
	s = d.screen()
	root = s.root

	for p in sol:
		x,y = coords[p]	
		root.warp_pointer(x, y)
		xtest.fake_input(d, Xlib.X.ButtonPress, 1)
		time.sleep(0.1)
		xtest.fake_input(d, Xlib.X.ButtonRelease, 1)
		d.sync()

		time.sleep(0.1)	

	time.sleep(1)	

	x = 600
	y = 495

	root.warp_pointer(x, y)
	xtest.fake_input(d, Xlib.X.ButtonPress, 1)
	time.sleep(0.1)
	xtest.fake_input(d, Xlib.X.ButtonRelease, 1)
	d.sync()

if __name__ == "__main__":
	line = sys.stdin.readline().strip()
	transmit(map(int, line.split(" ")))
