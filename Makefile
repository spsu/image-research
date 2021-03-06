# ============================================================================ #
# 					Makefile Setup:

C = g++ -g -Wall -fPIC
LN = g++ -g -lstdc++
SHARED = g++ -shared -lstdc++
CD = cd
RM = /bin/rm -f
INC = `pkg-config --cflags-only-I opencv gtk+-2.0` # missing libfeat 
LIB = `pkg-config --libs opencv  gtk+-2.0` # missing lpthread, lfeat, lstdc++
LOCALLIB = -Lbuild/lib -Wl,-rpath build/lib -llocal_app -llocal_cv \
		   -llocal_gtk -llocal_v4l2 -llocal_img
GEN = *.o *.a *.so *.out

all: 
	@echo "Cannot compile all programs at once."
	@echo "make [fourier, wavelet, seamcarve, (etc)]"

.PHONY: clean
clean:
	$(RM) $(GEN) panorama stereo fourier wavelet test testv4l grayscale \
				 threshold negative
	cd ./build && $(RM) */*.o */*.so */*/*.o */*/*.so 

.PHONY: stats
stats:
	@wc -l -w -c -L */*.*pp */*/*.*pp */*/*/*.*pp */*/*/*/*.*pp
	@echo " Lines\tWords\tBytes\tLine\t"


# ============================================================================ #
# 					Applications:

### GRAYSCALE #########################
grayscale: source/grayscale.cpp libs
	@echo "[compile] grayscale"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/grayscale.cpp
	@$(LN) $(LIB) $(LOCALLIB) build/out/grayscale.o -o grayscale
	@chmod +x grayscale

### THRESHOLD #########################
threshold: source/threshold.cpp libs
	@echo "[compile] threshold"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/threshold.cpp
	@$(LN) $(LIB) $(LOCALLIB) build/out/threshold.o -o threshold 
	@chmod +x threshold 

### STEREOSCOPIC ######################
stereo: source/stereo.cpp libs
	@echo "[compile] stereo"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/stereo.cpp
	@$(LN) $(LIB) $(LOCALLIB) build/out/stereo.o -o stereo
	@chmod +x stereo

### NEGATIVE ##########################
negative: source/negative.cpp libs
	@echo "[compile] negative"
	@$(CD) ./build/out && $(C) $(INC) -c ../../source/negative.cpp
	@$(LN) $(LIB) $(LOCALLIB) build/out/negative.o -o negative
	@chmod +x negative


# ============================================================================ #
# 					Libraries:

### ALL LIBS ##########################
libs: build/lib/liblocal_app.so build/lib/liblocal_cv.so \
	  build/lib/liblocal_gtk.so build/lib/liblocal_v4l2.so \
	  build/lib/liblocal_img.so
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
build/out/cv/*.o: source/cv/*.hpp source/cv/*.cpp source/cv/*/*.hpp \
				  source/cv/*/*.cpp
	@echo "[compile] OpenCV Wrapper Code"
	@$(CD) ./build/out/cv && $(C) $(INC) -c ../../../source/cv/*.cpp
	@$(CD) ./build/out/cv && $(C) $(INC) -c ../../../source/cv/*/*.cpp


### GTK WRAPPER LIB ###################
build/lib/liblocal_gtk.so: build/out/gtk/*.o
	@echo "[linking] Gtk Wrapper Code"
	@$(CD) ./build/lib && $(SHARED) ../out/gtk/*.o -o liblocal_gtk.so
build/out/gtk/*.o: source/gtk/*.hpp source/gtk/*.cpp source/gtk/*/*.hpp \
				   source/gtk/*/*.cpp
	@echo "[compile] Gtk Wrapper Code"
	@$(CD) ./build/out/gtk && $(C) $(INC) -c ../../../source/gtk/*.cpp
	@$(CD) ./build/out/gtk && $(C) $(INC) -c ../../../source/gtk/*/*.cpp


### V4L2 WRAPPER LIB ####################
build/lib/liblocal_v4l2.so: build/out/v4l2/*.o
	@echo "[linking] Video4Linux2 Wrapper Code"
	@$(CD) ./build/lib && $(SHARED) ../out/v4l2/*.o -o liblocal_v4l2.so
build/out/v4l2/*.o: source/v4l2/wrap/*.cpp source/v4l2/easyapi/*.cpp \
					source/v4l2/easyapi/*/*.cpp
	@echo "[compile] Video4Linux2 Wrapper Code"
	@$(CD) ./build/out/v4l2 && $(C) $(INC) -c ../../../source/v4l2/wrap/*.cpp
	@$(CD) ./build/out/v4l2 && $(C) $(INC) -c ../../../source/v4l2/easyapi/*.cpp
	@$(CD) ./build/out/v4l2 && $(C) $(INC) -c \
		   ../../../source/v4l2/easyapi/*/*.cpp

### IMAGE FUNCTIONS #####################
build/lib/liblocal_img.so: build/out/img/*.o
	@echo "[linking] Image Functions"
	@$(CD) ./build/lib && $(SHARED) ../out/img/*.o -o liblocal_img.so
build/out/img/*.o: source/img/*.hpp source/img/*.cpp
	@echo "[compile] Image Functions"
	@$(CD) ./build/out/img && $(C) $(INC) -c ../../../source/img/*.cpp


