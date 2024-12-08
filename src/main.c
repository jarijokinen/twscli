#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "twsapi.h"

#define TWSAPI_IPADDR "127.0.0.1"
#define TWSAPI_PORT 7496
#define TWSAPI_MIN_CLIENT_VER 100
#define TWSAPI_MAX_CLIENT_VER 157

int main(void)
{
  pthread_t tid;

  printf("Connecting to TWS API...\n");
  int sockfd = twsapi_connect(TWSAPI_IPADDR, TWSAPI_PORT);
  if (sockfd) {
    printf("Connected to TWS API successfully\n");
  }

  int server_ver = twsapi_handshake(
    sockfd,
    TWSAPI_MIN_CLIENT_VER,
    TWSAPI_MAX_CLIENT_VER
  );

  if (server_ver < 0) {
    printf("twsapi_handshake() error");
  }
  else {
    printf("Server API version: %d\n", server_ver);
  }

  if (pthread_create(&tid, NULL, twsapi_recv, (void *)&sockfd) != 0) {
    perror("pthread_create() error");
  }

  twsapi_start(sockfd);

  while (1) {
    sleep(1);
  }

  pthread_cancel(tid);
  pthread_join(tid, NULL);
  twsapi_disconnect(sockfd);
  printf("Disconnected from TWS API\n");

  exit(EXIT_SUCCESS);
}
