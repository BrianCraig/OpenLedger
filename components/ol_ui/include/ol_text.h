#ifndef OL_TEXT_H_
#define OL_TEXT_H_

#include <string>
#include "mcu_fonts.h"
#include <mcufont.h>

#define Roboto20 &mf_rlefont_RobotoRegular20.font
#define Roboto32 &mf_rlefont_RobotoRegular32.font

#ifdef __cplusplus
extern "C"
{
#endif
  typedef struct
  {
    uint16_t color;
    const mf_font_s *font;
  } RenderTextState;

  void renderText(int16_t x, int16_t y, enum mf_align_t align, std::string *string, uint16_t color, const mf_font_s *font);

#ifdef __cplusplus
}
#endif

#endif // OL_TEXT_H_