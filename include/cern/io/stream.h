#ifndef CERN_IO_STREAM_H
#define CERN_IO_STREAM_H 1

#include <glib-object.h>

G_BEGIN_DECLS

#define CERN_TYPE_IO_STREAM (cern_io_stream_get_type())

G_DECLARE_INTERFACE(CernIOStream, cern_io_stream, CERN, IO_STREAM, GObject)

gboolean
cern_io_stream_can_read(CernIOStream *stream);

gboolean
cern_io_stream_can_seek(CernIOStream *stream);

gboolean
cern_io_stream_can_timeout(CernIOStream *stream);

gboolean
cern_io_stream_can_write(CernIOStream *stream);

gsize
cern_io_stream_length(CernIOStream *stream);

gsize
cern_io_stream_get_position(CernIOStream *stream);

void
cern_io_stream_set_position(CernIOStream *stream, gsize position);

gsize
cern_io_stream_get_read_timeout(CernIOStream *self);

void
cern_io_stream_set_read_timeout(CernIOStream *self, gsize timeout);

gsize
cern_io_stream_get_write_timeout(CernIOStream *self);

void
cern_io_stream_set_write_timeout(CernIOStream *self, gsize timeout);

void
cern_io_stream_copy_to(CernIOStream *source, CernIOStream *destination);


G_END_DECLS

#endif /* CERN_IO_STREAM_H */
