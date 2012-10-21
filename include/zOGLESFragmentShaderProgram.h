#ifndef ZOGLESFRAGMENTSHADERPROGRAM_H__
#define ZOGLESFRAGMENTSHADERPROGRAM_H__

#include <GLES2/gl2.h>

class zLogger;

class zOGLESFragmentShaderProgram {
protected:
  zLogger* _logger;
  GLuint _id;
  GLuint _vertex;
  GLuint _fragment;

public:
  zOGLESFragmentShaderProgram(void);
  virtual ~zOGLESFragmentShaderProgram(void);

  // Get the program ID
  unsigned int get_id(void) { return _id; }
  /// Creates program.
  bool createProgram(char const* vertex, char const* fragment);
  /// Use program.
  bool useProgram(void);

  GLint get_attrib_location(char const* name);

protected:
  /// Load specific type of shader.
  GLuint loadShader(GLuint shaderType, char const* src);
};

#endif // ZOGLESFRAGMENTSHADERPROGRAM_H__
