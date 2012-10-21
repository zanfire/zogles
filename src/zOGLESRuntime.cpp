#include "zOGLESRuntime.h"

#include "zLogger.h"
#include "zOGLESConfigs.h"
#include "zOGLESContext.h"
#include "zOGLESSurface.h"

zMutex* zOGLESRuntime::_singleton_mtx = new zMutex();
zOGLESRuntime* zOGLESRuntime::_singleton = NULL;


zOGLESRuntime::zOGLESRuntime(void)  {
  _logger = zLogger::get_logger("zOGLESRuntime");
  _display = EGL_NO_DISPLAY;
  _configs = NULL;
  _surface = NULL;
  _context = NULL;
}


zOGLESRuntime::~zOGLESRuntime(void) {
  _logger->release_reference();

  if (_configs != NULL) delete _configs;
  if (_surface != NULL) delete _surface;
  if (_context != NULL) delete _context;

  if (_display != EGL_NO_DISPLAY) {
    EGLBoolean success = eglTerminate(_display);
    if (success != EGL_TRUE) {
      check_op_egl(_logger, "eglTerminate");
    }
    _logger->debug("Terminated EGL.");
  }
}


zOGLESRuntime* zOGLESRuntime::get_instance(void) {
  zScopeMutex scope(_singleton_mtx);
  if (_singleton == NULL) {
    _singleton = new zOGLESRuntime();
  }
  return _singleton;
}
  

void zOGLESRuntime::shutdown(void) {
  if (_singleton != NULL) delete _singleton;
  delete _singleton_mtx;
}


bool zOGLESRuntime::init(zWin* win) {
  _display = win->get_display();

  if (_display == EGL_NO_DISPLAY) {
    return false;
  }

  /* Initialize EGL. */
  EGLint major = -1;
  EGLint minor = -1;
  EGLBoolean success = eglInitialize(_display, &major, &minor);
  if (success != EGL_TRUE) {
    check_op_egl(_logger, "eglInitialize");
    return false;
  }

  _logger->debug("Initialized EGL (%d.%d).", major, minor);

  eglBindAPI(EGL_OPENGL_ES_API);

  zOGLESConfigs* cfgs = get_configs();
  EGLConfig config = cfgs->choose_match(8, 8, 8);
  
  _surface = new zOGLESSurface();
  _surface->create(_display, config, (EGLNativeWindowType)(win->get_id()));
  _context = new zOGLESContext();
  _context->create(_display, config, _surface->_surface);

  _context->make_current();

  _logger->info("Initialized OpenGL ES runtime.");
  _logger->info("OpenGL ES Version = %s", glGetString(GL_VERSION));
  _logger->info("OpenGL ES Vendor = %s", glGetString(GL_VENDOR));
  _logger->info("OpenGL ES Renderer = %s", glGetString(GL_RENDERER));
  _logger->info("OpenGL ES Extensions = %s", glGetString(GL_EXTENSIONS));


  return true;
}


zOGLESConfigs* zOGLESRuntime::get_configs(void) {
  zScopeMutex scope(_singleton_mtx);
  if (_configs == NULL) {
    _configs = new zOGLESConfigs(_display);
    _configs->load_configs();
  }
  return _configs;
}


bool zOGLESRuntime::check_op_egl(zLogger* logger, char const* op) { 
  EGLint error = eglGetError(); 
  if (error != 0) { 
    logger->error("eglGetError() for op %s: %i (0x%.4x)\n", op, (int)error, (int)error); 
    DebugBreak(); 
  } 
  return (error == 0);
}

bool zOGLESRuntime::check_op_gl(zLogger* logger, char const* op) { 
  GLint error = glGetError(); 
  if (error != 0) { 
    logger->error("glGetError() for op %s: %i (0x%.4x %s)\n", op, (int)error, (int)error, conv_gl_error_to_chars(error)); 
    DebugBreak(); 
  } 
  return (error == 0);
}


char const* zOGLESRuntime::conv_gl_error_to_chars(GLint error) {
  if (error == GL_NO_ERROR)           return "GL_NO_ERROR";
  if (error == GL_INVALID_ENUM)       return "GL_INVALID_ENUM";
  if (error == GL_INVALID_VALUE)      return "GL_INVALID_VALUE";
  if (error == GL_INVALID_OPERATION)  return "GL_INVALID_OPERATION";
  if (error == GL_OUT_OF_MEMORY)      return "GL_OUT_OF_MEMORY";
  return "??";
}