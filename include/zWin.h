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
#include "zThread.h"
#include "zObject.h"
#include "zRunnable.h"
#include "zColor.h"

#include <EGL/egl.h>

class zThread;
class zLogger;
class zGuiObject;
class zWinListener;

#if defined(_WIN32)
# define ZWIN_ID HWND
#else
#endif

class zWin : public zRunnable, public zObject {
  
  friend class zWinFactory;

protected:
  zLogger* _logger;
  zWinFactory* _factory;
  ZWIN_ID _id;
  zMutex* _mtx;
  zThread* _thread;
  zWinListener* _listener;
  bool _request_close;
  zGuiObject* _root;
  zSolidColor _background_color;
  
  bool _invalidate_pos;
  zRect _pos;
public:
  /// Returns the window ID. The window ID is the system dependent handle.
  ZWIN_ID get_id(void) const { return _id; }
  /// Returns the thread ID of the thread that handle this window.
  THREAD_ID get_loop_thread_id(void) const;
  zMutex* get_mutex(void) const { return _mtx; }
 
  /// Returns the EGL display for the window.
  EGLDisplay get_display(void);

  int get_width(void) const { return _pos.width(); }
  int get_height(void) const { return _pos.height(); }

  void set_background_color(zSolidColor const& color) { _background_color = color; }

protected:
  zWin(zWinFactory* factory, zWinListener* listener);
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

  // Callback
  void handle_size(int w, int h);

};

#endif // ZWIN_H__

