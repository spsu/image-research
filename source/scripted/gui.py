import os
import sys
import string

import pygtk
pygtk.require('2.0')
import gtk

# SWIG wrappers for C++ functions
import cv
import img

from keymap import KEYMAP, get_key

def rel_to_abspath(path, cwd=True):
	"""Convert relative path to abspath"""
	p = os.getcwd() if cwd else os.path.dirname(__file__)
	return os.path.abspath(os.path.join(p, path))

class Window(gtk.Window):
	"""A basic GTK window."""

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

	def __init__(self, title="Untitled", width=100, height=100):

		super(Window, self).__init__(gtk.WINDOW_TOPLEVEL)

		# Signal event handlers
		self.connect("destroy", self._destroy_cb)
		self.connect("delete_event", self._delete_cb)
		self.connect("drag_motion", self._drag_cb)
		self.connect("drag_motion", self._drop_cb)
		self.connect("drag_data_get", self._data_get_cb)
		self.connect("drag_data_received", self._data_received_cb)
		self.connect("key_press_event", self._key_press_cb)

		# Window params
		self.set_title(title)
		self.set_border_width(0)
		self.set_default_size(width, height)
		self.set_size_request(1, 1) # Make sure we can resize window to any dimension

		# Drag and drop
		self.drag_dest_set(gtk.DEST_DEFAULT_ALL, self.TARGET_TYPES, gtk.gdk.ACTION_COPY)

		self.show()

	def clone(self, arg=None):
		pass

	def close(self, widget, data=None):
		"""Manually close the window from an event we triggered."""
		gtk.Widget.destroy(self)

	def gtk_start(self):
		"""Start GTK main loop."""
		gtk.main()
	
	def gtk_quit(self): 
		"""Quits the GTK main loop."""
		gtk.main_quit()


	# =========== Callbacks ========================

	def _key_press_cb(self, window, event):
		"""Handle a key press"""
		ctrl = lambda: bool(gtk.gdk.CONTROL_MASK & event.state)
		alt = lambda: bool(gtk.gdk.MOD1_MASK & event.state)
		shift = lambda: bool(gtk.gdk.SHIFT_MASK & event.state)
		logo = lambda: bool(gtk.gdk.MOD4_Mask & event.state)

		key = get_key(event.keyval)
		if not key:
			key = event.string # XXX: May not always contain a value

		# Exit the application: mimics common desktop and terminal shortcuts
		if ctrl() and key.upper() in ['Q', 'W', 'C']:
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

	#def _expose_cb(self, widget, event, data=None):
	#	"""When the window is exposed or resized."""
	#	pass

class ImageWindow(Window):
	"""An Image Window"""

	def load_file(self, filename):
		"""Load an image from the file specified."""
		self.image.load_file(filename, True)

	def toggle_histogram(self):
		# XXX/TODO: Move to MainImageWidget, as that's where this belongs. 
		self.image.toggle_histogram()
		self.position_histogram()

	def update_histogram(self):
		"""Update the image's associated histogram."""
		self.image.update_histogram()

	def position_histogram(self):
		if not self.image.histogram.exists():
			return False

		width = self.image.get_width()
		height = self.image.get_height()
		h_width = self.image.histogram.get_width()
		h_height = self.image.histogram.get_height()

		x = width - h_width 
		y = height - h_height

		self.fixed.move(self.image.histogram, x, y)


	def __init__(self, filename, title="Untitled Window", width=100, height=100):
		"""Initialize an ImageWindow"""	
		super(ImageWindow, self).__init__(title, width, height)

		self.image = MainImageWidget()
		self.vbox = gtk.VBox()
		self.fixed = gtk.Fixed()
		self.alignment = gtk.Alignment(0.5, 0.5)

		# Signal event handlers
		#self.alignment.connect("expose_event", self._expose_cb)
		#self.alignment.connect("size-allocate", self._size_allocate_cb)

		# Additional window params
		self.modify_bg(gtk.STATE_NORMAL, gtk.gdk.Color(0, 0, 0))	
	
		# VBox first
		vbox = self.vbox
		vbox.show()
		#self.add(vbox)

		# Next, fixed placement	
		fixed = self.fixed
		fixed.show()

		# Alignment allows us to center the image widget
		alignment = self.alignment
		alignment.add(self.fixed)
		alignment.show()	
		#vbox.pack_start(self.alignment)
		self.add(alignment)

		# TODO: Status message
		# XXX #self.vbox.pack_start(status)

		# Image
		image = self.image
		#image.load_file(filename, True)
		self.load_file(filename)

		fixed.put(image, 0, 0)
		#fixed.put(button, 100, 100)
		fixed.put(image.histogram, 0, 0) # Need to position
		self.position_histogram()

	# =========== Debug Methods =================

	def print_allocations(self):
		"""Print the allocation boxes for each widget to better
		understand dynamic resizing."""	

		print '\n-------- Allocations ----------'
		def print_allocation(title, widget):
			alloc = widget.get_allocation()
			print "%s: %d, %d, %d, %d" % (title, alloc.width, alloc.height, 
											alloc.x, alloc.y)
		print "WIDTH, HEIGHT, X, Y"
		print_allocation("Window", self)
		print_allocation("Vbox", self.vbox)
		print_allocation("Alignment", self.alignment)
		print_allocation("Fixed", self.fixed)
		print_allocation("Image", self.image)
		print "\n"	

	def random_resize(self):
		"""Randomly resize the image to test thumb generation."""
		import random
		w = random.randint(80, 600)
		h = random.randint(80, 600)
		print "Random Resize dimensions: %dx%d" % (w, h)
		self.image.resize(w, h)


	# =========== Callbacks =====================

	def _size_allocate_cb(self, widget, allocation, data=None):
		self.print_allocations()

		width = allocation.width
		height = allocation.height

		print "%dx%d" % (width, height)

		print "----------"

		self.image.resize(width, height)

		


	def _expose_cb(self, widget, event, data=None):
		"""When the window is exposed or resized."""

		self.print_allocations()

		#area = event.area
		#print "%dx%d" % (area.width, area.height)
		#print "x,y: %d,%d" %(area.x, area.y)
	
		#x = area.width + area.x
		#y = area.height + area.y

		#print "X,Y: %d, %d" %(x, y)

		width = widget.allocation.width
		height = widget.allocation.height

		print "%dx%d" % (width, height)

		print "----------"

		self.image.resize(width, height)

		return False
		
		


		#self.image.refresh()


		#print dir(event)


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

		# XXX
		# TODO
		# XXX
		if key.upper() == 'H':
			self.toggle_histogram()

		if key.upper() == 'R':
			self.random_resize()

		return True

	def _click_cb(self, button, args):
		print "Clicked"
		img.grayscale(self.image.cv)
		self.image.refresh()

		self.update_histogram()

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


# ==========================================================

class ImageWidget(gtk.Image):

	def __init__(self):
		super(ImageWidget, self).__init__()
		self.cv = None
		self.pixbuf = None
		self.show()

	def get_width(self):
		"""Get the width of the image."""
		if not self.cv:
			return None
		return self.cv.getWidth()

	def get_height(self):
		"""Get the height of the image."""
		if not self.cv:
			return None
		return self.cv.getHeight()

	def clear(self):
		"""Free the CV and pixbuf memory, which are both non-display, 
		then clear the GtkImage fields."""
		super(ImageWidget, self).clear()
		self.cv = None
		self.pixbuf = None

	def refresh(self):
		"""Refresh the GtkImage from the Cv::Image (perhaps we updated 
		it elsewhere)."""
		print "ImageWidget.refresh()"
		self.pixbuf = self.cv_to_pixbuf(self.cv)
		self.set_from_pixbuf(self.pixbuf)



	def resize(self, width, height): # XXX: TODO
		"""Resize the widget"""

		if width < 2 or height < 2:
			print "Can't resize any smaller."
			return

		# XXX: Make it so we can't resize any 'bigger' (TODO/TEMP)
		norm_width = self.cv.getWidth()
		norm_height = self.cv.getHeight()

		width = min(width, norm_width)
		height = min(height, norm_height)	

		print "Original Image Size: %dx%d, Resize Asked: %dx%d" % (norm_width, norm_height, width, height)

		if width >= norm_width and height >= norm_height:
			print "No need to resize. Window is large enough for original"
			return

		cv_temp = img.resize_to_scale(self.cv, width, height)

		w = cv_temp.getWidth()
		h = cv_temp.getHeight()

		print "cv resized: %dx%d" % (w, h)

		pb = self.cv_to_pixbuf(cv_temp)

		print pb

		self.set_from_pixbuf(pb)


	# XXX XXX: Rowstride-related byte padding issues!! Please fix!!
	@staticmethod
	def cv_to_pixbuf(cv_img):
		"""Convert a Cv::Image (IplImage) to GdkPixbuf."""
		# XXX/TODO: See if there's a way I can get around SWIG's limitations

		# XXX DEBUG
		width = cv_img.getWidth()
		height = cv_img.getHeight()
		wstep = cv_img.getWidthStep()
		depth = cv_img.getDepth()

		#print "\n--------- cv_to_pixbuf ----------"
		#print "WidthStep: %d" % cv_img.getWidthStep()
		#print "Depth: %d" % cv_img.getDepth()
		#print "Size: %dx%d" % (cv_img.getWidth(), cv_img.getHeight())
		
		data = cv_img.getImageData()
		#print "IMAGE DATA LEN: %d" % len(data)

		calc = width * height * 3 
		#print "Calculated req: %d" % calc

		rem = calc % wstep
		#print "Remainder: %d" % rem

		fill = wstep - rem
		#print "Fill: %d" % fill

		full = calc + fill
		#print "Full: %d" % full

		#print "\n"

		channels = 3

		# XXX: Pixbuf requires padding if data doesn't fit within integer 
		# multiples of the widthstep/rowstride
		##required = width * height * channels	
		datalen = len(data)
		##if actual < required:
		if datalen % wstep != 0:
			padlen = wstep - (datalen % wstep)
			data += "\0" * padlen

		#print "New data len: %d" % len(data)

		out = gtk.gdk.pixbuf_new_from_data(
					#cv_img.getImageData(), 
					data,
					gtk.gdk.COLORSPACE_RGB,
					False,			   # No alpha channel
					cv_img.getDepth(), # Bits per sample
					cv_img.getWidth(),
					cv_img.getHeight(),
					cv_img.getWidthStep() # Rowstride
		)
		return out

class MainImageWidget(ImageWidget):

	def __init__(self, filename=None, gen_histogram=False):
		super(MainImageWidget, self).__init__()

		if filename:
			self.load_file(filename, gen_histogram)

		self.histogram = HistogramWidget()

	def load_file(self, filename, gen_histogram=False):
		"""Load an image file."""
		self.cv = cv.Image(filename)
		self.pixbuf = self.cv_to_pixbuf(self.cv)
		self.set_from_pixbuf(self.pixbuf)
		
		#self.histogram.clear()
		#if gen_histogram:
		#	self.update_histogram()

	def update_histogram(self):
		"""Regenerate the histogram for the image."""
		return
		if not self.histogram:
			self.histogram = HistogramWidget(self.cv)
		else:
			self.histogram.update_histogram(self.cv)

	def toggle_histogram(self):
		"""Toggle the histogram visibility."""
		return
		if self.histogram.get_property('visible'):
			self.histogram.hide()
		else:
			self.histogram.show()


class HistogramWidget(ImageWidget):

	def __init__(self, cvimage=None, height=40, padding_top=4, create=True):
		"""Generate a histogram widget for a Cv::Image."""
		super(HistogramWidget, self).__init__()

		self.height = height 
		self.padding_top = padding_top

		if create and cvimage:
			self.update_histogram(cvimage)

	def update_histogram(self, cvimage):
		"""Update the histogram for a new image and/or params."""
		self.cv = img.histogram(cvimage, 256, self.height, self.padding_top)
		self.pixbuf = self.cv_to_pixbuf(self.cv)
		self.set_from_pixbuf(self.pixbuf)

	def exists(self):
		"""Whether the histogram data exists. False if purged."""
		return bool(self.cv != None)

	def set_width(self, width):
		self.width = width
		if self.exists() and self.get_width() != self.width:
			pass # TODO: Mark state as dirty

	def set_height(self, height): 
		self.height = height
		if self.exists() and self.get_height() != self.height:
			pass # TODO: Mark state as dirty
		
