#ifndef OL_UI_H_
#define OL_UI_H_

#include <string>
#include <list>

#include "st7789.h"
#include "ol_ui.h"

enum UserAction
{
  No,
  Yes,
  Up,
  Down
};

enum OlWindowStage
{
  InProgress,
  Done
};

class OlWindowI
{
public:
  void setDev(TFT_t *dev) { this->dev = dev; }
  virtual ~OlWindowI() {}
  virtual enum OlWindowStage apply(enum UserAction action) = 0;
  virtual void draw() = 0;

protected:
  TFT_t *dev;
};

class OlMenuEntry
{
public:
  std::list<OlMenuEntry *> entries;
  OlMenuEntry(std::string *title);
  OlMenuEntry(std::string *title, OlWindowI *window);
  ~OlMenuEntry();
  std::string *title;
  void addEntry(OlMenuEntry *entry);
  OlWindowI *window = NULL;
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
  bool onWindow = false;

public:
  OlMenu(TFT_t *dev, FontxFile *font, OlMenuEntry *menu);
  ~OlMenu();
  void apply(enum UserAction action);
};

class OlStatusWindow : public OlWindowI
{
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
};

#endif // OL_UI_H_