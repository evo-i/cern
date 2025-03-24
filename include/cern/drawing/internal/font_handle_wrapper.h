#ifndef CERN_DRAWING_INTERNAL_FONT_HANDLE_WRAPPER_H
#define CERN_DRAWING_INTERNAL_FONT_HANDLE_WRAPPER_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_FONT_HANDLE_WRAPPER cern_font_handle_wrapper_get_type()

G_DECLARE_FINAL_TYPE(CernFontHandleWrapper, cern_font_handle_wrapper, CERN, FONT_HANDLE_WRAPPER, GObject);

typedef struct _CernFont CernFont; 

CernFontHandleWrapper *
cern_font_handle_wrapper_new(CernFont *font);

CernFont *
cern_font_handle_wrapper_get_font(CernFontHandleWrapper *self);

void
cern_font_handle_wrapper_set_font(CernFontHandleWrapper *self, CernFont *font);

void
cern_font_handle_wrapper_unref(CernFontHandleWrapper *self);


G_END_DECLS

#endif /* CERN_DRAWING_INTERNAL_FONT_HANDLE_WRAPPER_H */
