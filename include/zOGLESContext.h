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

#ifndef ZOGLESCONTEXT_H__
#define ZOGLESCONTEXT_H__

#include "zCommon.h"

#include <EGL/egl.h>


class zLogger;
class zWin;

class zOGLESContext {
protected:
  zLogger* _logger;
  EGLDisplay _display;
  EGLSurface _surface;
  EGLContext _context;
  
public:
  zOGLESContext(void);
  virtual ~zOGLESContext(void);

  bool create(EGLDisplay display, EGLConfig config, EGLSurface surface);
  void destroy(void);

  EGLSurface get_surface(void) { return _context; };
  EGLContext get_context(void) { return _surface; };

  void make_current(void);
  void swap_buffers(void);
};

#endif // ZOGLESCONTEXT_H__
