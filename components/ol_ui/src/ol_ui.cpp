#include <string>
#include <list>
#include <vector>
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
  OlCircle circle = OlCircle{olSystemStatus()->dev->_width / 2, 0, 30, 0xCE59};
  OlIcon *icon = this->icon();
  OlLine circleWithIcon = OlLine{{&circle,
                                  icon}};
  std::list<OlLayoutWithHeight *> items = {OlText{text(), OlTextSize::S20, 1}.withBackground(WHITE),
                                           &circleWithIcon};
  OlText infoText = OlText{info, OlTextSize::S20, 2};
  if (_withInfo)
  {
    items.push_back(&infoText);
  }
  OlLayout(items, 15);
  delete icon;
}

void OlStatusWindow::withInfo(std::string info)
{
  this->info = info;
  this->_withInfo = true;
}

std::string OlSuccessWindow::text()
{
  return "Success";
}

OlIcon *OlSuccessWindow::icon()
{
  return (new OlIcon(FaIconCheck, FaSolid26, (olSystemStatus()->dev->_width / 2) - 13, 18))->color(0x0461);
}

std::string OlErrorWindow::text()
{
  return "Error";
}

OlIcon *OlErrorWindow::icon()
{
  return (new OlIcon(FaIconTimes, FaSolid26, (olSystemStatus()->dev->_width / 2) - 9, 18))->color(0xF800);
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
      {OlText{"Incoming Transaction", OlTextSize::S20, 2}.withBackground(WHITE),
       OlText{"37.12mEth", OlTextSize::S20, 1}.withBackground(WHITE),
       OlText{"Deny, Open", OlTextSize::S20, 1}.withBackground(WHITE)},
      15);
}

OlWindowStage OlTransactionInfoWindow::apply(enum UserAction action)
{
  if (action == UserAction::Yes)
  {
    return OlWindowStage::Done;
  }
  else if (action == UserAction::No)
  {
    return OlWindowStage::Canceled;
  }

  draw();
  return OlWindowStage::InProgress;
}

void OlTransactionInfoWindow::draw()
{
  OlLayout(
      {OlLine{{OlIcon{FaIconIdCard, FaSolid26, 1, 0}.color(BLUE),
               OlText{"Alice", OlTextSize::S20, 1}.fromTo(34, olSystemStatus()->dev->_width)->align(MF_ALIGN_LEFT)}}
           .withBackground(WHITE)
           ->withPadding(2),
       OlLine{{OlText{"0x", OlTextSize::S20, 1}.fromTo(0, 32)->align(MF_ALIGN_RIGHT)->color(GRAY),
               OlText{"45a3 6a8e 118c 37e4 c47e f4ab 827a 7c9e 579e 11e2", OlTextSize::S20, 5}.fromTo(34, olSystemStatus()->dev->_width)->align(MF_ALIGN_LEFT)}}
           .withBackground(WHITE),
       OlLine{{OlIcon{FaIconEth, FaBrand26, 8, 0}.color(BLUE),
               OlText{"3.24mEth", OlTextSize::S20, 1}.fromTo(34, olSystemStatus()->dev->_width)->align(MF_ALIGN_LEFT)}}
           .withBackground(WHITE)
           ->withPadding(2)},
      15);
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

static std::list<char> characters = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
};

OlInputWindow::OlInputWindow(std::string title, int length)
{
  this->title = title;
  this->length = length;
  this->character = characters.begin();
}

enum OlWindowStage OlInputWindow::apply(enum UserAction action)
{
  if (action == UserAction::Yes)
  {
    if (character == characters.end())
    {
      return OlWindowStage::Done;
    }
    input.append(std::string{*character});
    if (input.length() == length)
    {
      return OlWindowStage::Done;
    }
  }
  else if (action == UserAction::No)
  {
    return OlWindowStage::Canceled;
  }
  else if (action == UserAction::Up)
  {
    std::advance(character, 1);
  }
  else if (action == UserAction::Down)
  {
    std::advance(character, -1);
  }

  draw();
  return OlWindowStage::InProgress;
}

std::string charItAsText(std::list<char>::iterator charIt)
{
  if (charIt == characters.end())
  {
    return "End";
  }
  return std::string{*charIt};
}

void OlInputWindow::draw()
{
  OlText dots = OlText{input, OlTextSize::S20, 1},
         spacing = OlText{"", OlTextSize::S32, 1},
         topChar = OlText{charItAsText(std::next(character)), OlTextSize::S20, 1},
         actualChar = OlText{charItAsText(character), OlTextSize::S32, 1},
         bottomChar = OlText{charItAsText(std::prev(character)), OlTextSize::S20, 1};
  OlLayout(
      {OlText{title, OlTextSize::S20, 1}.withBackground(WHITE),
       &dots,
       &spacing,
       &topChar,
       &actualChar,
       &bottomChar});
}

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