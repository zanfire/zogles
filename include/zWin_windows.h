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

#ifndef ZWIN_WINDOWS_H__
#define ZWIN_WINDOWS_H__

#include "zCommon.h"
#include "zWin.h"

#include <Windows.h>

class zWinListener;

class zWin_windows : public zWin {
public:
  HDC _deviceContext;

public:
  zWin_windows(zWinFactory* factory, zWinListener* listener);
  virtual ~zWin_windows(void);

  
  /// Handle a Windwo message.
  LRESULT handle_message(UINT uiMsg, WPARAM wParam, LPARAM lParam);
protected:
  /// Peek a message from the thread queue. returns true if processing is needed false otherwise.
  virtual bool peek_message(void);


  virtual void impl_create(zRect const& pos);
  EGLDisplay impl_get_display(void);
};

#endif // ZWIN_H__

