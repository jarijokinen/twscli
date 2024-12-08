#include <stdio.h>
#include <stdlib.h>
#include "twsapi.h"

#define TWSAPI_IPADDR "127.0.0.1"
#define TWSAPI_PORT 7496

int main(void)
{
  printf("Connecting to TWS API...\n");
  int sockfd = twsapi_connect(TWSAPI_IPADDR, TWSAPI_PORT);
  if (sockfd) {
    printf("Connected to TWS API successfully\n");
  }

  twsapi_disconnect(sockfd);
  printf("Disconnected from TWS API\n");

  exit(EXIT_SUCCESS);
}
