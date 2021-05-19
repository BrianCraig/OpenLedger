#include <string>
#include "ol_ui.h"

OlWindowI *callbackDecider(OlWindowStepsI *wind) {
  assert(wind->getSteps().size() == 2);
  std::string key = reinterpret_cast<OlInputWindow *>(*std::next(wind->getSteps().begin()))->input;
  if (key == std::string("AAA")) {
    return new OlSuccessWindow();
  } else {
    auto errWin = new OlErrorWindow();
    errWin->withInfo("Incorrect wallet key");
    return errWin;
  }
}

OlWindowI *createSteps() {
  auto *alias = new OlInputWindow("Define Alias", 10);
  auto *wk = new OlInputWindow("Wallet key", 10);
  return new OlStepsWindow(
    {
      alias,
      wk,
    }, &callbackDecider);
}

OlWindowI *successExample() {
  return new OlSuccessWindow();
}

OlWindowI *errorExample() {
  auto *errorWindowWithText = new OlErrorWindow();
  errorWindowWithText->withInfo("Incorrect wallet key");
  return errorWindowWithText;
}

OlWindowI *incomingTransactionExample() {
  return new OlIncomingTransactionWindow();
}

OlWindowI *transactionInfoExample() {
  return new OlTransactionInfoWindow();
}

OlWindowI *inputExample() {
  return new OlInputWindow("Define Alias", 8);
}

OlMenuEntry *exampleOlMenu() {
  auto *base = new OlMenuEntry(new std::string("Base"));

  auto *account = new OlMenuEntry(new std::string("Account"));
  auto *history = new OlMenuEntry(new std::string("History"));
  auto *settings = new OlMenuEntry(new std::string("Settings"));
  auto *test = new OlMenuEntry(new std::string("Test"));

  base->addEntry(account);
  base->addEntry(history);
  base->addEntry(settings);
  base->addEntry(test);

  auto *account_list = new OlMenuEntry(new std::string("List"));

  auto *account_create = new OlMenuEntry(new std::string("Create"), &createSteps);
  auto *account_add = new OlMenuEntry(new std::string("Add"));
  auto *account_info = new OlMenuEntry(new std::string("Info"));
  auto *account_contacts = new OlMenuEntry(new std::string("Contacts"));

  account->addEntry(account_list);
  account->addEntry(account_create);
  account->addEntry(account_add);
  account->addEntry(account_info);
  account->addEntry(account_contacts);

  auto *successExampleWindow = new OlMenuEntry(new std::string("Success"), &successExample);

  auto *errorExampleWindow = new OlMenuEntry(new std::string("Error"), &errorExample);
  auto *incomingTransaction = new OlMenuEntry(new std::string("Incoming"), incomingTransactionExample);
  auto *transactionInfo = new OlMenuEntry(new std::string("T Info"), transactionInfoExample);
  auto *input = new OlMenuEntry(new std::string("Input"), inputExample);
  test->addEntry(successExampleWindow);
  test->addEntry(errorExampleWindow);
  test->addEntry(incomingTransaction);
  test->addEntry(transactionInfo);
  test->addEntry(input);

  return base;
}
