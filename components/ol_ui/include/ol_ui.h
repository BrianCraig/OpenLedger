#ifndef OL_UI_H_
#define OL_UI_H_

#include <string>
#include <list>
#include <vector>

#include "st7789.h"
#include "ol_ui.h"
#include "_ol_layout.h"

enum UserAction {
  No,
  Yes,
  Up,
  Down
};

enum OlWindowStage {
  InProgress,
  Done,
  Canceled
};

class OlWindowI {
public:
  virtual ~OlWindowI() {}

  virtual enum OlWindowStage apply(enum UserAction action) = 0;
  virtual void draw() = 0;
};

class OlWindowStepsI {
public:
  virtual std::list<OlWindowI *> getSteps() = 0;
};

typedef OlWindowI *(*OlWindowICallback)();

class OlMenuEntry {
public:
  std::list<OlMenuEntry *> entries;
  OlMenuEntry(std::string *title);
  OlMenuEntry(std::string *title, OlWindowICallback callback);
  ~OlMenuEntry();
  std::string *title;
  void addEntry(OlMenuEntry *entry);
  void initializeWindow();
  void deleteWindow();
  OlWindowICallback callback = nullptr;
  OlWindowI *window = nullptr;
};

OlMenuEntry *exampleOlMenu();

class OlMenu {
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

class OlStatusWindow : public OlWindowI {
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
  void withInfo(std::string info);

protected:
  virtual std::string text() = 0;
  virtual OlIcon *icon() = 0;

private:
  std::string info;
  bool _withInfo = false;
};

class OlInputWindow : public OlWindowI {
public:
  OlInputWindow(std::string title, int length);
  enum OlWindowStage apply(enum UserAction action);
  void draw();
  std::string input = "";

private:
  std::string title;
  int length;
  std::list<char>::iterator character;
};

class OlSuccessWindow : public OlStatusWindow {
protected:
  virtual std::string text();
  OlIcon *icon();
};

class OlErrorWindow : public OlStatusWindow {
protected:
  virtual std::string text();
  OlIcon *icon();
};

class OlIncomingTransactionWindow : public OlWindowI {
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
};

class OlTransactionInfoWindow : public OlWindowI {
public:
  enum OlWindowStage apply(enum UserAction action);
  void draw();
};

class OlListSelect : public OlWindowI {
public:
  OlListSelect(std::list<std::string> options);
  enum OlWindowStage apply(enum UserAction action);
  void draw();

private:
  std::list<std::string> options;
  std::list<std::string>::iterator selected;
};

typedef OlWindowI *(*OlStepsCallback)(OlWindowStepsI *);

class OlStepsWindow : public OlWindowI, public OlWindowStepsI {
private:
  std::list<OlWindowI *> steps;
  OlStepsCallback callback;
public:
  OlStepsWindow(std::list<OlWindowI *> steps, OlStepsCallback callback);

  std::list<OlWindowI *> getSteps();
  enum OlWindowStage apply(enum UserAction action);
  void draw();

private:
  std::list<OlWindowI *>::iterator actual;
  OlWindowI *finalStep = nullptr;
  bool isFinalStep();
};

#endif // OL_UI_H_