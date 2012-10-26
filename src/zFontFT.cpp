#include "zFontFT.h"

#include "zLogger.h"
#include "zFontGlyph.h"

// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


zFontFT::zFontFT(void) : _glyphs(NO, 1024) {
  _logger = zLogger::get_logger("zFontFT");
	_error = ERROR_NO_ERROR;
	// Allocate opaque pointer.
	_library = (FT_Library*)malloc(sizeof(FT_Library));
	_face = NULL;

    // Create And Initilize A FreeType Font Library.
	if (FT_Init_FreeType((FT_Library*)_library)) {
		_error = ERROR_FAILED_INIT_LIB;
		return;
	}
}


zFontFT::~zFontFT(void) {
  while (_glyphs.get_count() > 0) {
    zFontGlyph* glyph =  NULL;
    if (_glyphs.remove(0, &glyph)) {
      glyph->release_reference();
    }
  }


  FT_Done_Face(*((FT_Face*)_face));
	FT_Done_FreeType(*((FT_Library*)(_library)));

	free(_library);
	free(_face);

  _logger->release_reference();
}


zFontFT::Error zFontFT::load_file(const char* path) {
	if (_error != ERROR_NO_ERROR) return _error;
	
	if (_face  != NULL) {
		FT_Done_FreeType(*((FT_Library*)(_library)));
		free(_face);
	}

	_face = (FT_Face*)malloc(sizeof(FT_Face));

	FT_Error res = FT_New_Face(*(FT_Library*)_library, path, 0, (FT_Face*)_face);
	if (res != 0) {
		free(_face);
		_face = NULL;
		return ERROR_FAILED_LOAD_FACE;
	}
	return ERROR_NO_ERROR;
}


zFontGlyph* zFontFT::load_bitmap(unsigned long unicode, int font_size) {

  // Search glyphs.
  for (int i = 0; i < _glyphs.get_count(); i++) {
    // 
    zFontGlyph* glyph = NULL;
    if (_glyphs.get(i, &glyph)) {
      // 
      if (glyph->get_unicode() == unicode && glyph->get_font_size() == font_size) {
        glyph->acquire_reference();
        return glyph;
      }
    }
  }

  if (_error != ERROR_NO_ERROR) {
    _logger->error("Failed to load bitmap for unicode %lu because the instance is in error.", unicode);
    return NULL;
  }

	FT_Face* face = (FT_Face*)_face;

  // For Some Twisted Reason, FreeType Measures Font Size
  // In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
  // h Pixels High, We Need To Request A Size Of h*64.
  // (h << 6 Is Just A Prettier Way Of Writing h*64)
  // NOTE: 0 means same values. (ex: w: 6 h: 0 means w: 6 h: 6)
  // TODO: GET DPI from the system!!!!!!!
  FT_Set_Char_Size(*face, 0, font_size << 6, 96, 96);

  // Load The Glyph For Our Character.
	FT_UInt idx = FT_Get_Char_Index(*face, unicode);
  if (FT_Load_Glyph(*face, idx, FT_LOAD_RENDER)) {
    _logger->error("Failed to load glyph for unicode %lu because the load has failed.", unicode);
		return NULL;
	}
 
  if (FT_Render_Glyph((*face)->glyph, FT_RENDER_MODE_LIGHT)) {
    _logger->error("Failed to render glyph for unicode %lu.", unicode);
    return NULL;
  }
  
  zFontGlyph* glyph = new zFontGlyph(unicode, (*face)->glyph->bitmap.buffer, font_size, (*face)->glyph->bitmap.width, (*face)->glyph->bitmap.rows, (*face)->glyph->bitmap_left, (*face)->glyph->bitmap_top, (*face)->glyph->advance.x >> 6, (*face)->glyph->advance.y >> 6);
  glyph->acquire_reference();
  _glyphs.append(glyph);
  return glyph;
}
