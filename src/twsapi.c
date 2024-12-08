#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "twsapi.h"

int twsapi_handshake(int sockfd, int min_client_ver, int max_client_ver)
{
  if (send(sockfd, "API", 4, 0) < 0) {
    perror("send() error");
    return -1;
  }

  char client_ver[10];
  snprintf(client_ver, sizeof(client_ver), "v%d..%d", min_client_ver, 
    max_client_ver);
  twsapi_send(sockfd, client_ver);

  return 0;
}

void twsapi_pack(const char *str, uint8_t **packed_msg, size_t *packed_msg_len)
{
  size_t str_len = strlen(str);

  *packed_msg_len = sizeof(uint32_t) + str_len;
  *packed_msg = malloc(*packed_msg_len);
  if (*packed_msg == NULL) {
    perror("malloc() error");
    exit(EXIT_FAILURE);
  }

  uint32_t str_len_n = htonl(str_len);
  memcpy(*packed_msg, &str_len_n, sizeof(uint32_t));

  memcpy(*packed_msg + sizeof(uint32_t), str, str_len);
}

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

  if (twsapi_handshake(sockfd, 100, 157) < 0) {
    perror("twsapi_handshake() error");
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
  size_t packed_msg_len = 0;
  uint8_t *packed_msg = NULL;

  twsapi_pack(msg, &packed_msg, &packed_msg_len);

  if (send(sockfd, &packed_msg, packed_msg_len, 0) < 0) {
    perror("send() error");
    return -1;
  }

  return 0;
}
