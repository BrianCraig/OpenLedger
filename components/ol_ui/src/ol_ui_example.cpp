#include <string>
#include <list>
#include "ol_ui.h"
#include "_ol_layout.h"

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

  OlListSelect *btcEthSel = new OlListSelect({"Bitcoin", "Ethereum"});

  OlStepsWindow *createSteps = new OlStepsWindow({
      btcEthSel,
      new OlSuccessWindow(),
      new OlErrorWindow(),
  });

  OlMenuEntry *account_create = new OlMenuEntry(new std::string("Create"), createSteps);
  OlMenuEntry *account_add = new OlMenuEntry(new std::string("Add"));
  OlMenuEntry *account_info = new OlMenuEntry(new std::string("Info"));
  OlMenuEntry *account_contacts = new OlMenuEntry(new std::string("Contacts"));

  account->addEntry(account_list);
  account->addEntry(account_create);
  account->addEntry(account_add);
  account->addEntry(account_info);
  account->addEntry(account_contacts);

  OlMenuEntry *successExample = new OlMenuEntry(new std::string("Success"), new OlSuccessWindow());
  OlMenuEntry *errorExample = new OlMenuEntry(new std::string("Error"), new OlErrorWindow());
  OlMenuEntry *incomingTransaction = new OlMenuEntry(new std::string("Incoming"), new OlIncomingTransactionWindow());
  OlMenuEntry *transactionInfo = new OlMenuEntry(new std::string("T Info"), new OlTransactionInfoWindow());
  OlMenuEntry *input = new OlMenuEntry(new std::string("Input"), new OlInputWindow("Define Alias", 8));
  test->addEntry(successExample);
  test->addEntry(errorExample);
  test->addEntry(incomingTransaction);
  test->addEntry(transactionInfo);
  test->addEntry(input);


  return base;
}
