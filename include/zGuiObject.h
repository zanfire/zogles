/******************************************************************************
 * Copyright 2012 Matteo Valdina
 *      
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *      
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef ZGUIOBJECT_H__
#define ZGUIOBJECT_H__

#include "zCommon.h"
#include "zArray.h"
#include "zRect.h"
#include "zObject.h"

class zWin;
class zLogger;

class zGuiObject : public zObject {
protected:
  zLogger* _logger;
  zWin* _win;
  int _width;
  int _height;
  zRect _padding;
  zGuiObject* _parent;
  zArray<zGuiObject*> _children;
  bool _is_enabled;
  bool _is_visible;

public:
  zGuiObject(zWin* win);

  bool init(void);
  /// Compute the size of the element.
  bool layout(zRect const& area);
  /// Compute the rendering.
  bool render(void);

  void set_padding(zRect const& p) { _padding = p; }
  void set_width(int w) { _width = w; }
  void set_height(int h) { _height = h; }

  // TODO: Add mutex to protect from the rendering thread access and update.
  bool set_enabled(bool b) { _is_enabled = b; }
  bool set_visible(bool b) { _is_visible = b; }

  bool is_enabled(void) const { return _is_enabled; }
  bool is_visible(void) const { return _is_visible; }

  bool add_child(zGuiObject* child);

  zGuiObject* get_parent(void) const { return _parent; }
  zGuiObject* get_child_at(int index) const;
  int get_child_count(void) const;

  zRect get_padding() { return _padding; }
  int get_width() { return _width; }
  int get_height() { return _height; }


protected:
  virtual ~zGuiObject(void);

  /*** zGuiObject interface **/
  virtual void impl_init(void) = 0;
  /// This method is called to compute the size if the object.
  /// NOTE: Needs to compute the object size.
  virtual void impl_layout(zRect const& area) = 0;
  /// This method is called to render the object.
  virtual void impl_render(void) = 0;
};

#endif // ZGUIOBJECT_H__
