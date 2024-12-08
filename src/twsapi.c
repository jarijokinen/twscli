#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "twsapi.h"

int twsapi_connect(const char *ipaddr, int port)
{
  int sockfd = 0;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket() error");
    return -1;
  }

  struct sockaddr_in serv_addr = {0};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0) {
    perror("inet_pton() error");
    close(sockfd);
    return -1;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect() error");
    close(sockfd);
    return -1;
  }

  return sockfd;
}

int twsapi_disconnect(int sockfd)
{
  close(sockfd);
  return 0;
}

int twsapi_send(int sockfd, const char *msg)
{
  return 0;
}
