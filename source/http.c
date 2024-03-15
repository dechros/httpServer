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

void handle_http_protocol(const char *request_string, char *response_string, const int buffer_size)
{
    struct request_data request = detect_request_type(request_string);
    printf("Request Type : %s\n", request.string);

    const char *hello_res = "Hello, World!\r\n";
    switch (request.type)
    {
    case REQUEST_GET:
    case REQUEST_POST:
    case REQUEST_PUT:
    case REQUEST_DELETE:
        snprintf(response_string, buffer_size, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: text/plain\r\n\r\n%s", strlen(hello_res), hello_res);
        break;

    case REQUEST_OTHER:
    case REQUEST_PATCH:
    case REQUEST_HEAD:
    case REQUEST_OPTIONS:
    case REQUEST_TRACE:
        snprintf(response_string, buffer_size, "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n");
        break;

    default:
        break;
    }
}

struct request_data detect_request_type(const char *request_string)
{
    struct request_data request_list[] = {
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

    struct request_data request = request_list[0];
    size_t request_size = strlen(request_string);
    size_t request_list_size = sizeof(request_list) / sizeof(struct request_data);

    for (size_t i = 0; i < request_list_size; i++)
    {
        size_t request_data_str_size = strlen(request_list[i].string);
        for (size_t j = 0; j < request_size; j++)
        {
            if (strncmp(request_list[i].string, request_string + j, request_data_str_size) == 0)
            {
                request = request_list[i];
                break;
            }
        }
    }

    return request;
}