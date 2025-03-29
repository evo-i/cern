#ifndef CERN_UI_MESSAGE_H
#define CERN_UI_MESSAGE_H 1

typedef struct _CernMessage {
  gpointer window_handle;
  gint32 message;
  gpointer first_param;
  gpointer second_param;
  gpointer result;
} CernMessage;

#endif /* CERN_UI_MESSAGE_H */
