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

#ifndef ZWIN_H__
#define ZWIN_H__

#include "zCommon.h"
#include "zRect.h"
#include "zRunnable.h"

#include <EGL/egl.h>

class zThread;
class zLogger;

#if defined(_WIN32)
# define ZWIN_ID HWND
#else
#endif

class zWin : zRunnable {
  friend class zWinFactory;

protected:
  zLogger* _logger;
  ZWIN_ID _id;
  zThread* _thread;
public:
  /// Returns the window ID. The window ID is the system dependent handle.
  ZWIN_ID get_id(void) const { return _id; }

  bool destroy(void);

  bool open(void);
  void close(void);

  /// Returns the EGL display for the window.
  EGLDisplay get_display(void);

protected:
  zWin(void);
  virtual ~zWin(void);
    
  void start(void);
  /// Body of the thread that handle this window.
  virtual int run(void* param);

  /*** Interface that derivation class must implements. **/

  virtual bool peek_message(void) = 0;

  void render(void);

  /// Create the window.
  virtual void impl_create(zRect const& pos) = 0;
  /// Get the EGL display.
  virtual EGLDisplay impl_get_display(void) = 0;
};

#endif // ZWIN_H__

