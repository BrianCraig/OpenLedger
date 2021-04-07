#include <algorithm>
#include <numeric>
#include "_ol_layout.h"
#include "ol_system_status.h"
#include "ol_text.h"

OlText::OlText(std::string text, enum OlTextSize size, int lines)
{
  this->text = text;
  this->size = size;
  this->lines = lines;
}

OlText *OlText::withBackground(uint16_t color)
{
  this->hasBackground = true;
  this->background = color;
  return this;
}

OlText *OlText::fromTo(int from, int to)
{
  //this->hasBackground = true;
  //this->background = color;
  return this;
}

OlText *OlText::align(enum mf_align_t align)
{
  //this->hasBackground = true;
  //this->background = color;
  return this;
}

int OlText::height()
{
  return (23 * lines);
}

typedef struct
{
  int y;
  const mf_font_s *font;
} LineState;

bool drawLine(mf_str text, uint16_t count, void *state)
{
  LineState *lineState = (LineState *)state;
  std::string *stdText = new std::string(text, count);
  renderText(olSystemStatus()->dev->_width / 2, lineState->y, MF_ALIGN_CENTER, stdText, BLACK, lineState->font);
  delete (stdText);
  lineState->y += 23;
  return true;
}

void OlText::render(int y)
{
  uint16_t width = olSystemStatus()->dev->_width;
  LineState state;
  state.y = y;
  state.font = size == OlTextSize::S32 ? Roboto32 : Roboto20;
  if (hasBackground)
    lcdDrawFillRect(olSystemStatus()->dev, 0, y, width, y + height(), background);
  mf_wordwrap(state.font, width, text.c_str(), &drawLine, &state);
}

void OlLayout(std::list<OlLayoutWithHeight *> elements)
{
  lcdStartFrame(olSystemStatus()->dev);
  lcdFillScreen(olSystemStatus()->dev, 0xE71C);

  int items = elements.size();
  int spacing = 5;
  int height = olSystemStatus()->dev->_height;
  auto unary_op = [](int add, OlLayoutWithHeight *elem) { return add + elem->height(); };
  int total = std::accumulate(elements.begin(), elements.end(), 0, unary_op);
  int drawY = (height - total - ((items - 1) * spacing)) / 2;
  for (OlLayoutWithHeight *element : elements)
  {
    element->render(drawY);
    drawY += element->height() + spacing;
  }

  lcdEndFrame(olSystemStatus()->dev);
}

OlLine::OlLine(std::list<OlLayoutWithHeight *> elements)
{
  this->elements = elements;
}

OlLine *OlLine::withBackground(uint16_t color)
{
  this->hasBackground = true;
  this->background = color;
  return this;
}

int OlLine::height()
{
  std::list<int> heights;
  std::transform(elements.begin(), elements.end(), heights.begin(), [](OlLayoutWithHeight *element) -> int { return element->height(); });
  return *std::max_element(heights.begin(), heights.end());
}

void OlLine::render(int y)
{
  if (hasBackground)
    lcdDrawFillRect(olSystemStatus()->dev, 0, y, olSystemStatus()->dev->_width, y + height(), background);
  for (auto element : elements)
  {
    element->render(y);
  }
}