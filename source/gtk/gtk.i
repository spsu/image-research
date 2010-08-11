/* SWIG wrapper for my CV wrapper code. */

%module(directors="1") gtkw
%{
#include "all.hpp"
#include "Widget.hpp"
#include "types.hpp"
#include "Adjustment.hpp"
#include "Gtk.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "Window.hpp"
#include "box/Box.hpp"
#include "box/HBox.hpp"
#include "box/VBox.hpp"
#include "scale/Scale.hpp"
#include "scale/HScale.hpp"
#include "scale/VScale.hpp"

%}

%include "std_string.i"

%include "all.hpp"
%include "Widget.hpp"
%include "types.hpp"
%include "Adjustment.hpp"
%include "Gtk.hpp"
%include "Image.hpp"
%include "Label.hpp"
%include "scale/Scale.hpp"
%include "scale/HScale.hpp"
%include "scale/VScale.hpp"
%include "Window.hpp"


