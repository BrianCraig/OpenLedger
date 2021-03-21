#include <string.h>

#include "actions.h"
#include "st7789.h"
#include "fontx.h"
#include "ui.h"
#include "some.h"

const char *NOSTR = "No";
const char *YESSTR = "Yess";
const char *LEFTSTR = "Left";
const char *RIGHTSTR = "Right";

uint8_t *toString(enum UserAction action)
{
  if (action == No)
    return (uint8_t *)&NOSTR;
  /*
  if (action == Yes)
    return "Yes";
  if (action == Left)
    return "Left";
  if (action == Right)
    return "Right";*/
  return (uint8_t *)&YESSTR;
}

void initUI(struct UI *ui)
{
  InitFontx(&ui->fx32G, "/spiffs/ILGH32XB.FNT");
  spi_master_init(&ui->dev);
  lcdInit(&ui->dev);
  lcdSetFontDirection(&ui->dev, 0);
};
void applyAction(struct UI *ui, enum UserAction action)
{
  lcdFillScreen(&ui->dev, BLACK);
  int x = 135 / 2;
  int y = 240 / 2;
  if (action == No)
    x -= 30;
  if (action == Yes)
    x += 30;
  if (action == Up)
    y -= 30;
  if (action == Down)
  {
    y += 30;
    makeRedDot(&ui->dev);
  }

  lcdDrawFillRect(&ui->dev, x - 15, y - 15, x + 15, y + 15, GREEN);
};
