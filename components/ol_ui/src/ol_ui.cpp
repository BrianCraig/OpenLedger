#include <string>
#include <list>
#include "st7789.h"
#include "ol_ui.h"

OlMenuEntry::OlMenuEntry(std::string *title)
{
  this->title = title;
}

void OlMenuEntry::addEntry(OlMenuEntry *entry)
{
  entries.push_back(entry);
}

OlMenuEntry::~OlMenuEntry()
{
  delete title;
  for (OlMenuEntry *entry : entries)
  {
    delete entry;
  }
}

OlMenuEntry *exampleOlMenu()
{
  OlMenuEntry *base = new OlMenuEntry(new std::string("Base"));
  base->addEntry(new OlMenuEntry(new std::string("Empty entry")));
  OlMenuEntry *baseWithTwo = new OlMenuEntry(new std::string("TwoEntry"));
  baseWithTwo->addEntry(new OlMenuEntry(new std::string((const char *)&"menu1")));
  baseWithTwo->addEntry(new OlMenuEntry(new std::string("menu2")));
  base->addEntry(baseWithTwo);
  return base;
}

OlMenu::OlMenu(TFT_t *dev, FontxFile *font, OlMenuEntry *menu)
{
  this->dev = dev;
  this->font = font;
  this->menu = menu;
  this->selected = menu->entries.front();
  path.push_back(menu);
  draw();
}

void OlMenu::apply(enum UserAction action)
{
  if (action == UserAction::Up)
  {
    selected = path.back()->entries.front();
  }
  else if (action == UserAction::Down)
  {
    auto l_front = path.back()->entries.begin();
    std::advance(l_front, 1);
    selected = *l_front;
  }
  else if (action == UserAction::No)
  {
    if (path.size() > 1)
    {
      path.pop_back();
      selected = path.back()->entries.front();
    }
  }
  else if (action == UserAction::Yes)
  {
    path.push_back(selected);
    selected = selected->entries.front();
  }
  draw();
}

void OlMenu::draw()
{
  lcdFillScreen(dev, BLACK);
  int y = 20;
  for (OlMenuEntry *entry : path.back()->entries)
  {
    lcdDrawFillCircle(dev, 20, y, 10, selected == entry ? RED : GREEN);
    lcdDrawString(dev, font, 40, y + 12, (uint8_t *)entry->title->c_str(), WHITE);
    y += 30;
  }
}

OlMenu::~OlMenu()
{
}
