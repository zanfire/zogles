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

#ifndef ZGUITEXT_H__
#define ZGUITEXT_H__

#include "zCommon.h"
#include "zGuiObject.h"
#include "zString.h"
#include "zArray.h"

#include <GLES2/gl2.h>

class zLogger;
class zWin;
class zFontFT;
class zFontGlyph;
class zOGLESFragmentShaderProgram;

class zGuiText : public zGuiObject {
protected:
  zFontFT* _font;
  zString _text;
  int _font_size;
  zArray<zFontGlyph*> _glyphs;

  zOGLESFragmentShaderProgram* _program;
  GLint _coord;
  GLint _uniform_tex;
  GLint _uniform_color;
  GLfloat* _position_vertexs;

  GLuint _tex;
  GLuint _vbo;
  
public:
  zGuiText(zWin* win);

  void set_font_size(int size) { _font_size = size; };
  void set_text(zString const& text) { _text = text; }
  zString get_text(void) { return _text; }

protected:
  /// Protected because must be delete by zObject::release_reference.
  virtual ~zGuiText(void);

  virtual void impl_init(void);
  virtual void impl_layout(zRect const& area);
  virtual void impl_render(void);
};

#endif // ZGUITEXT_H__

