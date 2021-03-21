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

  OlMenuEntry *account = new OlMenuEntry(new std::string("Account"));
  OlMenuEntry *history = new OlMenuEntry(new std::string("History"));
  OlMenuEntry *settings = new OlMenuEntry(new std::string("Settings"));

  base->addEntry(account);
  base->addEntry(history);
  base->addEntry(settings);

  OlMenuEntry *account_list = new OlMenuEntry(new std::string("List"));
  OlMenuEntry *account_add = new OlMenuEntry(new std::string("Add"));
  OlMenuEntry *account_info = new OlMenuEntry(new std::string("Info"));
  OlMenuEntry *account_contacts = new OlMenuEntry(new std::string("Contacts"));

  account->addEntry(account_list);
  account->addEntry(account_add);
  account->addEntry(account_info);
  account->addEntry(account_contacts);

  return base;
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
  if (action == UserAction::Up)
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
    path.push_back(*selectedIt);
    selectedIt = (*selectedIt)->entries.begin();
  }
  draw();
}

void OlMenu::draw()
{
  lcdFillScreen(dev, BLACK);
  int y = 20;
  for (OlMenuEntry *entry : path.back()->entries)
  {
    lcdDrawFillCircle(dev, 20, y, 10, *selectedIt == entry ? RED : GREEN);
    lcdDrawString(dev, font, 40, y + 12, (uint8_t *)entry->title->c_str(), WHITE);
    y += 30;
  }
}

OlMenu::~OlMenu()
{
}
