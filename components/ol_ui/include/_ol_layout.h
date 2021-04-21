#ifndef _OL_LAYOUT_H_
#define _OL_LAYOUT_H_

#include <list>
#include <string>
#include "ol_text.h"
#include "st7789.h"

class OlLayoutWithHeight
{
public:
  virtual ~OlLayoutWithHeight() {}
  virtual void render(int y) = 0;
  virtual int height() = 0;
};

enum OlTextSize
{
  S20,
  S32,
};

class OlLine : public OlLayoutWithHeight
{
public:
  OlLine(std::list<OlLayoutWithHeight *> elements);
  OlLine *withBackground(uint16_t color);
  void render(int y);
  int height();

private:
  bool hasBackground = false;
  uint16_t background;
  std::list<OlLayoutWithHeight *> elements;
};

class OlText : public OlLayoutWithHeight
{
public:
  OlText(std::string text, enum OlTextSize size, int lines);
  OlText *withBackground(uint16_t color);
  OlText *fromTo(int from, int to);
  OlText *align(enum mf_align_t align);
  OlText *color(uint16_t color);
  const mf_font_s *font();
  int x();
  void render(int y);
  int height();
  int from = 0, to;
  uint16_t _color = BLACK;
  enum mf_align_t _align = mf_align_t::MF_ALIGN_CENTER;
  enum OlTextSize size;
  int y;

private:
  bool hasBackground = false;
  uint16_t background;
  std::string text;
  int lines;
};

class OlIcon : public OlLayoutWithHeight
{
public:
  OlIcon(uint16_t code, const mf_font_s *font, int x, int y);
  OlIcon *color(uint16_t color);
  const mf_font_s *font;
  uint16_t code;
  void render(int y);
  int height();
  uint16_t _color = BLACK;
  int x, y;
};

class OlCircle : public OlLayoutWithHeight
{
public:
  OlCircle(int x, int y, int radius, uint16_t color);
  void render(int y);
  int height();
  uint16_t color;
  int x, y, radius;
};

void OlLayout(std::list<OlLayoutWithHeight *> elements);

#endif // _OL_LAYOUT_H_