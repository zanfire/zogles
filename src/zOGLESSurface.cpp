#include "zOGLESSurface.h"

#include "zLogger.h"
#include "zWin.h"

#include <EGL/egl.h>

zOGLESSurface::zOGLESSurface(void) {
  _logger = zLogger::get_logger("zOGLESSurface");
}


zOGLESSurface::~zOGLESSurface(void) {
}
