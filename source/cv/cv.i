/* SWIG wrapper for my CV wrapper code. */

%module(directors="1") cv
%{
#include "Arr.hpp"
#include "Mat.hpp"
#include "Image.hpp"
%}

%include "std_string.i"

%include "Arr.hpp"
%include "Mat.hpp"
%include "Image.hpp"

