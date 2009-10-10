#ifndef APP_IMAGEPANE
#define APP_IMAGEPANE

#include <string>
#include <gtk/gtk.h> // TODO: TEMP

/*namespace Cv {
	class Image;
}*/
namespace Gtk {
	class Image;
}

namespace App {
class ImagePane
{
	public:
		ImagePane(std::string filename = std::string());
		~ImagePane();

		/**
		 * Get the Gtk::Image widget.
		 */
		Gtk::Image* getImage();

		/**
		 * Drag and drop targets.
		 * TODO XXX TEMP: These don't belong here.
		 * Make a gtk/DragDrop class or something to automate adding types. 
		 */
		static const unsigned int DND_TARGET_TEXT = 50;
		static const unsigned int DND_TARGET_JPG  = 51;
		static const unsigned int DND_TARGET_PNG  = 52;
		static const unsigned int DND_TARGET_XDIR  = 53;
		static const unsigned int DND_TARGET_INODE  = 54;
		static const unsigned int DND_TARGET_URILIST = 55;
		static const unsigned int DND_TARGET_MOZURL = 56;

	protected:
		/**
		 * Drag callback.
		 */
		static gboolean dragMotionCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				gint x, gint y, guint time, gpointer obj);

		/**
		 * Drag callback.
		 */
		static void dragDataGetCb(GtkWidget* widget, GdkDragContext* dragCtx, 
				GtkSelectionData* data, guint info, guint time, gpointer obj);

		/**
		 * Drag callback.
		 */
		static void dragDataReceivedCb(GtkWidget* widget, 
				GdkDragContext* dragCtx, gint x, gint y, GtkSelectionData* data,
				guint info, guint time, gpointer obj);

	private:
		Gtk::Image* gtkImage;
		
};
}

#endif
