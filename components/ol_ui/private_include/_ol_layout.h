#ifndef _OL_LAYOUT_H_
#define _OL_LAYOUT_H_

#include <list>
#include <string>

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

class OlText : public OlLayoutWithHeight
{
public:
  OlText(std::string text, enum OlTextSize size, int lines);
  OlText *withBackground(uint16_t color);
  void render(int y);
  int height();

private:
  bool hasBackground = false;
  uint16_t background;
  std::string text;
  enum OlTextSize size;
  int lines;
};

void OlLayout(std::list<OlLayoutWithHeight *> elements);

#endif // _OL_LAYOUT_H_