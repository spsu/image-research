#ifndef V4L2_all
#define V4L2_all

/**
 * These are the core V4L2 API wrappers. 
 * XXX: Don't add any extra functionality beyond what seems reasonable.
 */
#include "wrap/Buffer.hpp"
#include "wrap/Capability.hpp"
#include "wrap/Device.hpp"
#include "wrap/Format.hpp"
#include "wrap/RequestBuffers.hpp"

/**
 * EasyAPI is a set of classes that abstract away the core V4L2 API, making it
 * simple to set up, manipulate, and use. The V4L2 primative wrappers may still
 * be used and intermixed, of course. 
 */
#include "easyapi/Buffers.hpp"
#include "easyapi/Camera.hpp"
#include "easyapi/Frame.hpp"
#include "easyapi/Frame/DriverFrame.hpp" // TODO: Rename MemMappedFrame ??
#include "easyapi/RgbImage.hpp"

#endif
