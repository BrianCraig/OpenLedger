#include "esp32/rom/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ol_ui.h"
#include "ol_system_status.h"

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
      return UserAction::No;
    if (myChar == 'd' || myChar == 'D')
      return UserAction::Yes;
    if (myChar == 'w' || myChar == 'W')
      return UserAction::Up;
    if (myChar == 's' || myChar == 'S')
      return UserAction::Down;
  }

  return Yes;
}

extern "C"
{

  void application()
  {
    TFT_t dev;
    spi_master_init(&dev);
    lcdInit(&dev);
    FontxFile font;
    olSystemStatus()->dev = &dev;
    olSystemStatus()->font24 = &font;
    InitFontx(&font, "/spiffs/ILGH24XB.FNT");
    lcdSetFontDirection(&dev, 0);
    OlMenu *menu = new OlMenu(&dev, &font, exampleOlMenu());
    for (;;)
    {
      menu->apply(getAction());
    }
  }
}