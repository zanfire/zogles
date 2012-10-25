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

#ifndef ZGUIRECT_H__
#define ZGUIRECT_H__

#include "zCommon.h"
#include "zGuiObject.h"

#include <GLES2/gl2.h>

class zLogger;
class zWin;
class zOGLESFragmentShaderProgram;

class zGuiRect : public zGuiObject {
protected:
  zOGLESFragmentShaderProgram* _program;
  GLint _position;
  GLint _color;
  GLfloat _position_vertexs[4 * 2];
  
public:
  zGuiRect(zWin* win);

protected:
  /// Protected because must be delete by zObject::release_reference.
  virtual ~zGuiRect(void);

  virtual void impl_init(void);
  virtual void impl_layout(zRect const& area);
  virtual void impl_render(void);
};

#endif // ZGUIRECT_H__

