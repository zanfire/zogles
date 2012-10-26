#include "zWin.h"

#include "zLogger.h"
#include "zThread.h"
#include "zMutex.h"

#include "zWinFactory.h"
#include "zGuiObject.h"
#include "zGuiStack.h"
#include "zGuiText.h"
#include "zOGLESRuntime.h"
#include "zOGLESConfigs.h"
#include "zOGLESContext.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


zWin::zWin(zWinFactory* factory) {
  _logger = zLogger::get_logger("zWin");
  _mtx = new zMutex();
  _id = 0;
  _thread = new zThread(this);
  _root = NULL;
  _request_close = false;
  _factory = factory;

  _invalidate_pos = true;
  _pos = zRect(0,0,0,0);
}


zWin::~zWin(void) {
  delete _mtx;
  delete _thread;
  _logger->release_reference();
}


void zWin::start(void) {
  _thread->start();
}

#include "zGuiRect.h"

int zWin::run(void* param) {
  impl_create(zRect(10, 10, 600, 600));

  zOGLESRuntime* runtime = zOGLESRuntime::get_instance();
  
  runtime->init(this);
  zOGLESContext* contex = runtime->get_context();

  contex->make_current();
  // Load gui objects

  _root = new zGuiStack(this);

  for (int i = 0; i < 2; i++) {
    zGuiObject* o = new zGuiRect(this);
    o->set_width(20);
    o->set_height(20);
    o->set_padding(zRect(10, 10, 10, 10));
    _root->add_child(o);
    o->release_reference();
  }
  
  for (int i = 0; i < 1; i++) {
    zGuiText* o = new zGuiText(this);
    o->set_text("Hello world!");
    o->set_font_size(20);
    o->set_padding(zRect(10, 10, 10, 10));
    _root->add_child(o);
    o->release_reference();
  }

  for (int i = 0; i < 2; i++) {
    zGuiObject* o = new zGuiRect(this);
    o->set_width(20);
    o->set_height(20);
    o->set_padding(zRect(10, 10, 10, 10));
    _root->add_child(o);
    o->release_reference();
  }

  ((zGuiStack*)_root)->set_orientation(zGuiStack::ORIENTATION_VERTICAL);
  _root->init();


  while (!_request_close) {
    while(peek_message()) {
      // Looping on message handling.
    }

    if (_invalidate_pos) {
      glViewport(0, 0, _pos.width(), _pos.height());
      _invalidate_pos = false;
      _root->layout(zRect(0, 0, _pos.width(), _pos.height()));
    }

    // Render
    contex->make_current();
    render();
    contex->swap_buffers();

    zThread::sleep(66); // TODO compute delta.
  }
  _root->release_reference();
  //impl_destroy();
  _factory->on_window_destroy(this);
  zOGLESRuntime::shutdown();
  return 0;
}


THREAD_ID zWin::get_loop_thread_id(void) const {
  return _thread->get_thread_id();
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
  glDisable(GL_DITHER);
  glDisable(GL_DEPTH_TEST);

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
  if (_root != NULL) {
    _root->render();
  }
}


void zWin::handle_size(int w, int h) {
  _logger->debug("Updating window size to %dx%d.", w, h);
  // Check thread!!!
  _invalidate_pos = true;
  _pos.right = _pos.left + w;
  _pos.bottom = _pos.top + h;
}
