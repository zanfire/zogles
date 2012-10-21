#include "zGuiStack.h"

#include "zLogger.h"
#include "zWin.h"

#include <EGL/egl.h>

zGuiStack::zGuiStack(zWin* win) : zGuiObject(win) {
  _orientation = ORIENTATION_HORIZONTAL;
}


zGuiStack::~zGuiStack(void) {
}


void zGuiStack::impl_layout(zRect const& area) {
  // Compute position for each child as a stack layout.


  int x = area.left;
  int y = area.top;

  // Compute children layout.
  for (int i = 0; i < _children.get_count(); i++) {
    zGuiObject* obj = NULL;
    if (_children.get(i, &obj)) {
      int w = obj->get_padding().left + obj->get_padding().right + obj->get_width();
      int h = obj->get_padding().top + obj->get_padding().bottom + obj->get_height();

      obj->layout(zRect(x, y, w, h));

      if (_orientation == ORIENTATION_HORIZONTAL) {
        x += w;
      }
      else {
        y += h;
      }
    }
  }
}


void zGuiStack::impl_render(void) {
}

