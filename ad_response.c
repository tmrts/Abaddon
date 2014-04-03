#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>

#include "ad_response.h"

void ad_response_send(int client, char *response, jmp_buf error_jmp)
{
    size_t bytes_left;
    ssize_t bytes_sent;

    bytes_left = strlen(response);

    while(bytes_left)
    {
        bytes_sent = send(client, response, strlen(response), MSG_NOSIGNAL);
        if (bytes_sent == -1) 
        {
            perror("ad_response_send"); 
            close(client);
            longjmp(error_jmp, 1);
        }
        bytes_left -= bytes_sent;
    }
}

void ad_response_sendfile(int client, int file, jmp_buf error_jmp)
{
    off_t offset = 0;
    size_t file_size, bytes_left;
    ssize_t bytes_sent;
    struct stat file_info;

    fstat(file, &file_info);
    file_size = file_info.st_size;
    bytes_left = file_size;

    while(bytes_left)
    {
        bytes_sent = sendfile(client, file, &offset, file_size);
        if (bytes_sent == -1) 
        {
            perror("ad_response_sendfile"); 
            close(client);
            close(file);
            longjmp(error_jmp, 1);
        }
        offset += bytes_sent;
        bytes_left -= bytes_sent;
    }
}

void ad_response_receive(int client, char *buff, size_t buff_len, jmp_buf error_jmp) 
{
    ssize_t bytes_received;

    bytes_received = recv(client, buff, buff_len, MSG_NOSIGNAL);
    if (bytes_received == -1)
    {
        perror("ad_response_receive");
        close(client);
        longjmp(error_jmp, 1);
    }
}
