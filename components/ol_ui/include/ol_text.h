#ifndef OL_TEXT_H_
#define OL_TEXT_H_

#include <string>
#include "mcu_fonts.h"
#include <mcufont.h>

#define Roboto20 &mf_rlefont_RobotoRegular20.font
#define Roboto32 &mf_rlefont_RobotoRegular32.font
#define FaBrand26 &mf_rlefont_fabrands26.font
#define FaSolid26 &mf_rlefont_fasolid26.font

#define FaIconEth 0xf42e
#define FaIconBtc 0xf379
#define FaIconCheck 0xf00c
#define FaIconTimes 0xf00d
#define FaIconIdCard 0xf2c2

#ifdef __cplusplus
extern "C"
{
#endif
  typedef struct
  {
    uint16_t color;
    const mf_font_s *font;
  } RenderTextState;

  void renderRleCharacter(const mf_font_s *font, uint16_t unicode, int16_t x, int16_t y, uint16_t color);

  void renderText(int16_t x, int16_t y, enum mf_align_t align, std::string *string, uint16_t color, const mf_font_s *font);

#ifdef __cplusplus
}
#endif

#endif // OL_TEXT_H_