#ifndef TWSCLI_TWSAPI_H
#define TWSCLI_TWSAPI_H

int twsapi_connect(const char *ipaddr, int port);
int twsapi_disconnect(int sockfd);
int twsapi_send(int sockfd, const char *msg);

#endif
