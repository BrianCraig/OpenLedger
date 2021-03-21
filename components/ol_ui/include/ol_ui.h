#ifndef OL_UI_H_
#define OL_UI_H_

enum UserAction
{
  No,
  Yes,
  Up,
  Down
};

#ifndef __cplusplus

THIS IS A CPP LIB ILL RUIN YOUR COMPILATION LUL

#endif

#include <string>
#include <list>

#include "st7789.h"
#include "ol_ui.h"

    class OlMenuEntry
{
public:
  std::list<OlMenuEntry *> entries;
  OlMenuEntry(std::string *title);
  ~OlMenuEntry();
  std::string *title;
  void addEntry(OlMenuEntry *entry);
};

OlMenuEntry *exampleOlMenu();

class OlMenu
{
private:
  TFT_t *dev;
  FontxFile *font;
  OlMenuEntry *menu;
  std::list<OlMenuEntry *> path;
  std::list<OlMenuEntry *>::iterator selectedIt;
  void draw();

public:
  OlMenu(TFT_t *dev, FontxFile *font, OlMenuEntry *menu);
  ~OlMenu();
  void apply(enum UserAction action);
};
#endif // OL_UI_H_