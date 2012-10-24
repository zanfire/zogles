#include "zFontGlyph.h"

#include "zLogger.h"

zFontGlyph::zFontGlyph(unsigned long unicode, unsigned char* bitmap, int font_size, int width, int height, int left, int top, int advance_x, int advance_y) {
  _logger = zLogger::get_logger("zFontGlyph");
  
  _unicode = unicode;
  // MEMCOPY
  _bitmap = (unsigned char*)malloc(sizeof(unsigned char) * width * height); // Only Alpha
  memcpy(_bitmap, bitmap, sizeof(unsigned char) * width * height);
  
  _font_size = font_size;
  _width     = width;
  _height    = height;
  _left      = left;
  _top       = top;
  _advance_x = advance_x;
  _advance_y = advance_y;
}


zFontGlyph::~zFontGlyph(void) {
  free(_bitmap);
  _logger->release_reference();
}


unsigned long zFontGlyph::get_unicode(void) const {
	return _unicode;
}


int zFontGlyph::get_font_size(void) const {
	return _font_size;
}



int zFontGlyph::get_bitmap_width(void) const {
	return _width;
}


int zFontGlyph::get_bitmap_height(void) const {
  return _height;
}


int zFontGlyph::get_glyph_left(void) const {
	return _left;
}


int zFontGlyph::get_glyph_top(void) const {
  return _top;
}


int zFontGlyph::get_advance_x(void) const {
  return _advance_x;
}


int zFontGlyph::get_advance_y(void) const {
	return _advance_y;
}


unsigned char* zFontGlyph::get_bitmap(void) const {
	return _bitmap;
}

/*
int zFontFT::get_bitmap_size(void) const {
	return (*bmp)->bitmap.width * (*bmp)->bitmap.pitch;
}
*/