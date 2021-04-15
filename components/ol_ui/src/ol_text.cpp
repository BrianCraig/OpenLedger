#include "ol_text.h"
#include "ol_system_status.h"
#include "mcu_fonts.h"
#include <mcufont.h>

static void pixel_callback(int16_t x, int16_t y, uint8_t count, uint8_t alpha, void *state)
{
  while (count--)
  {
    lcdDrawPixelAlpha(olSystemStatus()->dev, x, y, alpha, ((RenderTextState *)state)->color);
    x++;
  }
}

void renderRleCharacter(const mf_font_s *font, uint16_t unicode, int16_t x, int16_t y, uint16_t color)
{
  RenderTextState state;
  state.color = color;
  mf_rlefont_render_character(font, x, y, unicode, &pixel_callback, &state);
}

static uint8_t char_callback(int16_t x0, int16_t y0, mf_char character, void *state)
{
  return mf_render_character(((RenderTextState *)state)->font, x0, y0, character, &pixel_callback, state);
}

void renderText(int16_t x, int16_t y, enum mf_align_t align, std::string *string, uint16_t color, const mf_font_s *font)
{
  RenderTextState state;
  state.color = color;
  state.font = font;
  mf_render_aligned(
      font,
      x, y,
      align,
      string->c_str(),
      string->size(),
      &char_callback,
      (void *)&state);
}