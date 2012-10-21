#include "zOGLESConfigs.h"

#include "zLogger.h"
#include "zWin.h"
#include "zOGLESRuntime.h"


zOGLESConfigs::zOGLESConfigs(EGLDisplay display) : _configs(NO, 32) {
  _logger = zLogger::get_logger("zOGLESConfig");
  _display = display;
  /*
  EGLint configAttributes[] =
  {
    // These attributes are in a known order and may be re-written at initialization according to application requests.
    EGL_SAMPLES,             4,
    EGL_RED_SIZE,            8,
    EGL_GREEN_SIZE,          8,
    EGL_BLUE_SIZE,           8,
    EGL_ALPHA_SIZE,          0,

    EGL_BUFFER_SIZE,         32,
    EGL_STENCIL_SIZE,        0,
    EGL_RENDERABLE_TYPE,     0,    /// This field will be completed according to application request.
    EGL_SURFACE_TYPE,        EGL_WINDOW_BIT | EGL_PIXMAP_BIT,
    EGL_DEPTH_SIZE,          16,
    // If you need to add or override EGL attributes from above, do it below here.
    EGL_NONE
  };

  configAttributes[15] = EGL_OPENGL_ES2_BIT;
  
        
  // Disable AntiAliasing, useful if it's not supported on a platform.
  if (true) {
    configAttributes[1] = EGL_DONT_CARE;
  }
  */
}


zOGLESConfigs::~zOGLESConfigs(void) {
  _logger->release_reference();
}


bool zOGLESConfigs::load_configs(void) {
  int count = 0;

  // Enumerate available EGL configurations which match or exceed our required attribute list.
  //EGLBoolean success = eglChooseConfig(_display, /*configAttributes*/ NULL, NULL, 0, &count);
  EGLBoolean success = eglGetConfigs(_display, NULL, 0, &count);
  if (success != EGL_TRUE) {
    zOGLESRuntime::check_op_egl(_logger, "eglGetConfigs get count");
    return false;
  }

  _logger->debug("Number of configs found is %d\n", count);

  // Allocate space for all EGL configs available and get them.
  EGLConfig* configs = (EGLConfig*)malloc(count * sizeof(EGLConfig));
  int c = 0;
  success = eglGetConfigs(_display, configs, count, &c);
  // TODO: Check c != couny
  if(success != EGL_TRUE) {
    zOGLESRuntime::check_op_egl(_logger, "eglGetConfigs.");
    return false;
  }

  for (int i = 0; i < c; i++) {
    _configs.append(configs[i]);
  }

  free(configs);
  return true;
}


EGLConfig zOGLESConfigs::choose_match(int red_size, int green_size, int blue_size) {
  // Loop through the EGL configs to find a color depth match.
  // Note: This is necessary, since EGL considers a higher color depth than requested to be 'better'
  // even though this may force the driver to use a slow color conversion blitting routine. 
  for(int i = 0; i < _configs.get_count(); i++) {
    EGLConfig config;
    if (_configs.get(i, &config)) {
      EGLint r = 0;
      EGLint g = 0;
      EGLint b = 0;
      eglGetConfigAttrib(_display, config, EGL_RED_SIZE, &r);
      eglGetConfigAttrib(_display, config, EGL_RED_SIZE, &g);
      eglGetConfigAttrib(_display, config, EGL_RED_SIZE, &b);

      if (r == red_size && g == green_size && b == blue_size) {
        return config;
      }
    }
  }
}
