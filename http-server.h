#ifndef _HTTPD_H___
#define _HTTPD_H___

#include <string.h>
#include <stdio.h>
#include <microhttpd.h>

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                     const char *url, const char *method,
                     const char *version, const char *upload_data,
                     size_t *upload_data_size, void **con_cls);

void start_server(int port);
void stop_server() ;
#endif