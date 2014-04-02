#include <string.h>
#include <stdlib.h>

#include "ad_http_request.h"
#include "ad_method.h"
#include "ad_utils.h"

ad_http_header ad_http_request_parse_header(char *header_str) {}

void ad_http_request_parse(ad_http_request *http_request, char *request)
{
    int i;
    char *token, *substring, *end, *src = request;
    char **tokens = NULL;
    char **buffer = NULL;
    size_t substr_len;

    if (request != NULL || strstr(request, "\r\n\r\n") != NULL || strlen(request) > 4096) 
    {
        for (i = 0; strlen(request) > (src - request); i++)
        {
            buffer = realloc(buffer, sizeof(char *) * (i + 1));

            substr_len = end - src;

            buffer[i] = malloc(sizeof(char) * (substr_len + 1));

            strncpy(buffer[i], src, substr_len);
            /* Terminate string */
            buffer[i][substr_len] = NULL_CHAR;

            src = end + strlen(CLRF);
        }
        /* Terminate buffer */
        buffer[i] = NULL;

        /* Request Body */
        if (buffer[0])
        {
            tokens = ad_utils_split_str(buffer[0], " ");

            METHOD(http_request) = malloc(sizeof(char) * strlen(tokens[0]));
            strcpy(METHOD(http_request), tokens[0]);

            URI(http_request) = malloc(sizeof(char) * strlen(tokens[1]));
            strcpy(URI(http_request), tokens[1]);

            VERSION(http_request) = malloc(sizeof(char) * strlen(tokens[2]));
            strcpy(VERSION(http_request), tokens[2]);
        }

        /* TODO: Implement for HTTP/1.1 */
        /* Header Fields */
        for (i = 1; buffer[i]; i++)
        {
            ad_http_request_parse_header(buffer[i]);
        }

        for (i = 0; buffer[i]; i++) 
        {
            free(buffer[i]);
        }
        free(buffer);
    }
}

void ad_http_request_free(ad_http_request *http_request) 
{
    size_t i;

    free(METHOD(http_request));
    free(URI(http_request));
    free(VERSION(http_request));

    for (i = 0; HEADERS(http_request)[i]; i++)
    {
        free(HEADERS(http_request)[i]);
    }
    free(HEADERS(http_request)[i]);

    free(http_request);
}
