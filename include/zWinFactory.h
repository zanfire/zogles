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

#ifndef ZWINFACTORY_H__
#define ZWINFACTORY_H__

#include "zCommon.h"
#include "zRect.h"
#include "zWin.h"
#include "zString.h"
#include "zArray.h"
#include "zEvent.h"

class zWinFactory {
protected:
  static zMutex* _singleton_mtx;
  static zWinFactory* _singleton;

  zArray<zWin*> _windows;
  zEvent _event_close;
  zThread* _thread;
public:
  static zWinFactory* get_instance(void);
  /// Releases all resources. After this method the zWinFactory cannot be used.
  /// NOTE: This method MUST be called when application is closing.
  static void shutdown(void);
  
  /// Create a window
  zWin* create(zRect const& pos, zString const& name);
  /// Get zWin by window id.
  zWin* get_by_id(ZWIN_ID id);
  /// Wait for all windows are closed.
  bool wait_all_windows_close(void);

protected:
  zWinFactory(void);
  virtual ~zWinFactory(void);
};

#endif // ZWINFACTORY_H__

