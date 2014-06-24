#ifndef __display__
#define __display__


typedef struct _Display Display;

struct _Display
{
  void (*text_add)(Display* d, const char* text);
  void *data;
};

Display* display_console_new();

#endif
