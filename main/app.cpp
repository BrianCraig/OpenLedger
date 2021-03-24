#include "esp32/rom/uart.h"
#include "esp_log.h"
#include "esp_system.h"
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
    ESP_LOGI(LOG_COLOR_E, "int: %d, total: %d, worst: %d", esp_get_free_internal_heap_size(), esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
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