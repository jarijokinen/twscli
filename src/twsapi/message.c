#include <stdio.h>
#include <stdlib.h>

#include "message.h"

void twsapi_message_in(char **fields, int num_fields)
{
  enum IncomingMessage msg_type = atoi(fields[0]);
  int err = 0;

  switch (msg_type) {
    case ERR_MSG:
      err = atoi(fields[3]);
      if (err == 2104 || err == 2106 || err == 2158) {
        printf("Info: %s\n", fields[4]);
      }
      else {
        printf("Error: %s\n", fields[4]);
      }
      break;
    case NEXT_VALID_ID:
      printf("Next valid order ID: %s\n", fields[2]);
      break;
    case MANAGED_ACCTS:
      printf("Managed account ID: %s\n", fields[2]);
      break;
    default:
      printf("Unhandled incoming message type: %d\n", msg_type);
      printf("Fields: ");
      for (int i = 0; i < num_fields; i++) {
        printf("%d: %s\n", i, fields[i]);
      }
      break;
  }
}
