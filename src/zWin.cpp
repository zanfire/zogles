#include "zWin.h"

#include "zLogger.h"
#include "zThread.h"

#include "zOGLESContext.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


zWin::zWin(void) {
  _logger = zLogger::get_logger("zWin");
  _id = 0;
  _thread = new zThread(this);
}


zWin::~zWin(void) {
}


void zWin::start(void) {
  _thread->start();
}


int zWin::run(void* param) {
  impl_create(zRect(10, 10, 210, 210));
  
  zOGLESContext* contex = new zOGLESContext();

  if (!contex->create(this)) {
    _logger->error("Failed to create context.");
  }

  eglMakeCurrent(get_display(), contex->get_surface(), contex->get_surface(), contex->get_context());

  _logger->info("OpenGL ES Version = %s", glGetString(GL_VERSION));
  _logger->info("OpenGL ES Vendor = %s", glGetString(GL_VENDOR));
  _logger->info("OpenGL ES Renderer = %s", glGetString(GL_RENDERER));
  _logger->info("OpenGL ES Extensions = %s", glGetString(GL_EXTENSIONS));

  for (;;) {
    while(peek_message()) {
      // Looping on message handling.
    }

    render();

    // Push the EGL surface color buffer to the native window. Causes the rendered graphics to be displayed on screen.
    eglSwapBuffers(get_display(), contex->get_surface()); 

    zThread::sleep(66); // TODO compute delta.
  }
  //impl_destroy();
}

bool zWin::destroy(void) {
  
  return false;
}


EGLDisplay zWin::get_display(void) {
  EGLDisplay display = impl_get_display();
  if (display == EGL_NO_DISPLAY) {
    EGLint error = eglGetError();
    _logger->error("Failed to get EGL Display, eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
  }
  return display;
}


void zWin::render(void) {
  // Color background
    // Only one color, revert to standard glClear for better performance
  //glDisable(GL_BLEND);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Should invoke child rendering method.
}
