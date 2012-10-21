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

#ifndef ZOGLESCONFIGS_H__
#define ZOGLESCONFIGS_H__

#include "zCommon.h"

#include "zArray.h"

#include <EGL/egl.h>

class zLogger;
class zOGLESRuntime;

class zOGLESConfigs {
  friend class zOGLESRuntime;
protected:
  zLogger* _logger;
  EGLDisplay _display;
  zArray<EGLConfig> _configs;

public:

  /// Returns a EGLConfig with the requested the colors size.
  EGLConfig choose_match(int red_size, int green_size, int blue_size);

protected:
  zOGLESConfigs(EGLDisplay display);
  virtual ~zOGLESConfigs(void);

  bool load_configs(void);
};

#endif // ZOGLESCONFIGS_H__
