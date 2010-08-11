#!/usr/bin/env python2.6

import os
import sys

def rel_to_abspath(path, cwd=True):
	"""Convert relative path to abspath"""
	p = os.getcwd() if cwd else os.path.dirname(__file__)
	return os.path.abspath(os.path.join(p, path))

# Put the SWIG wrapper libraries into the syspath
sys.path.append(rel_to_abspath('../build/lib', False))

import pygtk
pygtk.require('2.0')

import gtk
import cv

def cv_to_pixbuf(img):
	"""Convert an IplImage to GdkPixbuf."""
	out = gtk.gdk.pixbuf_new_from_data(
				img.getImageData(), 
				gtk.gdk.COLORSPACE_RGB,
				False, 			# No alpha channel
				img.getDepth(), # Bits per sample
				img.getWidth(),
				img.getHeight(),
				img.getWidthStep() # Rowstride
	)
	return out

class HelloWorld:

	# This is a callback function. The data arguments are ignored
	# in this example. More on callbacks below.
	def hello(self, widget, data=None):
		print "Hello World"

	def delete_event(self, widget, event, data=None):
		# If you return FALSE in the "delete_event" signal handler,
		# GTK will emit the "destroy" signal. Returning TRUE means
		# you don't want the window to be destroyed.
		# This is useful for popping up 'are you sure you want to quit?'
		# type dialogs.
		print "delete event occurred"

		# Change FALSE to TRUE and the main window will not be destroyed
		# with a "delete_event".
		return False

	def destroy(self, widget, data=None):
		print "destroy signal occurred"
		gtk.main_quit()

	def __init__(self):
		# create a new window
		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    
		# When the window is given the "delete_event" signal (this is given
		# by the window manager, usually by the "close" option, or on the
		# titlebar), we ask it to call the delete_event () function
		# as defined above. The data passed to the callback
		# function is NULL and is ignored in the callback function.
		self.window.connect("delete_event", self.delete_event)
    
		# Here we connect the "destroy" event to a signal handler.  
		# This event occurs when we call gtk_widget_destroy() on the window,
		# or if we return FALSE in the "delete_event" callback.
		self.window.connect("destroy", self.destroy)

		# Sets the border width of the window.
		self.window.set_border_width(10)
    
		# Creates a new button with the label "Hello World".
		self.button = gtk.Button("Hello World")

		# When the button receives the "clicked" signal, it will call the
		# function hello() passing it None as its argument.  The hello()
		# function is defined above.
		self.button.connect("clicked", self.hello, None)
    
		# This will cause the window to be destroyed by calling
		# gtk_widget_destroy(window) when "clicked".  Again, the destroy
		# signal could come from here, or the window manager.
		self.button.connect_object("clicked", gtk.Widget.destroy, self.window)


		self.image = gtk.Image()


		path = rel_to_abspath('../media/example.jpg', False)

		if len(sys.argv) > 1:
			path = rel_to_abspath(sys.argv[1])

		print path


		self.im = cv.Image(path)
		#self.pb = self.im.toPixbuf()
		#print dir(self.pb)
		#print type(self.pb)
		#self.image.set_from_pixbuf(self.pb)

		self.pb = cv_to_pixbuf(self.im)
		self.image.set_from_pixbuf(self.pb)

		#self.image.set_from_file('../../media/example.jpg')


		# This packs the button into the window (a GTK container).
		self.window.add(self.image)

		# The final step is to display this newly created widget.
		self.image.show()
    
		# and the window
		self.window.show()

	def main(self):
		# All PyGTK applications must have a gtk.main(). Control ends here
		# and waits for an event to occur (like a key press or mouse event).
		gtk.main()

# If the program is run directly or passed as an argument to the python
# interpreter then create a HelloWorld instance and show it
if __name__ == "__main__":
	hello = HelloWorld()
	hello.main()

