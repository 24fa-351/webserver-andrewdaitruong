#include "http_message.h"

void read_http_client_message(int client_sock, 
    http_client_message_t** msg,
    http_read_result_t* result)
    {
        *msg = malloc(sizeof(http_client_message_t));
    }