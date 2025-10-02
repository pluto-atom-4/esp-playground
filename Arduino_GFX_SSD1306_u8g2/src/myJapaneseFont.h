#pragma once
#include <stdint.h>
#include <gfxfont.h>

// Bitmap data for the glyphs (example: not actual Japanese, just for structure)
const uint8_t myJapaneseFontBitmaps[] PROGMEM = {
  // ... bitmap data for "こ"
  // ... bitmap data for "ん"
  // ... bitmap data for "に"
  // ... bitmap data for "ち"
  // ... bitmap data for "は"
};

// Glyph metadata (one entry per character, example only)
const GFXglyph myJapaneseFontGlyphs[] PROGMEM = {
  // { bitmapOffset, width, height, xAdvance, xOffset, yOffset }
  {     0, 16, 16, 18, 0, -14 }, // "こ" (U+3053)
  {   32, 16, 16, 18, 0, -14 }, // "ん" (U+3093)
  {   64, 16, 16, 18, 0, -14 }, // "に" (U+306B)
  {   96, 16, 16, 18, 0, -14 }, // "ち" (U+3061)
  {  128, 16, 16, 18, 0, -14 }, // "は" (U+306F)
};

// Unicode intervals (example only)
const UnicodeInterval myJapaneseFontIntervals[] = {
  { 0x3053, 0x3053, 0 }, // "こ"
  { 0x3093, 0x3093, 1 }, // "ん"
  { 0x306B, 0x306B, 2 }, // "に"
  { 0x3061, 0x3061, 3 }, // "ち"
  { 0x306F, 0x306F, 4 }, // "は"
};

// Font definition
const GFXfont myJapaneseFont16pt7b PROGMEM = {
  (uint8_t*)myJapaneseFontBitmaps,
  (GFXglyph*)myJapaneseFontGlyphs,
  (UnicodeInterval*)myJapaneseFontIntervals,
  5,     // number of glyphs
  16,    // font size (yAdvance)
  0,     // ascender
  -4,    // descender
};