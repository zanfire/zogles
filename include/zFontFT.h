/******************************************************************************
 * Copyright 2009 - 2012 Matteo Valdina
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

#ifndef FONTFT_H__
#define FONTFT_H__

#include "zCommon.h"
#include "zObject.h"
#include "zArray.h"

class zLogger;
class zFontGlyph;

/// This class provides a C++ wrapper to FreeType library. 
/// NOTE: This class is not thread-safe.
class zFontFT : public zObject {
public: 
	enum Error {
		ERROR_NO_ERROR = 0,
		ERROR_FAILED_INIT_LIB,
		ERROR_FAILED_LOAD_FACE,
		ERROR_FAILED_LOAD_GLYPH
	};

protected:
  zLogger* _logger;
	// Opaque pointer to FreeType library.
	void* _library;
	// Opaque pointer to FreeType face (font).
	void* _face;
	// Current status
	Error _error;
	// Vector of _glyphs.
  zArray<zFontGlyph*> _glyphs;

public:
	/// Default constructor 
	/// @param fontPath is the path to the font.
	zFontFT(void);
	
	/// Load from a file.
	/// @param file can be a absolute path or a file name. In case of file name will be searched in Font system folder.
	Error load_file(const char* fileName);
	/// Load from a buffer.
	Error load_memory(unsigned char* buffer, int len);
	/// 
	zFontGlyph* load_bitmap(unsigned long unicode, int desired_height);

protected:
  virtual ~zFontFT(void);
};


#endif // ZFONTFT_H__
