#!/usr/bin/python
import gtk.gdk

cmap = {
	(237, 112, 161): 1,
  (96, 92, 168): 2,
	(243, 246, 29): 3,
	(220,74,32): 4,
	(70, 177, 226): 5,
	(126,157,30): 6,
}

def read_screenshot():
	import numpy
	r = numpy.zeros((14,14), numpy.int8)
	w = gtk.gdk.get_default_root_window()
	sz = w.get_size()
	#print "The size of the window is %d x %d" % sz
	pb = gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB,False,8,sz[0],sz[1])
	pb = pb.get_from_drawable(w,w.get_colormap(),0,0,0,0,sz[0],sz[1])
	if (pb != None):
			x = 508
			y = 269
			th = 24
			tw = 24
			width = 14 * tw
			height = 14 * th
			dst = gtk.gdk.Pixbuf(gtk.gdk.COLORSPACE_RGB, False, 8, width, height)
			pb.copy_area(x,y,width,height,dst,0,0)
			dst.save("screenshot.png","png")
			#print "Screenshot saved to screenshot.png."

			colors = dst.get_pixels_array();
			for x in range(14):
				for y in range(14):
					#print "(%s, %s) = %s" % (x,y,tuple(colors[y * th + 5, x * tw + 5]))
					r[y,x] = cmap[tuple(colors[y * th + 5, x * tw + 5])]
	else:
			print "Unable to get the screenshot."

	#print r
	from solvefloodit import *
	print_state(r)

	return r

def read_screenshot_and_write(fn):
	r = read_screenshot()
	f = open(fn, "w")
	for y in range(14):
		f.write(" ".join(map(lambda x: str(x), r[y])))
		f.write("\n")


if __name__ == "__main__":
	read_screenshot_and_write("q")
