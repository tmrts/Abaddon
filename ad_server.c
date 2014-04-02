#include <stdio.h>
#define __USE_GNU
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "ad_server.h"
#include "ad_http_request.h"
#include "ad_response.h"
#include "ad_utils.h"
#include "ad_method.h"

#include <pthread.h>

#include "ad_queue.h"
#include "ad_thread.h"
#include "ad_thread_pool.h"

volatile sig_atomic_t ad_server_terminating = 0;

int ad_server_is_terminating(void)
{
    return ad_server_terminating;
}

void ad_server_terminate(int signum) 
{ 
    ad_server_terminating = 1;
    fprintf(stdout, "\nAbaddon HTTP server is terminating please wait!\n");
    fflush(stdout);
}

void ad_server_answer(int client_socket, jmp_buf error_jmp)
{
    int requested_file;
    char path[512];
    char buffer[4096];
    ssize_t bytes_received, bytes_sent;
    ad_http_request *http_request = malloc(sizeof(ad_http_request));

    ad_response_receive(client_socket, buffer, 4096, error_jmp);

    ad_http_request_parse(http_request, buffer);

    if(http_request == NULL || METHOD(http_request) == NULL || !ad_method_is_valid(METHOD(http_request)))
    {
        ad_response_send(client_socket, AD_RESPONSE_CLIENT_BAD_REQUEST, error_jmp);
    }
    else if(strcasecmp(METHOD(http_request), "GET"))
    {
        ad_response_send(client_socket, AD_RESPONSE_SERVER_NOT_IMPLEMENTED, error_jmp);
    }
    else if(!strcasecmp(METHOD(http_request),"GET"))
    {
        sprintf(path, "htdocs%s", URI(http_request));
        if(ad_utils_is_directory(path))
        {
            strcat(path, "index.html");
        }

        if((requested_file = open(path,O_RDONLY)) == 0)
        {
            ad_response_send(client_socket, AD_RESPONSE_CLIENT_NOT_FOUND, error_jmp);
        }
        else
        {
            ad_response_send(client_socket, AD_RESPONSE_HTTP_OK, error_jmp);

            ad_response_sendfile(client_socket, requested_file, error_jmp);
        }
        close(requested_file);
    }

    shutdown(client_socket, SHUT_WR);

    ad_response_receive(client_socket, buffer, 4096, error_jmp);

    close(client_socket);

    ad_http_request_free(http_request);
}


int ad_server_listen(unsigned short int server_port)
{
    int  server_socket = -1;
    int  client_socket = -1;
    int *client_ptr = NULL;

    struct sigaction sig_struct;

    socklen_t server_len, client_len;
    struct sockaddr_in server_addr, client_addr;

    ad_queue *request_queue;
    ad_thread_pool *thread_pool;

    ad_queue_mutex request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
    ad_thread_pool_mutex pool_mutex= PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

    ad_queue_cond add_request_cond = PTHREAD_COND_INITIALIZER;
    ad_thread_pool_cond add_thread_cond = PTHREAD_COND_INITIALIZER;

    signal(SIGPIPE, SIG_IGN);

    /* system calls (e.g. accept()) are restarted when we use sigaction */
    sig_struct.sa_handler = ad_server_terminate;
    sig_struct.sa_flags = 0;
    sigaction(SIGINT, &sig_struct, NULL);

    request_queue = ad_queue_construct(&request_mutex, &add_request_cond);
    thread_pool = ad_thread_pool_construct(&pool_mutex, &add_thread_cond, request_queue);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_len = sizeof(server_addr);

    /* Initialize and configure server socket address */
    bzero(&server_addr, server_len);
    server_addr.sin_port = htons(server_port);
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    bind(server_socket, (struct sockaddr *) &server_addr, server_len);

    listen(server_socket, AD_SERVER_CONNECTION_BACKLOG);
    printf("Abaddon HTTP server is running on port %d\n", server_port);

    while (!ad_server_terminating)
    {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_len);

        if (client_socket == -1) 
        { 
            if(errno == EINTR) { /* The server received SIGINT */ }
            else               { perror("Client connection wasn't established\n"); }
        }
        else
        { 
            client_ptr = malloc(sizeof(int));
           *client_ptr = client_socket;

            ad_queue_push(request_queue, (void *)client_ptr); 

            /* Signals the thread pool to allow resizing of the pool */
            pthread_cond_signal(COND(THREAD_QUEUE(thread_pool)));
        }
    }
    close(server_socket);
    free(client_ptr);

    ad_queue_destruct(request_queue);
    ad_thread_pool_destruct(thread_pool);
    
    printf("Abaddon HTTP server terminated successfully\n");

    return EXIT_SUCCESS;
}
