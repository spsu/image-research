/* SWIG wrapper for my CV wrapper code. */

%module(directors="1") gtkw
%{
#include "Widget.hpp"
#include "types.hpp"
#include "Adjustment.hpp"
#include "Gtk.hpp"
#include "Image.hpp"
#include "Label.hpp"
#include "scale/Scale.hpp"
#include "scale/HScale.hpp"
#include "scale/VScale.hpp"
#include "Window.hpp"

%}

%include "std_string.i"

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


