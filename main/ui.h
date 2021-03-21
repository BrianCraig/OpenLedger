#ifndef UI_BR_H
#define UI_BR_H

#include "actions.h"
#include "st7789.h"
#include "fontx.h"

struct UI
{
  FontxFile fx32G;
  TFT_t dev;
};

void initUI(struct UI *ui);
void applyAction(struct UI *ui, enum UserAction action);

#endif