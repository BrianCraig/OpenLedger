#include "st7789.h"
#include "ol_system_status.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_heap_caps.h"

/*
const bool text[] = {
  1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 
  1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 
  1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 
  1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 
  1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 
}; */

const bool text[] = {
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    1,
    1,
    1,
    1,
    0,
    1,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    1,
    1,
    0,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
};

void drawHello()
{
  ESP_LOGI(LOG_COLOR_I, "Preparing to allocate data");
  uint16_t *random = (uint16_t *)malloc(135 * 240 * 2);
  ESP_LOGI(LOG_COLOR_I, "Data allocated at 0x%08x", (int)random);
  ESP_LOGI(LOG_COLOR_E, "int: %d, total: %d, worst: %d", esp_get_free_internal_heap_size(), esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
  esp_fill_random(random, 135 * 240 * 2);
  //uint16_t toDraw[95] = {};
  //for (int i = 0; i < 95; i++)
  //{
  //  toDraw[i] = text[i] ? WHITE : BLACK;
  //}
  //lcdDrawMultiPixelsWH(olSystemStatus()->dev, 19, 5, 19, 5, (uint16_t *)&toDraw);
  ESP_LOGI(LOG_COLOR_E, "random filled");

  for (int i = 0; i < 240; i += 2)
  {
    lcdDrawMultiPixelsWH(olSystemStatus()->dev, 0, i, 135, 2, (uint16_t *)((random) + (i * 2 * 135)));
  }

  //lcdDrawMultiPixelsWH(olSystemStatus()->dev, 0, 0, 135, 20, (uint16_t *)&random);
  ESP_LOGI(LOG_COLOR_I, "Printed");
  free(random);
}
