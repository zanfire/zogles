#include "zGuiRect.h"

#include "zLogger.h"
#include "zWin.h"

#include "zOGLESFragmentShaderProgram.h"
#include "zOGLESRuntime.h"

zGuiRect::zGuiRect(zWin* win) : zGuiObject(win) {
  _program = NULL;
  _position = -1;
  _color = -1;
}


zGuiRect::~zGuiRect(void) {
  if (_program != NULL) {
    delete _program;
    _program = NULL;
  }
}

void zGuiRect::impl_init(void) {
  // TODO: de-init
  _program = new zOGLESFragmentShaderProgram();

  static const char vertex[] =
    "attribute vec2 position;"
    "attribute vec4 color;"
    "varying vec4 colorVarying;"
    "void main() {"
    "  gl_Position = vec4(position.xy, 0, 1);"
    "  colorVarying = color;"
    "}";
  static const char fragment[] =
    "varying lowp vec4 colorVarying;"
    "void main() {"
    "  gl_FragColor = colorVarying;"
    "}";

  _program->create(vertex, fragment);

  _position = _program->get_attrib_location("position");
  _color = _program->get_attrib_location("color");

  glEnableVertexAttribArray(_position);
  zOGLESRuntime::check_op_gl(_logger, "glEnableVertexAttribArray position");
  glEnableVertexAttribArray(_color);
  zOGLESRuntime::check_op_gl(_logger, "glEnableVertexAttribArray color");

}


void zGuiRect::impl_layout(zRect const& area) {
  float win_w = (float)_win->get_width();
  float win_h = (float)_win->get_height();
  float x = -1.0f + area.left / win_w + _padding.left / win_w;
  float y = -1.0f + area.top / win_h + _padding.top / win_h;
  float rel_w = _width / win_w;
  float rel_h = _height / win_h;

  GLfloat vertexs[] = { x,          y,       
                        x + rel_w,  y,       
                        x,          y + rel_h, 
                        x + rel_w,  y + rel_h };
  memcpy(&_position_vertexs, &vertexs, sizeof(vertexs));
}


void zGuiRect::impl_render(void) {
  GLfloat colorVertexs[] = { 1.0f, 0.0f, 0.0f, _alpha, 
                             1.0f, 0.0f, 0.0f, _alpha,
                             1.0f, 0.0f, 0.0f, _alpha,
                             1.0f, 0.0f, 0.0f, _alpha };
  _program->use();

 
  
  glVertexAttribPointer(_position, 2, GL_FLOAT, GL_FALSE, 0, &_position_vertexs);
  zOGLESRuntime::check_op_gl(_logger, "glVertexAttribPointer positiion");
  glVertexAttribPointer(_color, 4, GL_FLOAT, GL_FALSE, 0, &colorVertexs);
  zOGLESRuntime::check_op_gl(_logger, "glVertexAttribPointer color");

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  zOGLESRuntime::check_op_gl(_logger, "glDrawArrays");

  //glDisableVertexAttribArray(_color);
  //glDisableVertexAttribArray(_position);
}
