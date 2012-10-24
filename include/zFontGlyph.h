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

#ifndef ZFONTGLYPH_H__
#define ZFONTGLYPH_H__

#include "zCommon.h"
#include "zObject.h"

class zLogger;

/// This class store a glyph
/// 
class zFontGlyph : public zObject {
protected:
  zLogger* _logger;
  // The unicode value.
  unsigned long _unicode;
  /// The bitmap.
	unsigned char* _bitmap;
  // Requested size by the other classes.
  int _font_size;
	// Bitmap width.
  int _width;
  // Bitmap height.
  int _height;
  // Distance from left.
  int _left;
  // Distance from top.
  int _top;
  // X advance of current font.
  int _advance_x;
  // Y advance of current font
  int _advance_y;
  
public:
	/// Default constructor 
	zFontGlyph(unsigned long unicode, unsigned char* bitmap, int font_size, int width, int height, int left, int top, int advance_x, int advance_y);

  unsigned long get_unicode(void) const;
  int get_font_size(void) const;
	
	//int get_num_glyphs(void) const;
	int get_glyph_left(void) const;
  int get_glyph_top(void) const;
	int get_advance_x(void) const;
	int get_advance_y(void) const;
	
	unsigned char* get_bitmap(void) const;
	int get_bitmap_width(void) const;
	int get_bitmap_height(void) const;

protected:
  virtual ~zFontGlyph(void);
};

#endif // ZFONTGLYPH_H__
