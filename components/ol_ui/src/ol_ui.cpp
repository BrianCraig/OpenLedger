#include <string>
#include <list>
#include "st7789.h"
#include "ol_ui.h"
#include "ol_system_status.h"

OlMenuEntry::OlMenuEntry(std::string *title)
{
  this->title = title;
}

OlMenuEntry::OlMenuEntry(std::string *title, OlWindowI *window)
{
  this->title = title;
  this->window = window;
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

OlMenu::OlMenu(TFT_t *dev, FontxFile *font, OlMenuEntry *menu)
{
  this->dev = dev;
  this->font = font;
  this->menu = menu;
  this->selectedIt = menu->entries.begin();
  path.push_back(menu);
  draw();
}

void OlMenu::apply(enum UserAction action)
{
  if (onWindow)
  {
    OlWindowStage stage = (*selectedIt)->window->apply(action);
    if (stage == OlWindowStage::Done)
    {
      onWindow = false;
    }
    else if (stage == OlWindowStage::InProgress)
    {
      return;
    }
  }
  else if (action == UserAction::Up)
  {
    if (selectedIt != path.back()->entries.begin())
      std::advance(selectedIt, -1);
  }
  else if (action == UserAction::Down)
  {
    if (*selectedIt != path.back()->entries.back())
      std::advance(selectedIt, 1);
  }
  else if (action == UserAction::No)
  {
    if (path.size() > 1)
    {
      path.pop_back();
      selectedIt = path.back()->entries.begin();
    }
  }
  else if (action == UserAction::Yes)
  {
    if ((*selectedIt)->window)
    {
      onWindow = true;
      (*selectedIt)->window->draw();
      return;
    }
    else
    {
      path.push_back(*selectedIt);
      selectedIt = (*selectedIt)->entries.begin();
    }
  }
  draw();
}

void OlMenu::draw()
{
  lcdFillScreen(olSystemStatus()->dev, BLACK);
  int y = 20;
  for (OlMenuEntry *entry : path.back()->entries)
  {
    lcdDrawFillCircle(olSystemStatus()->dev, 20, y, 10, *selectedIt == entry ? RED : GREEN);
    lcdDrawString(olSystemStatus()->dev, font, 40, y + 12, (uint8_t *)entry->title->c_str(), WHITE);
    y += 30;
  }
}

OlMenu::~OlMenu()
{
}

enum OlWindowStage OlStatusWindow::apply(enum UserAction action)
{
  auto stage = action == UserAction::Yes ? OlWindowStage::Done : OlWindowStage::InProgress;
  if (stage == OlWindowStage::InProgress)
  {
    draw();
  }
  return stage;
}

void OlStatusWindow::draw()
{
  lcdFillScreen(olSystemStatus()->dev, BLACK);
  lcdDrawString(olSystemStatus()->dev, olSystemStatus()->font24, 10, 30, (uint8_t *)text().c_str(), WHITE);
  lcdDrawFillCircle(olSystemStatus()->dev, 135 / 2, 240 / 2, 50, color());
}

std::string OlSuccessWindow::text()
{
  return "Successful";
}

uint16_t OlSuccessWindow::color()
{
  return GREEN;
}

std::string OlErrorWindow::text()
{
  return "Error";
}

uint16_t OlErrorWindow::color()
{
  return RED;
}
