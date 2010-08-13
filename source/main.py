#!/usr/bin/env python2.6

import os
import sys

# Put the SWIG wrapper libraries into the syspath
sys.path.append(os.path.join(os.path.dirname(__file__), '../build/lib'))

from scripted.gui import ImageWindow
from scripted.gui import rel_to_abspath

def main():
	path = rel_to_abspath('../../media/example.jpg', False) # rel to scripted/ dir.
	if len(sys.argv) > 1:
		path = rel_to_abspath(sys.argv[1])

	print path
	
	window = ImageWindow(path)
	window.gtk_start()

if __name__ == '__main__':
	main()


