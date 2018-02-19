#ifndef __rest_client_h__
#define __rest_client_h__

#include <ESP8266WiFi.h>

typedef struct {
    const char* header;
    const char* value;
} http_header_t;

typedef struct {
    String hostname;
    IPAddress ip;
    String path;
    int port;
    String body;
} http_request_t;

typedef struct
{
    int status;
    String body;
} http_response_t;

class RestClient {
public:
    char buffer[1024];

    RestClient();
    ~RestClient();

    void get(http_request_t &aRequest, http_response_t &aResponse)
    {
        request(aRequest, aResponse, (http_header_t*)NULL, HTTP_METHOD_GET);
    }

    void post(http_request_t &aRequest, http_response_t &aResponse)
    {
        request(aRequest, aResponse, (http_header_t*)NULL, HTTP_METHOD_POST);
    }

    void get(http_request_t &aRequest, http_response_t &aResponse,
        http_header_t headers[])
    {
        request(aRequest, aResponse, headers, HTTP_METHOD_GET);
    }

    void post(http_request_t &aRequest, http_response_t &aResponse,
        http_header_t headers[])
    {
        request(aRequest, aResponse, headers, HTTP_METHOD_POST);
    }

private:
    const char* HTTP_METHOD_GET = "GET";
    const char* HTTP_METHOD_POST = "POST";
    const char* HTTP_METHOD_PUT = "PUT";
    const char* HTTP_METHOD_DELETE = "DELETE";
    const char* HTTP_METHOD_PATCH = "PATCH";

    WiFiClient _client;
    unsigned long _port;
    char * _host;


    void request(http_request_t &aRequest, http_response_t &aResponse, http_header_t header[], const char *);
    void sendHeader(const char *, const char *);
    void sendHeader(const char *, const int);
    void sendHeader(const char *);
};

#endif