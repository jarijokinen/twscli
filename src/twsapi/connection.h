#ifndef TWSAPI_CONNECTION_H
#define TWSAPI_CONNECTION_H

int twsapi_connect(const char *ipaddr, int port);
int twsapi_disconnect(int sockfd);
int twsapi_send(int sockfd, char *msg);
int twsapi_handshake(int sockfd, int min_client_ver, int max_client_ver);
void *twsapi_recv(void *arg);
void twsapi_start(int sockfd);

#endif
