#include "zOGLESContext.h"

#include "zLogger.h"
#include "zWin.h"
#include "zOGLESRuntime.h"


zOGLESContext::zOGLESContext(void) {
  _logger = zLogger::get_logger("zOGLESContext");
  _display = EGL_NO_DISPLAY;
  _surface = EGL_NO_SURFACE;
  _context = EGL_NO_CONTEXT;
}


zOGLESContext::~zOGLESContext(void) {
  _logger->release_reference();
}


bool zOGLESContext::create(EGLDisplay display, EGLConfig config, EGLSurface surface) {
  _display = display;
  _surface = surface;

  EGLint contextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

  _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttributes);
  if(_context == EGL_NO_CONTEXT) {
    zOGLESRuntime::check_op_egl(_logger, "eglCreateContext");
    return false;
  }
  _logger->debug("Created EGL context.");

  return true;
}


void zOGLESContext::destroy(void) {
  if (_context != EGL_NO_CONTEXT) {
    eglDestroyContext(_display, _context);
  }
}


void zOGLESContext::make_current(void) {
  eglMakeCurrent(_display, _surface, _surface, _context);
}


void zOGLESContext::swap_buffers(void) {
  eglSwapBuffers(_display, _surface); 
}