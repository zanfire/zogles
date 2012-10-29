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

#ifndef ZGUIABSLAYOUT_H__
#define ZGUIABSLAYOUT_H__

#include "zCommon.h"
#include "zGuiObject.h"

class zLogger;
class zWin;

/// This class is a container of zGuiObject. 
/// Each obj is placed in base of position information.
class zGuiAbsLayout : public zGuiObject {
public:
  enum Edge {
    EDGE_LEFT_TOP,
    EDGE_RIGHT_TOP,
    EDGE_LEFT_BOTTOM,
    EDGE_RIGHT_BOTTOM
  };

protected:
  struct AbsLayoutPosition {
    zGuiObject* obj;
    float x;
    float y;
    Edge edge;
  };

  zArray<AbsLayoutPosition> _children_positions;

public:
  zGuiAbsLayout(zWin* win);
  
  bool add(zGuiObject* obj, float x, float y, Edge edge);

protected:
  /// Protected because must be delete by zObject::release_reference.
  virtual ~zGuiAbsLayout(void);

  virtual void impl_init(void) {}
  virtual void impl_layout(zRect const& area);
  virtual void impl_render(void);

  // Should HIDE normal ADD!!!
};

#endif // ZGUISTACK_H__

