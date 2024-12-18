#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connection.h"
#include "message.h"

void twsapi_start(int sockfd)
{
  char msg[] = "71|2|1||";
  twsapi_send(sockfd, msg);
  sleep(1);
}

void *twsapi_recv(void *arg)
{
  int sockfd = *(int *)arg;
  char *buffer = NULL;

  while (1) {
    uint32_t msg_len;
    int bytes = recv(sockfd, &msg_len, sizeof(msg_len), 0);
    if (bytes < 0) {
      perror("recv() error");
      break;
    }
    else if (bytes == 0) {
      perror("recv() error: connection closed by server");
      break;
    }

    msg_len = ntohl(msg_len);
    
    buffer = malloc(msg_len + 1);
    if (buffer == NULL) {
      perror("malloc() error");
      exit(EXIT_FAILURE);
    }

    size_t total_bytes = 0;
    while (total_bytes < msg_len) {
      bytes = recv(sockfd, buffer + total_bytes, msg_len - total_bytes, 0);
      if (bytes < 0) {
        perror("recv() error");
        free(buffer);
        break;
      }
      else if (bytes == 0) {
        perror("recv() error: connection closed by server");
        free(buffer);
        break;
      }
      total_bytes += bytes;
    }

    if (total_bytes < msg_len) {
      perror("Incomplete message received");
      free(buffer);
      break;
    }

    buffer[msg_len] = '\0';

    size_t fields_capacity = 10;
    char **fields = malloc(fields_capacity * sizeof(char *));
    if (fields == NULL) {
      perror("malloc() error");
      free(buffer);
      exit(EXIT_FAILURE);
    }

    size_t num_fields = 0;
    char *field = buffer;

    while (field < buffer + msg_len) {
      if (num_fields >= fields_capacity) {
        fields_capacity *= 2;
        char **new_fields = realloc(fields, fields_capacity * sizeof(char *));
        if (new_fields == NULL) {
          perror("realloc() error");
          free(fields);
          free(buffer);
          exit(EXIT_FAILURE);
        }
        fields = new_fields;
      }

      fields[num_fields++] = field;
      field += strlen(field) + 1;

      if (field >= buffer + msg_len) {
        break;
      }
    }

    if (num_fields > 0) {
        twsapi_message_in(fields, num_fields);
    }

    free(fields);
    free(buffer);
  }

  return NULL;
}

int twsapi_handshake(int sockfd, int min_client_ver, int max_client_ver)
{
  char buffer[1024] = {0};

  if (send(sockfd, "API\0", 4, 0) < 0) {
    perror("send() error");
    return -1;
  }
  
  char client_ver[10];
  snprintf(client_ver, sizeof(client_ver), "v%d..%d", min_client_ver, 
    max_client_ver);
  twsapi_send(sockfd, client_ver);

  int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
  if (bytes < 0) {
    perror("recv() error");
    return -1;
  }
  else if (bytes == 0) {
    perror("recv() error: connection closed by server");
    return -1;
  }

  uint32_t msg_len = 0;
  memcpy(&msg_len, buffer, sizeof(uint32_t));
  msg_len = ntohl(msg_len);

  char *msg = malloc(msg_len + 1);
  if (msg == NULL) {
    perror("malloc() error");
    return -1;
  }

  memcpy(msg, buffer + sizeof(uint32_t), msg_len);
  msg[msg_len] = '\0';

  int server_ver = atoi(msg);
  free(msg);
  if (server_ver < min_client_ver || server_ver > max_client_ver) {
    perror("Server API version not supported");
    return -1;
  }

  return server_ver;
}

void twsapi_pack(char *str, uint8_t **packed_msg, size_t *packed_msg_len)
{
  size_t str_len = strlen(str);

  for (size_t i = 0; str[i] != '\0'; i++) {
    if (str[i] == '|') {
      str[i] = '\0';
    }
  }

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

  return sockfd;
}

int twsapi_disconnect(int sockfd)
{
  close(sockfd);
  return 0;
}

int twsapi_send(int sockfd, char *msg)
{
  size_t packed_msg_len = 0;
  uint8_t *packed_msg = NULL;

  twsapi_pack(msg, &packed_msg, &packed_msg_len);
  
  if (send(sockfd, packed_msg, packed_msg_len, 0) < 0) {
    perror("send() error");
    free(packed_msg);
    return -1;
  }

  free(packed_msg);
  return 0;
}
