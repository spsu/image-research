# ============================================================================ #
# 					Makefile Setup:

C = g++ -g -Wall -fPIC
LN = g++ -g
SHARED = g++ -shared
CD = cd
RM = /bin/rm -f
INC = `pkg-config --cflags-only-I opencv gtk+-2.0` # missing libfeat 
LIB = `pkg-config --libs opencv  gtk+-2.0` # missing lpthread, lfeat, lstdc++
LOCALLIB = -Lbuild/lib -llocal_app -llocal_cv -llocal_gtk
GEN = *.o *.a *.so *.out

all: 
	@echo "Cannot compile all programs at once."
	@echo "make [fourier, wavelet, seamcarve, (etc)]"

.PHONY: clean
clean:
	$(RM) $(GEN) fourier wavelet test grayscale
	cd ./build && $(RM) */*.o */*.so */*/*.o */*/*.so 

# ============================================================================ #
# 					Applications:

### GRAYSCALE #########################
grayscale: source/grayscale.cpp libs
	@echo "[compile] grayscale"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/grayscale.cpp
	@$(LN) $(LIB) $(LOCALLIB) build/out/grayscale.o -o grayscale
	@chmod +x grayscale

### NEGATIVE #########################
negative: negative.cpp libs
	@echo "[compile] negative"
	@$(C) $(INC) -c negative.cpp
	@$(LN) $(LIB) $(LOCALLIB) negative.o -o negative

### STEREOSCOPIC ####################
stereo: stereo.cpp libs
	@echo "[compile] stereo"
	@$(C) $(INC) -c stereo.cpp
	@$(LN) $(LIB) $(LOCALLIB) stereo.o -o stereo

### FOURIER #########################
fourier: 
	@echo "TODO"

### WAVELET #########################
wavelet: 
	@echo "TODO"

# ============================================================================ #
# 					Libraries:

### ALL LIBS ##########################
libs: build/lib/liblocal_app.so build/lib/liblocal_cv.so \
	  build/lib/liblocal_gtk.so
		@$(CD) .

### APP CODE LIB ######################
build/lib/liblocal_app.so: build/out/app/*.o
	@echo "[linking] App Library Code"
	@$(CD) ./build/lib && $(SHARED) ../out/app/*.o -o liblocal_app.so
build/out/app/*.o: source/app/*.hpp source/app/*.cpp
	@echo "[compile] App Library Code"
	@$(CD) ./build/out/app && $(C) $(INC) -c ../../../source/app/*.cpp

### CV WRAPPER LIB ####################
build/lib/liblocal_cv.so: build/out/cv/*.o
	@echo "[linking] OpenCV Wrapper Code"
	@$(CD) ./build/lib && $(SHARED) ../out/cv/*.o -o liblocal_cv.so
build/out/cv/*.o: source/cv/*.hpp source/cv/*.cpp
	@echo "[compile] OpenCV Wrapper Code"
	@$(CD) ./build/out/cv && $(C) $(INC) -c ../../../source/cv/*.cpp


### GTK WRAPPER LIB ###################
build/lib/liblocal_gtk.so: build/out/gtk/*.o
	@echo "[linking] Gtk Wrapper Code"
	@$(CD) ./build/lib && $(SHARED) ../out/gtk/*.o -o liblocal_gtk.so
build/out/gtk/*.o: source/gtk/*.hpp source/gtk/*.cpp source/gtk/*/*.hpp source/gtk/*/*.cpp
	@echo "[compile] Gtk Wrapper Code"
	@$(CD) ./build/out/gtk && $(C) $(INC) -c ../../../source/gtk/*.cpp
	@$(CD) ./build/out/gtk && $(C) $(INC) -c ../../../source/gtk/*/*.cpp

