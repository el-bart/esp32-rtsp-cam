#pragma once
#include <esp_task_wdt.h>

inline void watchdog_init()
{
  esp_task_wdt_init(30/*s*/, true);
  esp_task_wdt_add(NULL);   // add current task to watchdog watch
}

inline void watchdog_reset()
{
  esp_task_wdt_reset();
}
