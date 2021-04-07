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
  Done,
  Canceled
};

class OlWindowI
{
public:
  virtual ~OlWindowI() {}
  virtual enum OlWindowStage apply(enum UserAction action) = 0;
  virtual void draw() = 0;
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
  OlMenuEntry *menu;
  std::list<OlMenuEntry *> path;
  std::list<OlMenuEntry *>::iterator selectedIt;
  void draw();
  bool onWindow = false;

public:
  OlMenu(OlMenuEntry *menu);
  ~OlMenu();
  void apply(enum UserAction action);
};

class OlStatusWindow : public OlWindowI
{
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();

protected:
  virtual std::string text() = 0;
  virtual uint16_t color() = 0;
};

class OlSuccessWindow : public OlStatusWindow
{
protected:
  virtual std::string text();
  uint16_t color();
};

class OlErrorWindow : public OlStatusWindow
{
protected:
  virtual std::string text();
  uint16_t color();
};

class OlIncomingTransactionWindow : public OlWindowI
{
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
};

class OlTransactionInfoWindow : public OlWindowI
{
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
};

class OlListSelect : public OlWindowI
{
public:
  OlListSelect(std::list<std::string> options);
  enum OlWindowStage apply(enum UserAction action);
  void draw();

private:
  std::list<std::string> options;
  std::list<std::string>::iterator selected;
};

class OlStepsWindow : public OlWindowI
{
public:
  OlStepsWindow(std::list<OlWindowI *> steps);
  ~OlStepsWindow();
  enum OlWindowStage apply(enum UserAction action);
  void draw();

private:
  std::list<OlWindowI *> steps;
  std::list<OlWindowI *>::iterator actual;
};

#endif // OL_UI_H_