#include <string.h>
#include <stdlib.h>

#include "ad_http_request.h"
#include "ad_method.h"
#include "ad_utils.h"

ad_http_header ad_http_request_parse_header(char *header_str) 
{
}

/* Parses the request received from client.
 *
 * @param   request http request string
 * @return  returns either NULL(not a legal HTTP request) 
 *          or ad_http_request struct allocated containing 
 *          the parsed results
 */
ad_http_request *ad_http_request_parse(char *request)
{
    int i = 0;
    char *end, *src = request;
    char **tokens = NULL;
    char **buffer = NULL;
    size_t substr_len;
    ad_http_request *http_request = NULL;

    if (request != NULL && strstr(request, "\r\n\r\n") != NULL && strlen(request) > strlen(CLRF)) 
    {
        http_request = malloc(sizeof(ad_http_request));
        HEADERS(http_request) = NULL;
        for (i = 0; strlen(request) > (src - request); i++)
        {
            buffer = realloc(buffer, sizeof(char *) * (i + 2));

            end = strstr(src, CLRF);
            substr_len = end - src;

            buffer[i] = malloc(sizeof(char) * (substr_len + 1));
            strncpy(buffer[i], src, substr_len);
            /* Terminate string */
            strcat(buffer[i], "\0");

            src = end + strlen(CLRF);
        }
        /* Terminate buffer */
        buffer[i] = NULL;

        /* Assumes that the sent requests consist of method, uri, version for the moment */
        /* TODO: support argnumber and version flow control (e.g. treat as HTTP/0.9 if only a method is sent) */
        /* Request Body */
        if (buffer[0])
        {
            tokens = ad_utils_split_str(buffer[0], " ");

            METHOD(http_request) = tokens[0];

            URI(http_request) = tokens[1];

            VERSION(http_request) = tokens[2];

            free(tokens);
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

    return http_request;
}

/* Frees the resources held by the ad_http_request struct.
 *
 * @param http_request ad_http_request structure to be freed.
 */
void ad_http_request_free(ad_http_request *http_request) 
{
    size_t i;

    free(METHOD(http_request));
    free(URI(http_request));
    free(VERSION(http_request));

    if(HEADERS(http_request))
    {
        for (i = 0; HEADERS(http_request)[i]; i++)
        {
            free(HEADERS(http_request)[i]);
        }
    }

    free(http_request);
}
