#include "actions.h"
#include "esp32/rom/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui.h"

void waitMs(int ms)
{
  vTaskDelay(ms / portTICK_PERIOD_MS);
}

enum UserAction getAction()
{
  for (;;)
  {
    waitMs(10);
    uint8_t myChar;
    STATUS s = uart_rx_one_char(&myChar);
    if (s != OK)
      continue;
    if (myChar == 'a' || myChar == 'A')
      return No;
    if (myChar == 'd' || myChar == 'D')
      return Yes;
    if (myChar == 'w' || myChar == 'W')
      return Up;
    if (myChar == 's' || myChar == 'S')
      return Down;
  }

  return Yes;
}

void application()
{
  struct UI ui;
  initUI(&ui);
  for (;;)
  {
    applyAction(&ui, getAction());
  }
}