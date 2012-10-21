#include "zOGLESSurface.h"

#include "zLogger.h"
#include "zWin.h"
#include "zOGLESRuntime.h"


zOGLESSurface::zOGLESSurface(void) {
  _logger = zLogger::get_logger("zOGLESSurface");
  _display = EGL_NO_DISPLAY;
}


zOGLESSurface::~zOGLESSurface(void) {
  if(_surface != EGL_NO_SURFACE) {
    eglDestroySurface(_display,_surface);
  }
  _logger->release_reference();
}

bool zOGLESSurface::create(EGLDisplay display, EGLConfig config, EGLNativeWindowType native_win) {
  _display = display;

  ///Using the defaults (EGL_RENDER_BUFFER = EGL_BACK_BUFFER).
  EGLint windowAttributes[] = {
    //EGL_RENDER_BUFFER,     EGL_BACK_BUFFER,
    EGL_NONE
  };

  // Create a surface.
  _surface = eglCreateWindowSurface(display, config, native_win, windowAttributes);
  if(_surface == EGL_NO_SURFACE) {
    zOGLESRuntime::check_op_egl(_logger, "eglCreateWindowSurface");
    return false;
  }

  return true;
}
