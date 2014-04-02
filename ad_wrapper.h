#ifndef AD_WRAPPER_H
#define AD_WRAPPER_H
#include <sys/socket.h>

void system_error(const char *error_msg);

void kill_server(int return_value, char *func_name);

int Socket(int family, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

#endif
