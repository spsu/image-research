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
	$(RM) $(GEN) fourier wavelet
	cd ./gui && $(RM) $(GEN)
	cd ./gui/box && $(RM) $(GEN)
	cd ./gui/button && $(RM) $(GEN)
	cd ./gui/entry && $(RM) $(GEN)
	cd ./image && $(RM) $(GEN)
	

### FOURIER #########################
fourier: fourier.cpp image/Image.o \
	gui/Window.o gui/Image.o gui/Widget.o gui/Label.o \
	gui/button/Button.o gui/button/ToggleButton.o gui/button/CheckButton.o \
	gui/box/Box.o gui/box/HBox.o gui/box/VBox.o \
	gui/entry/Entry.o gui/entry/SpinButton.o
	$(C) $(INC) -c fourier.cpp
	$(LN) $(LIB) image/Image.o \
	gui/Window.o gui/Image.o gui/Widget.o gui/Label.o \
	gui/button/Button.o gui/button/ToggleButton.o gui/button/CheckButton.o \
	gui/box/Box.o gui/box/HBox.o gui/box/VBox.o \
	gui/entry/Entry.o gui/entry/SpinButton.o \
	fourier.o -o fourier

### FOURIER #########################
grayscale: grayscale.cpp image/Image.o \
	gui/Window.o gui/Image.o gui/Widget.o gui/Label.o \
	gui/button/Button.o gui/button/ToggleButton.o gui/button/CheckButton.o \
	gui/box/Box.o gui/box/HBox.o gui/box/VBox.o \
	gui/entry/Entry.o gui/entry/SpinButton.o
	$(C) $(INC) -c grayscale.cpp
	$(LN) $(LIB) image/Image.o \
	gui/Window.o gui/Image.o gui/Widget.o gui/Label.o \
	gui/button/Button.o gui/button/ToggleButton.o gui/button/CheckButton.o \
	gui/box/Box.o gui/box/HBox.o gui/box/VBox.o \
	gui/entry/Entry.o gui/entry/SpinButton.o \
	grayscale.o -o grayscale

### WAVELET ########################
wavelet: wavelet.cpp
	$(C) $(INC) -c wavelet.cpp
	$(LN) $(LIB) wavelet.o -o wavelet

### GTK GUI LIBRARY #################
gui/Window.o: gui/Window.hpp gui/Window.cpp
	$(CD) ./gui && $(C) $(INC) -c Window.cpp
gui/Image.o: gui/Image.hpp gui/Image.cpp
	$(CD) ./gui && $(C) $(INC) -c Image.cpp

gui/Widget.o: gui/Widget.hpp gui/Widget.cpp
	$(CD) ./gui && $(C) $(INC) -c Widget.cpp
gui/Label.o: gui/Label.hpp gui/Label.cpp
	$(CD) ./gui && $(C) $(INC) -c Label.cpp
gui/button/Button.o: gui/button/Button.hpp gui/button/Button.cpp
	$(CD) ./gui/button && $(C) $(INC) -c Button.cpp
gui/button/ToggleButton.o: gui/Widget.o gui/button/ToggleButton.hpp \
	gui/button/ToggleButton.hpp
	$(CD) ./gui/button && $(C) $(INC) -c ToggleButton.cpp
gui/button/CheckButton.o: gui/button/ToggleButton.o gui/button/CheckButton.hpp \
	gui/button/CheckButton.hpp
	$(CD) ./gui/button && $(C) $(INC) -c CheckButton.cpp
gui/box/Box.o: gui/Widget.o gui/box/Box.hpp gui/box/Box.hpp
	$(CD) ./gui/box && $(C) $(INC) -c Box.cpp
gui/box/HBox.o: gui/box/Box.o gui/box/HBox.hpp gui/box/HBox.hpp
	$(CD) ./gui/box && $(C) $(INC) -c HBox.cpp
gui/box/VBox.o: gui/box/Box.o gui/box/VBox.hpp gui/box/VBox.hpp
	$(CD) ./gui/box && $(C) $(INC) -c VBox.cpp
gui/entry/Entry.o: gui/entry/Entry.hpp gui/entry/Entry.hpp
	$(CD) ./gui/entry && $(C) $(INC) -c Entry.cpp
gui/entry/SpinButton.o: gui/entry/Entry.o gui/entry/SpinButton.hpp \
	gui/entry/SpinButton.hpp
	$(CD) ./gui/entry && $(C) $(INC) -c SpinButton.cpp


### CV IMAGE WRAPPER ################
image/Image.o: image/Image.hpp image/Image.cpp
	$(CD) ./image && $(C) $(INC) -c Image.cpp




