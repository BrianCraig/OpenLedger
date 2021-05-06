#include <string>
#include <list>
#include "ol_ui.h"
#include "_ol_layout.h"

OlWindowI *createSteps()
{
  OlListSelect *btcEthSel = new OlListSelect({"Bitcoin", "Ethereum"});
  return new OlStepsWindow({
      btcEthSel,
      new OlSuccessWindow(),
      new OlErrorWindow(),
  });
};

OlWindowI *successExample()
{
  return new OlSuccessWindow();
}

OlWindowI *errorExample()
{
  OlErrorWindow *errorWindowWithText = new OlErrorWindow();
  errorWindowWithText->withInfo("Incorrect wallet key");
  return errorWindowWithText;
}

OlWindowI *incomingTransactionExample()
{
  return new OlIncomingTransactionWindow();
}

OlWindowI *transactionInfoExample()
{
  return new OlTransactionInfoWindow();
}

OlWindowI *inputExample()
{
  return new OlInputWindow("Define Alias", 8);
}

OlMenuEntry *exampleOlMenu()
{
  OlMenuEntry *base = new OlMenuEntry(new std::string("Base"));

  OlMenuEntry *account = new OlMenuEntry(new std::string("Account"));
  OlMenuEntry *history = new OlMenuEntry(new std::string("History"));
  OlMenuEntry *settings = new OlMenuEntry(new std::string("Settings"));
  OlMenuEntry *test = new OlMenuEntry(new std::string("Test"));

  base->addEntry(account);
  base->addEntry(history);
  base->addEntry(settings);
  base->addEntry(test);

  OlMenuEntry *account_list = new OlMenuEntry(new std::string("List"));

  OlMenuEntry *account_create = new OlMenuEntry(new std::string("Create"), &createSteps);
  OlMenuEntry *account_add = new OlMenuEntry(new std::string("Add"));
  OlMenuEntry *account_info = new OlMenuEntry(new std::string("Info"));
  OlMenuEntry *account_contacts = new OlMenuEntry(new std::string("Contacts"));

  account->addEntry(account_list);
  account->addEntry(account_create);
  account->addEntry(account_add);
  account->addEntry(account_info);
  account->addEntry(account_contacts);

  OlMenuEntry *successExampleWindow = new OlMenuEntry(new std::string("Success"), &successExample);

  OlMenuEntry *errorExampleWindow = new OlMenuEntry(new std::string("Error"), &errorExample);
  OlMenuEntry *incomingTransaction = new OlMenuEntry(new std::string("Incoming"), incomingTransactionExample);
  OlMenuEntry *transactionInfo = new OlMenuEntry(new std::string("T Info"), transactionInfoExample);
  OlMenuEntry *input = new OlMenuEntry(new std::string("Input"), inputExample);
  test->addEntry(successExampleWindow);
  test->addEntry(errorExampleWindow);
  test->addEntry(incomingTransaction);
  test->addEntry(transactionInfo);
  test->addEntry(input);

  return base;
}
