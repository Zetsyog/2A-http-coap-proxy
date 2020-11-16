#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "http-server.h"
#include "resource.h"
#include "util.h"


static struct MHD_Daemon *srv_daemon;
struct MHD_Response *route(const char *url);

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url, const char *method,
                         const char *version, const char *upload_data,
                         size_t *upload_data_size, void **con_cls) {
    struct MHD_Response *response;
    int ret;
    (void)cls;              /* Unused. Silent compiler warning. */
    (void)url;              /* Unused. Silent compiler warning. */
    (void)version;          /* Unused. Silent compiler warning. */
    (void)upload_data;      /* Unused. Silent compiler warning. */
    (void)upload_data_size; /* Unused. Silent compiler warning. */
    (void)con_cls;          /* Unused. Silent compiler warning. */

    if (0 != strcmp(method, "GET"))
        return MHD_NO;

    const char *error_str =
        "<html><body>An error occured<br> 404 Error : Not Found</body></html>";

    response = route(url);

    if (NULL != response) {
        MHD_add_response_header(response, "Content-Type", "text/html");
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);

        return ret;
    } else {
        log_error(NOERRNO, "404 : route not found");
        response = MHD_create_response_from_buffer(
            strlen(error_str), (void *)error_str, MHD_RESPMEM_PERSISTENT);
        if (response != NULL) {
            MHD_add_response_header(response, "Content-Type", "text/html");
            ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);

            return ret;
        } else {
            return MHD_NO;
        }
    }
}

/**
 *  Http router
 */
struct MHD_Response *route(const char *url) {
    struct MHD_Response *response;

    resource_t r = resource_get_by_name(url);

    if (r == -1)
        return NULL;

    log_info("find route %s", url);

    char val[100] = {0};
    resource_value(r, val);

    char buf[256] = {0};
    snprintf(buf, 256, "<html><body>%s</body></html>", val);

    response = MHD_create_response_from_buffer(strlen(buf), (void *)buf,
                                               MHD_RESPMEM_MUST_COPY);
    return response;
}

void print_address(int port) {
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    printf("Server started %shttp://%s:%d%s\n", "\033[92m",
           inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), port,
           "\033[0m");
}

void start_server(int port) {
    srv_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, port, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);

    if (NULL == srv_daemon) {
        perror("");
        exit(EXIT_FAILURE);
    }

    print_address(port);
}

void stop_server() {
    MHD_stop_daemon(srv_daemon);
}