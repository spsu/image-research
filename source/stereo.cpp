/**
 * Copyright Brandon Thomas Suit 2009
 * Available under the LGPL 2.
 * <http://possibilistic.org> 
 * <echelon@gmail.com>
 */

#include "app/Gui.hpp"
#include "app/ImagePane.hpp"
#include "cv/Image.hpp"
#include "cv/Camera.hpp"
#include "gtk/all.hpp"
#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h> // TODO: TEMP FIX

/**
 * Globals.
 * TODO: Globals are bad. Make a lookup system/dictionary in App::Gui.
 */

App::ImagePane* imgPane1 = 0;
App::ImagePane* imgPane2 = 0;
Cv::Camera* cam1 = 0;
Cv::Camera* cam2 = 0;

/**
 * Query camera.
 * Function pattern matches callback.
 */
void* queryCam(void* n)
{
	static int turn = 0; // TODO: Temp fix for OpenCV resize bug (RECOMPILE IT!)
	Cv::Image* frame1 = 0;
	Cv::Image* frame2 = 0;
	Gtk::Image* gtkImg1 = 0;
	Gtk::Image* gtkImg2 = 0;

	gtkImg1 = imgPane1->getImage();
	gtkImg2 = imgPane2->getImage();

	cam1 = new Cv::Camera(1);
	cam2 = new Cv::Camera(2);
	sleep(2);

	cam1->setSize(320, 240);
	cam2->setSize(320, 240);

	while(1) {
		sleep(1); // TODO: Try to ensure USB bandwidth is enough.		

		if(turn == 0) {
			turn = 1;
			printf("Query 1...\n");
			frame1 = cam1->queryFrame();
			if(!frame1->isValid()) {
				continue; // skip
			}
			printf("Captured 1\n");
			gtkImg1->setPixbuf(frame1->toPixbuf());
		}
		else {
			turn = 0;
			printf("Query 2...\n");
			frame2 = cam2->queryFrame();
			if(!frame2->isValid()) {
				continue; // skip
			}
			printf("Captured 2\n");
			gtkImg2->setPixbuf(frame2->toPixbuf());
		}
	}
	return 0;
}

/**
 * Query the camera
 */
void queryCamCb(GtkButton* gtkbutton, gpointer data)
{
	static int done = 0;
	pthread_t thread;

	if(done > 0) {
		return;
	}
	done = 1;

	pthread_create(&thread, NULL, queryCam, NULL);
}

/**
 * Main function
 * Sets up the Gui, attaches any callbacks, then starts Gtk.
 */
int main(int argc, char *argv[])
{
	App::Gui* gui = 0;
	Gtk::VBox* vbox = 0;
	Gtk::HBox* hbox = 0;
	Gtk::Button* button = 0;
	printf("Test\n");

	// Create main application elements
	gui = new App::Gui("Negative Demo");
	imgPane1 = new App::ImagePane("./media/example.jpg");
	imgPane2 = new App::ImagePane("./media/example.jpg");
	printf("Test\n");

	// Create other Gtk widgets
	hbox = new Gtk::HBox(false, 0);
	vbox = new Gtk::VBox(false, 0);
	button = new Gtk::Button("Begin");
	printf("Test\n");

	// Construct GUI
	gui->setChild(vbox);
	vbox->packStart(hbox, false, false, 0);
	hbox->packStart(imgPane1->getImage(), true, true, 0);
	hbox->packStart(imgPane2->getImage(), true, true, 0);
	vbox->packStart(button, false, false, 0);
	printf("Test\n");

	// Install negative callback
	button->addClickedCb(queryCamCb, gui);

	gui->start();

	return 0;
}

