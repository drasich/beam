#include "display.h"
#include <stdlib.h>
#include <stdio.h>

static void
_console_text_add(Display* d, const char* text)
{
  printf("%s", text);
}

Display* display_console_new()
{
  Display* d = calloc(1, sizeof *d);
  d->text_add = _console_text_add;
  return d;
}

static void
_gui_text_add(Display* d, const char* text)
{

}

Display* display_gui_new()
{
  Display* d = calloc(1, sizeof *d);
  d->text_add = _gui_text_add;
  //d->data = evas ob
  return d;
}

