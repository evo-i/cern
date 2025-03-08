#include "cern/drawing/drawing.h"
#include <Windows.h>
#include <stdio.h>
#include <gdiplus.h>

static
void
WINAPI
event_cb(int level, CHAR *message) {
  switch (level) {
    case 0:
      fprintf(stdout, "GDI+ Warning: %s\n", message);
      break;
    case 1:
      fprintf(stderr, "GDI+ Fatal Error: %s\n", message);
      break;
  }
}

static
ULONG_PTR gdiplus_token = 0;

static
struct GdiplusStartupInput init_input = {
  .DebugEventCallback = event_cb,
  .GdiplusVersion = 1,
  .SuppressBackgroundThread = FALSE,
  .SuppressExternalCodecs = FALSE
};

static
struct
GdiplusStartupOutput init_output = {
  .NotificationUnhook = NULL
};

gboolean
cern_drawing_init(void) {
  return
    GdiplusStartup(&gdiplus_token, &init_input, &init_output) == Ok;
}

void
cern_drawing_shutdown(void) {
  GdiplusShutdown(gdiplus_token);
  gdiplus_token = 0;
}
