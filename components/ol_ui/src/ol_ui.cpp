#include <string>
#include <list>
#include "st7789.h"
#include "ol_ui.h"
#include "esp_log.h"
#include "ol_system_status.h"
#include "ol_text.h"
#include "_ol_layout.h"

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

OlMenu::OlMenu(OlMenuEntry *menu)
{
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
  lcdStartFrame(olSystemStatus()->dev);
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
    renderText(olSystemStatus()->dev->_width / 2, start - 4, MF_ALIGN_CENTER, entry->title, *selectedIt == entry ? BLACK : WHITE, Roboto32);
    start += block + spacing;
  }
  lcdEndFrame(olSystemStatus()->dev);
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
  std::string titletext = text();
  lcdStartFrame(olSystemStatus()->dev);
  lcdFillScreen(olSystemStatus()->dev, WHITE);
  renderText(olSystemStatus()->dev->_width / 2, 10, MF_ALIGN_CENTER, &titletext, BLACK, Roboto20);
  lcdDrawFillCircle(olSystemStatus()->dev, 135 / 2, 240 / 2, 50, color());
  lcdEndFrame(olSystemStatus()->dev);
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

OlWindowStage OlIncomingTransactionWindow::apply(enum UserAction action)
{
  auto stage = action == UserAction::Yes ? OlWindowStage::Done : OlWindowStage::InProgress;
  if (stage == OlWindowStage::InProgress)
  {
    draw();
  }
  return stage;
}

void OlIncomingTransactionWindow::draw()
{

  OlLayout(
      {(new OlText("Incoming Transaction", OlTextSize::S20, 2))->withBackground(WHITE),
       (new OlText("37.12mEth", OlTextSize::S20, 1))->withBackground(WHITE),
       new OlText("Deny, Open", OlTextSize::S20, 2)});
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
  lcdStartFrame(olSystemStatus()->dev);
  lcdFillScreen(olSystemStatus()->dev, 0xE71C);
  int width = 135;
  int height = 240;
  int block = 30;
  int spacing = 5;
  int items = options.size();
  int start = (height - (items * block) - ((items - 1) * spacing)) / 2;
  for (std::string option : options)
  {
    lcdDrawFillRect(olSystemStatus()->dev, 0, start, width, start + block, *selected == option ? WHITE : BLACK);
    renderText(olSystemStatus()->dev->_width / 2, start - 4, MF_ALIGN_CENTER, &option, *selected == option ? BLACK : WHITE, Roboto32);
    start += block + spacing;
  }
  lcdEndFrame(olSystemStatus()->dev);
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