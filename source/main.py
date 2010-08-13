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
import string

from scripted.keymap import KEYMAP, get_key

# SWIG wrappers for C++ functions
import cv
import img

# XXX/TODO: See if there's a way I can get around SWIG's limitations
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

class Window(object):
	"""A basic GTK window."""

	def __init__(self, title="Untitled", width=400, height=400):

		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		window = self.window
		
		self.vbox = gtk.VBox()
		vbox = self.vbox

		# Signal event handlers
		window.connect("destroy", self._destroy_cb)
		window.connect("delete_event", self._delete_cb)
		window.connect("drag_motion", self._drag_cb)
		window.connect("drag_motion", self._drop_cb)
		window.connect("drag_data_get", self._data_get_cb)
		window.connect("drag_data_received", self._data_received_cb)
		window.connect("key_press_event", self._key_press_cb)

		# Window params
		window.set_title(title)
		window.set_border_width(0)
		window.set_default_size(width, height)

		# Drag and drop
		window.drag_dest_set(gtk.DEST_DEFAULT_ALL, self.TARGET_TYPES, gtk.gdk.ACTION_COPY)

		# VBox	
		window.add(vbox)

		vbox.show()
		window.show()

	def clone(self, arg=None):
		pass

	def close(self, widget, data=None):
		"""Manually close the window from an event we triggered."""
		gtk.Widget.destroy(self.window)

	def gtk_start(self):
		"""Start GTK main loop."""
		gtk.main()
	
	def gtk_quit(self): 
		"""Quits the GTK main loop."""
		gtk.main_quit()


	# =========== Callbacks ========================

	def _key_press_cb(self, window, event):
		"""Handle a key press"""
		def ctrl():
			return bool(gtk.gdk.CONTROL_MASK & event.state)

		def alt():
			return bool(gtk.gdk.MOD1_MASK & event.state)

		def shift():
			return bool(gtk.gdk.SHIFT_MASK & event.state)

		def super():
			return bool(gtk.gdk.MOD4_Mask & event.state)

		key = get_key(event.keyval)
		if not key:
			key = event.string # XXX: May not always contain a value

		# Exit the application
		if key.upper() in ['Q', 'W', 'C'] and ctrl():
			print "Quitting."
			self.gtk_quit()
			sys.exit()

		return False

	def _click_cb(self, button, data=None):
		"""Handle a click event"""
		pass

	def _delete_cb(self, widget, data=None):
		# Falls through to destroy_cb.
		# We may want to ask "Are you sure?"
		pass

	def _destroy_cb(self, widget, data=None):
		"""Kill GTK main loop."""
		# XXX: Old version kept multiple windows alive and only killed GTK
		# when all windows were closed.
		self.gtk_quit()

	def _drag_cb(self, widget, context, x, y, time):
		"""Called at the start of a drag action (when something is 
		being dragged over the widget)."""
		context.drag_status(gtk.gdk.ACTION_COPY, time)
		return True

	def _drop_cb(self, widget, context, x, y, time):
		pass

	def _data_get_cb(self):
		pass

	def _data_received_cb(self, widget, context, x, y, selection, target_type, time):
		"""This completes the drag by receiving the data."""
		pass

class ImageWindow(Window):
	"""An Image Window"""

	# Valid drag type codes
	DND_TARGET_TEXT = 50
	DND_TARGET_JPG  = 51
	DND_TARGET_PNG  = 52
	DND_TARGET_XDIR  = 53
	DND_TARGET_INODE  = 54
	DND_TARGET_URILIST = 55
	DND_TARGET_MOZURL = 56

	# Valid drag type mimetypes
	TARGET_TYPES = [
		("text/plain", 0, DND_TARGET_TEXT),
		("image/jpg", 0, DND_TARGET_JPG),
		("image/png", 0, DND_TARGET_PNG),
		("x-directory/normal", 0, DND_TARGET_XDIR),
		("inode/directory", 0, DND_TARGET_INODE),
		("text/uri-list", 0, DND_TARGET_URILIST),
		("text/x-moz-url", 0, DND_TARGET_MOZURL)
	]

	def __init__(self, filename, title="Untitled Window", width=400, height=400):
		"""Initialize an ImageWindow"""	

		super(ImageWindow, self).__init__(title, width, height)

		# Define Image
		self.gtk_image = gtk.Image()
		self.cv_image = None
		self.pixbuf = None
	
		window = self.window

		# Signal event handlers
		window.connect("destroy", self._destroy_cb)
		window.connect("delete_event", self._delete_cb)
		window.connect("drag_motion", self._drag_cb)
		window.connect("drag_motion", self._drop_cb)
		window.connect("drag_data_get", self._data_get_cb)
		window.connect("drag_data_received", self._data_received_cb)
		window.connect("key_press_event", self._key_press_cb)


		self.vbox.pack_start(self.gtk_image)


		# Button

		button = gtk.Button('Test')
		button.show()
		button.connect("clicked", self._click_cb, None)

		self.vbox.pack_start(button)

		# Image
	
		self.set_image_file(filename)

		self.gtk_image.show()


	def set_image_file(self, filename):
		self.cv_image = cv.Image(filename)
		self.pixbuf = cv_to_pixbuf(self.cv_image)
		self.gtk_image.set_from_pixbuf(self.pixbuf)

	# =========== Callbacks =====================

	def _key_press_cb(self, window, event):
		"""Handle a key press"""
		# Run defaults first
		if super(ImageWindow, self)._key_press_cb(window, event): 
			return True

		ctrl = lambda: bool(gtk.gdk.CONTROL_MASK & event.state)
		alt = lambda: bool(gtk.gdk.MOD1_MASK & event.state)
		shift = lambda: bool(gtk.gdk.SHIFT_MASK & event.state)
		logo = lambda: bool(gtk.gdk.MOD4_Mask & event.state)

		key = get_key(event.keyval)
		if not key:
			key = event.string # XXX: May not always contain a value
		
		
		return True

	def _click_cb(self, button, args):
		print "Clicked"
		img.grayscale(self.cv_image)
		self.pixbuf = cv_to_pixbuf(self.cv_image)
		self.gtk_image.set_from_pixbuf(self.pixbuf)


	def _data_received_cb(self, widget, context, x, y, selection, target_type, time):
		"""This completes the drag by receiving the data."""
		filename = None
		if target_type in [self.DND_TARGET_URILIST, self.DND_TARGET_MOZURL]:
			filename = selection.data
		elif target_type == self.DND_TARGET_TEXT:
			# TODO: We may need to add extra parsing...
			filename = selection.data

		if not filename:
			return False
		
		filename = filename.strip().replace("file:///", "/")

		if not os.path.isfile(filename):
			return

		self.set_image_file(filename)

def main():
	path = rel_to_abspath('../media/example.jpg', False)

	if len(sys.argv) > 1:
		path = rel_to_abspath(sys.argv[1])

	print path
	
	window = ImageWindow(path)
	window.gtk_start()

if __name__ == '__main__':
	main()


