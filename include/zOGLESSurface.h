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

#ifndef ZOGLESSURFACE_H__
#define ZOGLESSURFACE_H__

#include "zCommon.h"

class zLogger;
class zWin;
class zOGLESRuntime;

#include <EGL/egl.h>

class zOGLESSurface {
  friend class zOGLESRuntime;

protected:
  zLogger* _logger;
  EGLDisplay _display;
  EGLSurface _surface;

public:

protected:
  zOGLESSurface(void);
  virtual ~zOGLESSurface(void);

protected:
  bool create(EGLDisplay display, EGLConfig config, EGLNativeWindowType native_win);
};

#endif // ZOGLESSURFACE_H__

