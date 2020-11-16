# HTTP/CoAP Proxy

This is a simple HTTP/CoAP proxy written in C

Features :
* A simple HTTP web server
* Multi-devices CoAP Endpoints
* Cache for each endpoint

## Usage

```c
// Register your routes with
resource_t r1 = resource_create("coap_ip", "coap_res_name", "http_route", cache_duration);
// this will query for every http request to YOUR_IP/http_route the coap resource with uri coap_res_name on ip coap_ip
// caching it if cache_duration > 0 (recommended)

// Then call (non blocking)
start_server(port);

// Whenever you want to stop your server use
stop_server();

// Don't forget to free memory before leaving the program
resource_destroy(handle);
```

For a working example see _main.c_, the _Makefile_ and the binary _bin/proxy_.

## Libraries

This software is built using [GNU Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) and [LibCoAP](https://libcoap.net/)


