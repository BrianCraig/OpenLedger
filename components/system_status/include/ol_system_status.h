#ifndef SYSTEM_STATUS_OL_H_
#define SYSTEM_STATUS_OL_H_

#include "st7789.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    TFT_t *dev;
  } SystemStatus;

  SystemStatus *olSystemStatus();

#ifdef __cplusplus
}
#endif

#endif // SYSTEM_STATUS_OL_H_