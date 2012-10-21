#include "zGuiObject.h"

#include "zLogger.h"
#include "zThread.h"
#include "zWin.h"


zGuiObject::zGuiObject(zWin* win) : _children(YES, 10) {
  _logger = zLogger::get_logger("zGuiObject");
  _win = win;
  _parent = NULL;
  _is_enabled = true;
  _is_visible = true;

  _width = -1;
  _height = -1; 
}


zGuiObject::~zGuiObject(void) {
  while (_children.get_count() > 0) {
    zGuiObject* obj = NULL;
    if (_children.remove(0, &obj)) {
      obj->release_reference();
    }
  }

  _logger->release_reference();
}


bool zGuiObject::init(void) {
  impl_init();

  for (int i = 0; i < _children.get_count(); i++) {
    zGuiObject* obj = NULL;
    if (_children.get(i, &obj)) {
      obj->init();
    }
  }
  return true;
}


bool zGuiObject::layout(zRect const& area) {
  // Check thread
  if (zThread::get_current_thread_id() != _win->get_loop_thread_id()) {
    _logger->error("Layout can be computed only in the GUI thread.");
    return false;
  }
  // Compute first my layout.
  impl_layout(area);
  return true;
}


bool zGuiObject::render(void) {
    // Check thread
  if (zThread::get_current_thread_id() != _win->get_loop_thread_id()) {
    _logger->error("Layout can be computed only in the GUI thread.");
    return false;
  }
  
  impl_render();
  
  for (int i = 0; i < _children.get_count(); i++) {
    zGuiObject* obj = NULL;
    if (_children.get(i, &obj)) {
      obj->render();
    }
  }
  return true;
}


bool zGuiObject::add_child(zGuiObject* child) {
  if (child == this) {
    _logger->error("An zGuiObject instance cannot be added to itself.");
    return false;
  }
  if (child->_parent != NULL) {
    //_logger->error("zGuiObject \"\" has just a parent, this operation is not permitted.", child->to_string().get_buffer());
    return false;
  }
  child->_parent = this;
  child->acquire_reference();
  return _children.append(child);
}


zGuiObject* zGuiObject::get_child_at(int index) const {
  zGuiObject* obj = NULL;
  if (_children.get(index, &obj)) {
    return obj;
  }
  return NULL;
}


int zGuiObject::get_child_count(void) const {
  return _children.get_count();
}
