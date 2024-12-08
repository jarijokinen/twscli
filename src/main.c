#include <stdlib.h>
#include "twsapi.h"

#define TWSAPI_IPADDR "127.0.0.1"
#define TWSAPI_PORT 7496

int main(void)
{
  int sockfd = twsapi_connect(TWSAPI_IPADDR, TWSAPI_PORT);
  twsapi_disconnect(sockfd);

  exit(EXIT_SUCCESS);
}
