#ifndef AD_SERVER_H
#define AD_SERVER_H

#include <setjmp.h>

#define AD_SERVER_CONNECTION_BACKLOG 128
#define AD_HTTP_REQUEST_MAX_SIZE 4096
#define AD_SERVER_SOCKET_READ_TIMEOUT_SECONDS 10
#define AD_SERVER_SOCKET_WRITE_TIMEOUT_SECONDS 10

int ad_server_is_terminating(void);

/* SIGINT handler */
void ad_server_terminate(int signum);

int  ad_server_listen(unsigned short int server_port);

void ad_server_answer(int client_socket, jmp_buf error_jmp);

#endif
