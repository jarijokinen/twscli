#include <stdio.h>
#include <stdlib.h>

#include "message.h"

void twsapi_message_in(char **fields, size_t num_fields)
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
    case HISTORICAL_DATA:
      printf("Historical data:\n");
      printf("Fields: ");
      for (uint32_t i = 4; i+7 < num_fields; i += 8) {
        printf("D: %s ", fields[i+1]);
        printf("O: %s ", fields[i+2]);
        printf("C: %s ", fields[i+3]);
        printf("H: %s ", fields[i+4]);
        printf("L: %s ", fields[i+5]);
        printf("W: %s ", fields[i+6]);
        printf("V: %s\n", fields[i+7]);
      }
      break;
    default:
      printf("Unhandled incoming message type: %d\n", msg_type);
      printf("Fields: ");
      for (uint32_t i = 0; i < num_fields; i++) {
        printf("%d: %s\n", i, fields[i]);
      }
      break;
  }
}
