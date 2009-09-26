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
	#cd ./ai && $(RM) -f *.o *.out
	

### FOURIER #########################
fourier: fourier.cpp gui/Window.o gui/Image.o
	$(C) $(INC) -c fourier.cpp
	$(LN) $(LIB) gui/Window.o gui/Image.o fourier.o -o fourier

### WAVELET ########################
wavelet: wavelet.cpp
	$(C) $(INC) -c wavelet.cpp
	$(LN) $(LIB) wavelet.o -o wavelet

### GTK GUI LIBRARY #################
gui/Window.o: gui/Window.hpp gui/Window.cpp
	$(CD) ./gui && $(C) $(INC) -c Window.cpp
gui/Image.o: gui/Image.hpp gui/Image.cpp
	$(CD) ./gui && $(C) $(INC) -c Image.cpp

