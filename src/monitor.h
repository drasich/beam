#ifndef __monitor__
#define __monitor__
#include "Eina.h"
#include "display.h"

typedef struct _Monitor Monitor;
struct _Monitor
{
  const char* path;
  Eina_List* extensions;
  const char* command;

  Display* display;
};

Monitor* monitor_new(const char* path, Eina_List* extensions, const char* cmd);
void monitor_init();
void monitor_shutdown();

#endif
