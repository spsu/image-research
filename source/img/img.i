/* SWIG wrapper for image processing code. */

%module img
%{
#include "Color.hpp"
#include "grayscale.hpp"
#include "negative.hpp"
#include "threshold.hpp"
#include "histogram.hpp"
%}

%include "std_string.i"

%include "Color.hpp"
%include "grayscale.hpp"
%include "negative.hpp"
%include "threshold.hpp"
%include "histogram.hpp"

