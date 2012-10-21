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

#ifndef ZOGLESRUNTIME_H__
#define ZOGLESRUNTIME_H__

#include "zCommon.h"
#include "zRect.h"
#include "zWin.h"
#include "zString.h"
#include "zArray.h"
#include "zEvent.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

class zLogger;
class zOGLESConfigs;
class zOGLESSurface;
class zOGLESContext;

class zOGLESRuntime {
protected:
  static zMutex* _singleton_mtx;
  static zOGLESRuntime* _singleton;

  zLogger* _logger;
  EGLDisplay _display;
  zOGLESConfigs* _configs;
  zOGLESSurface* _surface;
  zOGLESContext* _context;

public:
  static zOGLESRuntime* get_instance(void);
  /// Releases all resources. After this method the zWinFactory cannot be used.
  /// NOTE: This method MUST be called when application is closing.
  static void shutdown(void);

  bool init(zWin* win);
  
  zOGLESConfigs* get_configs(void);
  zOGLESSurface* get_surface(void) { return _surface; }
  zOGLESContext* get_context(void) { return _context; }

  // Utils methods
  static bool check_op_egl(zLogger* logger, char const* op);
  static bool check_op_gl(zLogger* logger, char const* op);

  static char const* conv_gl_error_to_chars(GLint error);
protected:
  zOGLESRuntime(void);
  virtual ~zOGLESRuntime(void);

};

#endif // ZOGLESRUNTIME_H__

