#include "zOGLESFragmentShaderProgram.h"

#include "zOGLESRuntime.h"
#include "zLogger.h"

zOGLESFragmentShaderProgram::zOGLESFragmentShaderProgram(void) {
  _logger = zLogger::get_logger("zOGLESFragmentShaderProgram");
  _id = glCreateProgram();
  _vertex = 0;
  _fragment = 0;
  // Allocate program. 

  _logger->debug("Created program %u.", _id);
}


zOGLESFragmentShaderProgram::~zOGLESFragmentShaderProgram(void) {
  if (_id != 0) {
     glDeleteProgram(_id);
     _logger->debug("Deleted program %u.", _id);
  }
  _logger->release_reference();
} 


bool zOGLESFragmentShaderProgram::createProgram(char const* vertex, char const* fragment) {
  if (_id == 0) {
    _logger->error("Failed to create program because the program isn't initialized correctly.");
    return false;
  }

  _vertex = loadShader(GL_VERTEX_SHADER, vertex);
  if (_vertex == 0) {
    _logger->error("Failed to load vertex shader in program %u.", _id);
    return false;
  }

  _fragment = loadShader(GL_FRAGMENT_SHADER, fragment);

  if (_fragment == 0) {
    // TODO: Handle unload of _vertex shader.
    _logger->error("Failed to load fragment shader %u.", _id);
    return false;
  }

  glAttachShader(_id, _vertex);
  glAttachShader(_id, _fragment);
  glLinkProgram(_id);

  GLint result = GL_FALSE;
  glGetProgramiv(_id, GL_LINK_STATUS, &result);
  if (result != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
      // Get error log for the link fase
      char* buf = (char*) malloc(length);
      glGetProgramInfoLog(_id, length, NULL, buf);
      _logger->error("Could not link program:%s", buf);
      free(buf);
    }
    return false;
  }
  return true;
}


bool zOGLESFragmentShaderProgram::useProgram(void) {
  glUseProgram(_id);
  return zOGLESRuntime::check_op_gl(_logger, "glUseProgram");
}

GLint zOGLESFragmentShaderProgram::get_attrib_location(char const* name) {
  // TODO: Logging
  if (_id != 0) {
    return glGetAttribLocation(_id, name);
  }
  return 0;
}



GLuint zOGLESFragmentShaderProgram::loadShader(GLuint type, const char* src) {
  GLuint shader = glCreateShader(type);
  if (shader != 0) {
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      // ERROR compiling shader.
      GLint lenght = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);
      if (lenght > 0) {
        char* buf = (char*)malloc(lenght);
        glGetShaderInfoLog(shader, lenght, NULL, buf);
        _logger->error("Failed to compile shader type %u: \"%s\"", type, buf);
        free(buf);
        glDeleteShader(shader);
        shader = 0;
      }
    }
  }
  return shader;
}
