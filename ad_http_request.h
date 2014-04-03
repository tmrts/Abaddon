#ifndef AD_HTTP_REQUEST_H
#define AD_HTTP_REQUEST_H

#define NULL_CHAR '\0';
#define CLRF "\r\n"

typedef struct {
    char *name;
    char *field;
}ad_http_header;

#define NAME(h) ((h) -> name)
#define FIELD(h) ((h) -> field)

typedef struct {
    char *method;
    char *uri;
    char *version;
    ad_http_header **headers;
}ad_http_request;

#define METHOD(r)   ((r) -> method)
#define URI(r)      ((r) -> uri)
#define VERSION(r)  ((r) -> version)
#define HEADERS(r)  ((r) -> headers)

int ad_http_request_is_valid(ad_http_request request);

ad_http_request *ad_http_request_parse(char *request);

void ad_http_request_free(ad_http_request *http_request);

#endif
