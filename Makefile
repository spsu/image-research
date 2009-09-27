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
	cd ./image && $(RM) $(GEN)
	

### FOURIER #########################
fourier: fourier.cpp gui/Window.o gui/Image.o image/Image.o \
	gui/Widget.o gui/ToggleButton.o gui/CheckButton.o \
	gui/Box.o gui/HBox.o gui/VBox.o
	$(C) $(INC) -c fourier.cpp
	$(LN) $(LIB) gui/Window.o gui/Image.o image/Image.o \
	gui/Widget.o gui/ToggleButton.o gui/CheckButton.o \
	gui/Box.o gui/HBox.o gui/VBox.o \
	fourier.o -o fourier

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
gui/ToggleButton.o: gui/Widget.o gui/ToggleButton.hpp gui/ToggleButton.hpp
	$(CD) ./gui && $(C) $(INC) -c ToggleButton.cpp
gui/CheckButton.o: gui/ToggleButton.o gui/CheckButton.hpp gui/CheckButton.hpp
	$(CD) ./gui && $(C) $(INC) -c CheckButton.cpp
gui/Box.o: gui/Widget.o gui/Box.hpp gui/Box.hpp
	$(CD) ./gui && $(C) $(INC) -c Box.cpp
gui/HBox.o: gui/Box.o gui/HBox.hpp gui/HBox.hpp
	$(CD) ./gui && $(C) $(INC) -c HBox.cpp
gui/VBox.o: gui/Box.o gui/VBox.hpp gui/VBox.hpp
	$(CD) ./gui && $(C) $(INC) -c VBox.cpp


### CV IMAGE WRAPPER ################
image/Image.o: image/Image.hpp image/Image.cpp
	$(CD) ./image && $(C) $(INC) -c Image.cpp




