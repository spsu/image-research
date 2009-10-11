### VARIABLES #############
C = g++ -g -Wall
LN = g++ -g
CD = cd
RM = /bin/rm -f
INC = `pkg-config --cflags-only-I opencv gtk+-2.0` # missing libfeat 
LIB = `pkg-config --libs opencv  gtk+-2.0` # missing lpthread, lfeat, lstdc++
GEN = *.o *.a *.out

#INCPATH = -I/usr/local/include/opencv -I/usr/local/include/libfeat \
#		`pkg-config --cflags-only-I gtk+-2.0`
#LIBS = -L/usr/local/bin -lhighgui -lstdc++ -lpthread -lfeat \
#		`pkg-config --libs gtk+-2.0`

all: 
	@echo "Cannot compile all programs at once."
	@echo "make [fourier, wavelet, seamcarve, (etc)]"

.PHONY: clean
clean:
	$(RM) $(GEN) fourier wavelet test grayscale
	cd ./app && $(RM) $(GEN)
	cd ./gtk && $(RM) $(GEN)
	cd ./gtk/box && $(RM) $(GEN)
	cd ./gtk/button && $(RM) $(GEN)
	cd ./gtk/entry && $(RM) $(GEN)
	cd ./cv && $(RM) $(GEN)
	

### FOURIER #########################
fourier: fourier.cpp cv/Image.o \
	gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o
	$(C) $(INC) -c fourier.cpp
	$(LN) $(LIB) cv/Image.o \
	gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	fourier.o -o fourier

### GRAYSCALE #########################
grayscale: grayscale.cpp \
	app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o
	$(C) $(INC) -c grayscale.cpp
	$(LN) $(LIB) app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o \
	grayscale.o -o grayscale

### NEGATIVE #########################
negative: negative.cpp \
	app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o
	$(C) $(INC) -c negative.cpp
	$(LN) $(LIB) app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o \
	negative.o -o negative

### WAVELET #########################
wavelet: wavelet.cpp
	$(C) $(INC) -c wavelet.cpp
	$(LN) $(LIB) wavelet.o -o wavelet

### TEST ############################
test: test.cpp \
	app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o
	$(C) $(INC) -c test.cpp
	$(LN) $(LIB) app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o \
	test.o -o test

stereo: stereo.cpp \
	app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o cv/Camera.o
	$(C) $(INC) -c stereo.cpp
	$(LN) $(LIB) app/ImageCache.o app/Gui.o app/ImagePane.o \
	gtk/Gtk.o gtk/Window.o gtk/Image.o gtk/Widget.o gtk/Label.o \
	gtk/button/Button.o gtk/button/ToggleButton.o gtk/button/CheckButton.o \
	gtk/box/Box.o gtk/box/HBox.o gtk/box/VBox.o \
	gtk/entry/Entry.o gtk/entry/SpinButton.o \
	cv/Image.o cv/Camera.o \
	stereo.o -o stereo

### APPLICATION CLASSES #############
app/ImageCache.o: app/ImageCache.hpp app/ImageCache.cpp
	$(CD) ./app && $(C) $(INC) -c ImageCache.cpp
app/Gui.o: app/Gui.cpp
	$(CD) ./app && $(C) $(INC) -c Gui.cpp
app/ImagePane.o: app/ImagePane.hpp app/ImagePane.cpp
	$(CD) ./app && $(C) $(INC) -c ImagePane.cpp

### GTK WRAPPER LIBRARY #############
gtk/Gtk.o: gtk/Gtk.hpp gtk/Gtk.cpp
	$(CD) ./gtk && $(C) $(INC) -c Gtk.cpp

gtk/Window.o: gtk/Window.hpp gtk/Window.cpp
	$(CD) ./gtk && $(C) $(INC) -c Window.cpp
gtk/Image.o: gtk/Image.hpp gtk/Image.cpp
	$(CD) ./gtk && $(C) $(INC) -c Image.cpp

gtk/Widget.o: gtk/Widget.hpp gtk/Widget.cpp
	$(CD) ./gtk && $(C) $(INC) -c Widget.cpp
gtk/Label.o: gtk/Label.hpp gtk/Label.cpp
	$(CD) ./gtk && $(C) $(INC) -c Label.cpp
gtk/button/Button.o: gtk/button/Button.hpp gtk/button/Button.cpp
	$(CD) ./gtk/button && $(C) $(INC) -c Button.cpp
gtk/button/ToggleButton.o: gtk/Widget.o gtk/button/ToggleButton.hpp \
	gtk/button/ToggleButton.hpp
	$(CD) ./gtk/button && $(C) $(INC) -c ToggleButton.cpp
gtk/button/CheckButton.o: gtk/button/ToggleButton.o gtk/button/CheckButton.hpp \
	gtk/button/CheckButton.hpp
	$(CD) ./gtk/button && $(C) $(INC) -c CheckButton.cpp
gtk/box/Box.o: gtk/Widget.o gtk/box/Box.hpp gtk/box/Box.hpp
	$(CD) ./gtk/box && $(C) $(INC) -c Box.cpp
gtk/box/HBox.o: gtk/box/Box.o gtk/box/HBox.hpp gtk/box/HBox.hpp
	$(CD) ./gtk/box && $(C) $(INC) -c HBox.cpp
gtk/box/VBox.o: gtk/box/Box.o gtk/box/VBox.hpp gtk/box/VBox.hpp
	$(CD) ./gtk/box && $(C) $(INC) -c VBox.cpp
gtk/entry/Entry.o: gtk/entry/Entry.hpp gtk/entry/Entry.hpp
	$(CD) ./gtk/entry && $(C) $(INC) -c Entry.cpp
gtk/entry/SpinButton.o: gtk/entry/Entry.o gtk/entry/SpinButton.hpp \
	gtk/entry/SpinButton.hpp
	$(CD) ./gtk/entry && $(C) $(INC) -c SpinButton.cpp


### CV IMAGE WRAPPER ################
cv/Image.o: cv/Image.hpp cv/Image.cpp
	$(CD) ./cv && $(C) $(INC) -c Image.cpp
cv/Camera.o: cv/Camera.hpp cv/Camera.cpp
	$(CD) ./cv && $(C) $(INC) -c Camera.cpp




