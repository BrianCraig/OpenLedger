#include <string>
#include <list>
#include "st7789.h"
#include "ol_ui.h"
#include "esp_log.h"
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
  lcdFillScreen(olSystemStatus()->dev, 0xE71C);
  int width = 135;
  int height = 240;
  int block = 30;
  int spacing = 5;
  int items = path.back()->entries.size();
  int start = (height - (items * block) - ((items - 1) * spacing)) / 2;
  for (OlMenuEntry *entry : path.back()->entries)
  {
    lcdDrawFillRect(olSystemStatus()->dev, 0, start, width, start + block, *selectedIt == entry ? WHITE : BLACK);
    lcdDrawString(olSystemStatus()->dev, font, 20, start + 30, (uint8_t *)entry->title->c_str(), *selectedIt == entry ? BLACK : WHITE);
    start += block + spacing;
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
  //drawHello();
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

/* Base
OlMenu::OlMenu(TFT_t *dev, FontxFile *font, OlMenuEntry *menu)
{

}

enum OlWindowStage OlStatusWindow::apply(enum UserAction action)
{

}

void OlMenu::draw()
{

}
*/

OlListSelect::OlListSelect(std::list<std::string> options)
{
  for (std::string option : options)
  {
    this->options.push_back(option);
  }
  this->selected = this->options.begin();
}

enum OlWindowStage OlListSelect::apply(enum UserAction action)
{
  if (action == UserAction::Up)
  {
    if (selected != options.begin())
      std::advance(selected, -1);
  }
  else if (action == UserAction::Down)
  {
    if (*selected != options.back())
      std::advance(selected, 1);
  }
  else if (action == UserAction::No)
  {
    return OlWindowStage::Canceled;
  }
  else if (action == UserAction::Yes)
  {
    return OlWindowStage::Done;
  }
  draw();
  return OlWindowStage::InProgress;
}

void OlListSelect::draw()
{
  lcdFillScreen(olSystemStatus()->dev, BLACK);
  int y = 20;
  for (std::string option : options)
  {
    lcdDrawFillCircle(olSystemStatus()->dev, 20, y, 10, *selected == option ? RED : GREEN);
    lcdDrawString(olSystemStatus()->dev, olSystemStatus()->font24, 40, y + 12, (uint8_t *)option.c_str(), WHITE);
    y += 30;
  }
}

OlStepsWindow::OlStepsWindow(std::list<OlWindowI *> steps)
{
  for (OlWindowI *step : steps)
  {
    this->steps.push_back(step);
  }
  this->actual = this->steps.begin();
}

enum OlWindowStage OlStepsWindow::apply(enum UserAction action)
{
  OlWindowStage stage = (*actual)->apply(action);
  if (stage == OlWindowStage::Done)
  {
    std::advance(actual, 1);
    if (actual == steps.end())
    {
      return OlWindowStage::Done;
    }
    else
    {
      (*actual)->draw();
      return OlWindowStage::InProgress;
    }
  }
  else if (stage == OlWindowStage::InProgress)
  {
    return OlWindowStage::InProgress;
  }
  assert(stage == OlWindowStage::Canceled);
  return OlWindowStage::Done;
}

void OlStepsWindow::draw()
{
  ESP_LOGI(LOG_COLOR_E, "pointing: 0x%08x", (int)(*actual));
  (*actual)->draw();
}

OlStepsWindow::~OlStepsWindow()
{
}