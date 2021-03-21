#include "esp_log.h"
#include "st7789.h"

class RedDotMakerPlus
{
private:
  TFT_t *dev;

public:
  RedDotMakerPlus(TFT_t *dev);
  ~RedDotMakerPlus();
  void blink();
};

RedDotMakerPlus::RedDotMakerPlus(TFT_t *mdev)
{
  dev = mdev;
}

RedDotMakerPlus::~RedDotMakerPlus()
{
  ESP_LOGE(LOG_COLOR_E, "DESTROYED RED DOT");
}

void RedDotMakerPlus::blink()
{
  lcdDrawFillRect(dev, 0, 0, 100, 100, RED);
}

extern "C"
{
  void makeRedDot(TFT_t *dev)
  {
    RedDotMakerPlus *redDot = new RedDotMakerPlus(dev);
    redDot->blink();
    delete redDot;
  }
}
