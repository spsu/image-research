#ifndef BT_Gtk_types
#define BT_Gtk_types

namespace Gtk {

/**
 * Represents an uninstantiable member of a hierarchy.
 * An example is "Scale" in Widget > Scale > HScale.
 */
enum UninstantiableType
{
	DIRECTLY_UNINSTANTIABLE
};

/**
 * Represents the positioning of an item.
 */
enum PositionType
{
	POS_LEFT,
	POS_RIGHT,
	POS_TOP,
	POS_BOTTOM
};

} // end namespace Gtk

#endif
