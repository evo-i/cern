#include <glib.h>
#ifndef CERN_IO_STREAM_H
#define CERN_IO_STREAM_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_STREAM (cern_stream_get_type())

G_DECLARE_INTERFACE(CernStream, cern_stream, CERN, STREAM, GObject)

struct _CernStreamClass {
  GObject parent_instance;

  gboolean
  (*can_read)(CernStream *self);

  gboolean
  (*can_write)(CernStream *self);


};

gboolean
cern_stream_can_read(CernStream *stream);

gboolean
cern_stream_can_seek(CernStream *stream);

gboolean
cern_stream_can_timeout(CernStream *stream);

gboolean
cern_stream_can_write(CernStream *stream);

gsize
cern_stream_length(CernStream *stream);

gsize
cern_stream_get_position(CernStream *stream);

void
cern_stream_set_position(CernStream *stream, gsize position);

gsize
cern_stream_get_read_timeout(CernStream *self);

void
cern_stream_set_read_timeout(CernStream *self, gsize timeout);

gsize
cern_stream_get_write_timeout(CernStream *self);

void
cern_stream_set_write_timeout(CernStream *self, gsize timeout);

void
cern_stream_copy_to(CernStream *source, CernStream *destination);


G_END_DECLS

#endif /* CERN_IO_STREAM_H */
