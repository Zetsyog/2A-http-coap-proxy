#ifndef _HTTPD_H___
#define _HTTPD_H___

#include <string.h>
#include <stdio.h>
#include <microhttpd.h>

/***
 * @brief Starts the http server daemon
 * Non blocking
 */
void start_server(int port);

/**
 * @brief Stops the http server daemon
 */
void stop_server() ;
#endif