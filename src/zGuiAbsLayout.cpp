#include "zGuiAbsLayout.h"

#include "zLogger.h"
#include "zWin.h"

zGuiAbsLayout::zGuiAbsLayout(zWin* win) : zGuiObject(win), _children_positions(YES, 32) {
}


zGuiAbsLayout::~zGuiAbsLayout(void) {
}


bool zGuiAbsLayout::add(zGuiObject* obj, float x, float y, zGuiAbsLayout::Edge edge) {
  if (add_child(obj)) {
    AbsLayoutPosition pos;
    pos.obj = obj;
    pos.x = (x > 1.0f) ? 1.0f : x;
    pos.y = (y > 1.0f) ? 1.0f : y;
    pos.edge = edge;
    _children_positions.append(pos);
    return true;
  }
  return false;
}


void zGuiAbsLayout::impl_layout(zRect const& area) {
  // Compute position for each child.

  int base_x = area.left;
  int base_y = area.top;

  // Compute children layout.
  for (int i = 0; i < _children_positions.get_count(); i++) {
    AbsLayoutPosition pos;
    if (_children_positions.get(i, &pos)) {
      zGuiObject* obj = pos.obj;
      int w = obj->get_padding().left + obj->get_padding().right + obj->get_width();
      int h = obj->get_padding().top + obj->get_padding().bottom + obj->get_height();

      int x = base_x;
      int y = base_y;
      if (pos.edge == EDGE_LEFT_TOP) {
        x += (int)(area.width() * pos.x);
        y += area.height() - (int)(area.height() * pos.y) - h;
      }
      else if (pos.edge == EDGE_RIGHT_TOP) {
        x += area.width() - (int)(area.width() * pos.x) - w;
        y += area.height() - (int)(area.height() * pos.y) - h;
      }
      else if (pos.edge == EDGE_RIGHT_BOTTOM) {
        x += area.width() - (int)(area.width() * pos.x) - w;
        y += (int)(area.height() * pos.y);
      }
      else if (pos.edge == EDGE_LEFT_BOTTOM) {
        x += (int)(area.width() * pos.x);
        y += (int)(area.height() * pos.y);
      }

      if (obj->get_horizontal_align() == HORIZONTAL_ALIGN_LEFT) {
        // nop
      }
      else if (obj->get_horizontal_align() == HORIZONTAL_ALIGN_CENTER) {
        x -= w / 2;
      }
      else if (obj->get_horizontal_align() == HORIZONTAL_ALIGN_RIGHT) {
        x -= w;
      }

      if (obj->get_vertical_align() == VERTICAL_ALIGN_TOP) {
        y -= h;
      }
      else if (obj->get_vertical_align() == VERTICAL_ALIGN_CENTER) {
        y -= h / 2;
      }
      else if (obj->get_vertical_align() == VERTICAL_ALIGN_BOTTOM) {
        // nop
      }
      //_vertical_align = VERTICAL_ALIGN_CENTER;

      obj->layout(zRect(x, y, w, h));
    }
  }
}


void zGuiAbsLayout::impl_render(void) {
}

