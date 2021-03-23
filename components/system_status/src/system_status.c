#include "ol_system_status.h"

SystemStatus _olSystemStatus;

SystemStatus *olSystemStatus()
{
  return &_olSystemStatus;
}