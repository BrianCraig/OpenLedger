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

int OlText::height()
{
  return 30;
}

void OlText::render(int y)
{
  uint16_t width = olSystemStatus()->dev->_width;
  if (hasBackground)
    lcdDrawFillRect(olSystemStatus()->dev, 0, y, width, y + height(), background);
  renderText(width / 2, y - 4, MF_ALIGN_CENTER, &text, BLACK, size == OlTextSize::S32 ? Roboto32 : Roboto20);
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