#include <string.h>
#include <stdio.h>
#include "http.h"

enum request_types
{
    REQUEST_OTHER,
    REQUEST_GET,
    REQUEST_POST,
    REQUEST_PUT,
    REQUEST_DELETE,
    REQUEST_PATCH,
    REQUEST_HEAD,
    REQUEST_OPTIONS,
    REQUEST_TRACE
};

struct request_data
{
    enum request_types type;
    char *string;
};

static struct request_data detect_request_type(const char *request_string);

void handle_http_protocol(const char *request, char *response, const int buffer_size)
{
    struct request_data request_type = detect_request_type(request);
    printf("Request Type : %s\n", request_type.string);

    const char *hello_res = "Hello, World!\r\n";
    switch (request_type.type)
    {
    case REQUEST_GET:
    case REQUEST_POST:
    case REQUEST_PUT:
    case REQUEST_DELETE:
        snprintf(response, buffer_size, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: text/plain\r\n\r\n%s", strlen(hello_res), hello_res);
        break;

    case REQUEST_OTHER:
    case REQUEST_PATCH:
    case REQUEST_HEAD:
    case REQUEST_OPTIONS:
    case REQUEST_TRACE:
        snprintf(response, buffer_size, "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n");
        break;

    default:
        break;
    }
}

struct request_data detect_request_type(const char *request_string)
{
    struct request_data request_type_list[] = {
        {.type = REQUEST_OTHER, .string = "OTHER"},
        {.type = REQUEST_GET, .string = "GET"},
        {.type = REQUEST_POST, .string = "POST"},
        {.type = REQUEST_PUT, .string = "PUT"},
        {.type = REQUEST_DELETE, .string = "DELETE"},
        {.type = REQUEST_PATCH, .string = "PATCH"},
        {.type = REQUEST_HEAD, .string = "HEAD"},
        {.type = REQUEST_OPTIONS, .string = "OPTIONS"},
        {.type = REQUEST_TRACE, .string = "TRACE"},
    };

    struct request_data request_type = request_type_list[0];

    for (size_t i = 0; i < sizeof(request_type_list) / sizeof(struct request_data); i++)
    {
        for (size_t j = 0; j < strlen(request_string); j++)
        {
            if (strncmp(request_type_list[i].string, request_string + j, strlen(request_type_list[i].string)) == 0)
            {
                request_type = request_type_list[i];
                break;
            }
        }
    }

    return request_type;
}