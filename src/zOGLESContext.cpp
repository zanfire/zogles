#include "zOGLESContext.h"

#include "zLogger.h"
#include "zWin.h"

#define LOGE _logger->error

zOGLESContext::zOGLESContext(void) {
  _logger = zLogger::get_logger("zOGLESContext");
  _surface = EGL_NO_SURFACE;
  _context = EGL_NO_CONTEXT;
}


zOGLESContext::~zOGLESContext(void) {
}


bool zOGLESContext::create(zWin* win) {
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

  EGLint contextAttributes[] =
  {
    EGL_CONTEXT_CLIENT_VERSION, 0,
    EGL_NONE
  };

  ///Using the defaults (EGL_RENDER_BUFFER = EGL_BACK_BUFFER).
  EGLint windowAttributes[] =
  {
    EGL_NONE
    // Uncomment and remove EGL_NONE above to specify EGL_RENDER_BUFFER value to eglCreateWindowSurface.
    // EGL_RENDER_BUFFER,     EGL_BACK_BUFFER,
    // EGL_NONE
  };



  EGLDisplay display = win->get_display();

  if (display == EGL_NO_DISPLAY) {
    return false;
  }

  /* Initialize EGL. */
  EGLBoolean success = eglInitialize(display, NULL, NULL);
  if (success != EGL_TRUE) {
    EGLint error = eglGetError();
    _logger->error("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
    _logger->error("Failed to initialize EGL at %s:%i\n", __FILE__, __LINE__);
    return false;
  }
  EGLConfig *configsArray = NULL;
  EGLint numberOfConfigs = 0;

  configAttributes[15] = EGL_OPENGL_ES2_BIT;
  contextAttributes[1] = 2;
        
  // Disable AntiAliasing, useful if it's not supported on a platform.
  if (true) {
    configAttributes[1] = EGL_DONT_CARE;
  }

  // Enumerate available EGL configurations which match or exceed our required attribute list.
  success = eglChooseConfig(display, configAttributes, NULL, 0, &numberOfConfigs);
  if (success != EGL_TRUE) {
    EGLint error = eglGetError();
    _logger->error("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
    _logger->error("Failed to enumerate EGL configs at %s:%i.", __FILE__, __LINE__);
    return false;
  }

  _logger->info("Number of configs found is %d\n", numberOfConfigs);

  // Allocate space for all EGL configs available and get them.
  configsArray = (EGLConfig *)calloc(numberOfConfigs, sizeof(EGLConfig));
  //success = eglChooseConfig(display, configAttributes, configsArray, numberOfConfigs, &numberOfConfigs);
  success = eglGetConfigs(display, configsArray, numberOfConfigs, &numberOfConfigs);
  if(success != EGL_TRUE)
  {
    EGLint error = eglGetError();
    _logger->error("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
    _logger->error("Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
    return false;
  }

  // Loop through the EGL configs to find a color depth match.
  // Note: This is necessary, since EGL considers a higher color depth than requested to be 'better'
  // even though this may force the driver to use a slow color conversion blitting routine. 
  bool matchFound = false;
  int matchingConfig = -1;
  for(int configsIndex = 0; (configsIndex < numberOfConfigs) && !matchFound; configsIndex++)
  {
      EGLint attributeValue = 0;

      success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_RED_SIZE, &attributeValue);
      if(success != EGL_TRUE)
      {
          EGLint error = eglGetError();
          LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
          LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
          return false;
      }

      if(attributeValue == 8)
      {
          success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_GREEN_SIZE, &attributeValue);
          if(success != EGL_TRUE)
          {
              EGLint error = eglGetError();
              LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
              LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
              return false;
          }

          if(attributeValue == 8)
          {
              success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_BLUE_SIZE, &attributeValue);
              if(success != EGL_TRUE)
              {
                  EGLint error = eglGetError();
                  LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
                  LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
                  return false;
              }

              if(attributeValue == 8) 
              {
                  matchFound = true;
                  matchingConfig = configsIndex;
              }
          }
      }
  }
  if(!matchFound)
  {
      LOGE("Failed to find matching EGL config at %s:%i\n", __FILE__, __LINE__);
      return false;
  }
  // Copy the configsArray to our static variables.
  EGLConfig config = configsArray[matchingConfig];


  // Create a surface.
  _surface = eglCreateWindowSurface(display, configsArray[matchingConfig], (EGLNativeWindowType)(win->get_id()), windowAttributes);
  if(_surface == EGL_NO_SURFACE)
  {
      EGLint error = eglGetError();
      LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
      LOGE("Failed to create EGL surface at %s:%i\n", __FILE__, __LINE__);
      return false;
  }

  // Unconditionally bind to OpenGL ES API as we exit this function, since it's the default.
  eglBindAPI(EGL_OPENGL_ES_API);

  _context = eglCreateContext(display, configsArray[matchingConfig], EGL_NO_CONTEXT, contextAttributes);
  if(_context == EGL_NO_CONTEXT)
  {
      EGLint error = eglGetError();
      LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
      LOGE("Failed to create EGL context at %s:%i\n", __FILE__, __LINE__);
      return false;
  }

  free(configsArray);
  configsArray = NULL;
  return true;
}


void zOGLESContext::destroy(void) {

}
