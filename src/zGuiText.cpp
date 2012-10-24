#include "zGuiText.h"

#include "zLogger.h"
#include "zWin.h"

#include "zOGLESFragmentShaderProgram.h"
#include "zOGLESRuntime.h"
#include "zFontFT.h"
#include "zFontGlyph.h"

zGuiText::zGuiText(zWin* win) : zGuiObject(win), _glyphs(NO, 1024) {
  _font = new zFontFT();
  _position_vertexs = NULL;
  _program = NULL;
  _coord = -1;
  _uniform_tex = -1;
  _uniform_color = -1;
}


zGuiText::~zGuiText(void) {
  if (_program != NULL) {
    delete _program;
    _program = NULL;
  }
  _font->release_reference();
}

void zGuiText::impl_init(void) {
  _font->load_file("C:\\Windows\\Fonts\\GARABD.TTF");


  _width = 0;
  _height = 200; //48 << 6;
  for (int i = 0; i < _text.get_length(); i++) {
    zFontGlyph* glyph = _font->load_bitmap((unsigned long)(_text.get_buffer()[i]), 48);

    // Move the draw position
    _width += (glyph->get_advance_x() >> 6);
    //y += (glyph->get_advance_y() >> 6) * sy;
    _glyphs.append(glyph);
  }
  _position_vertexs = (GLfloat*) malloc(sizeof(GLfloat) * 16 * _glyphs.get_count());

  // TODO: de-init
  _program = new zOGLESFragmentShaderProgram();

  static const char vertex[] =
    "attribute vec4 coord;"
    "varying vec2 texcoord;"
    "void main() {"
    "  gl_Position = vec4(coord.xy, 0, 1);"
    "  texcoord = coord.zw;"
    "}";
  static const char fragment[] =
    "precision mediump float;"
    "varying vec2 texcoord;"
    "uniform sampler2D tex;"
    "uniform vec4 color;"
    "void main() {"
    "  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color;"
    "}";

  _program->create(vertex, fragment);

  _coord = _program->get_attrib_location("coord");
  _uniform_tex = _program->get_uniform_location("tex");
  _uniform_color =  _program->get_uniform_location("color");

  glGenBuffers(1, &_vbo);
}


void zGuiText::impl_layout(zRect const& area) {
  float win_w = (float)_win->get_width();
  float win_h = (float)_win->get_height();
  float x = -1.0f + area.left / win_w + _padding.left / win_w;
  float y = -1.0f + area.top / win_h + _padding.top / win_h;
  
  for (int i = 0; i < _glyphs.get_count(); i++) {
    zFontGlyph* glyph = NULL;
    if (_glyphs.get(i, &glyph)) {
      float rel_w = glyph->get_bitmap_width() * (2.0f / win_w);
      float rel_h = glyph->get_bitmap_height() * (2.0f / win_h);
      GLfloat* ptr = _position_vertexs;
      GLfloat vertexs[] = { x,          y,         0, 0,
                            x + rel_w,  y,         1, 0,
                            x,          y + rel_h, 0, 1,
                            x + rel_w,  y + rel_h, 1, 1 };
      /*GLfloat vertexs[] = { x,          y,       
                            x + rel_w,  y,       
                            x,          y + rel_h, 
                            x + rel_w,  y + rel_h };
                            */
      memcpy(ptr, &vertexs, sizeof(vertexs));
      // 
      ptr = ptr + 16;
      x += (glyph->get_advance_x() >> 6) * (2.0f / win_w);
    }
  }

}


void zGuiText::impl_render(void) {
  _program->use();
  

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &_tex);  
  glBindTexture(GL_TEXTURE_2D, _tex);
  glUniform1i(_uniform_tex, 0);

  GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
  glUniform4fv(_uniform_color, 1, red);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER , GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER , GL_LINEAR);


  glEnableVertexAttribArray(_coord);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
  GLfloat* pos = _position_vertexs;
  for (int i = 0; i < _text.get_length(); i++) {
    zFontGlyph* glyph = NULL;
    if (_glyphs.get(i, &glyph)) {

      glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyph->get_bitmap_width(), glyph->get_bitmap_height(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->get_bitmap());
      zOGLESRuntime::check_op_gl(_logger, "glTexImage2D");

    
      glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, pos, GL_DYNAMIC_DRAW);
      zOGLESRuntime::check_op_gl(_logger, "glBufferData");
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      zOGLESRuntime::check_op_gl(_logger, "glDrawArrays");

      pos += 8;
    }
  }
  glDisableVertexAttribArray(_coord);
  glDeleteTextures(1, &_tex);

  glDisable(GL_BLEND);
}
