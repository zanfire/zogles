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

#ifndef ZGUISTACK_H__
#define ZGUISTACK_H__

#include "zCommon.h"
#include "zGuiObject.h"

class zLogger;
class zWin;

class zGuiStack : public zGuiObject {
public:
  enum Orientation {
    ORIENTATION_VERTICAL = 0,
    ORIENTATION_HORIZONTAL
  };

protected:
    Orientation _orientation;
public:
  zGuiStack(zWin* win);
  
  void set_orientation(Orientation orientation) { _orientation = orientation; }

protected:
  /// Protected because must be delete by zObject::release_reference.
  virtual ~zGuiStack(void);

  virtual void impl_init(void) {}
  virtual void impl_layout(zRect const& area);
  virtual void impl_render(void);
};

#endif // ZGUISTACK_H__

