#include <ESP8266WiFi.h>

#include "rest_client.h"

static const uint16_t TIMEOUT = 50000;

RestClient::RestClient()
{

}

RestClient::~RestClient()
{

}

void RestClient::sendHeader(const char* aHeaderName, const char* aHeaderValue)
{
    _client.print(aHeaderName);
    _client.print(": ");
    _client.println(aHeaderValue);
}

void RestClient::sendHeader(const char* aHeaderName, const int aHeaderValue)
{
    _client.print(aHeaderName);
    _client.print(": ");
    _client.println(aHeaderValue);
}

void RestClient::sendHeader(const char* aHeaderName)
{
    _client.println(aHeaderName);
}

void RestClient::request(http_request_t &aRequest, http_response_t &aResponse,
    http_header_t headers[], const char* aHttpMethod)
{
    aResponse.status = -1;

    bool connected = false;
    if(aRequest.hostname !=NULL) {
        connected = _client.connect(aRequest.hostname.c_str(), (aRequest.port) ? aRequest.port : 80);
    } else {
        connected = _client.connect(aRequest.ip, aRequest.port);
    }

    if(!connected) {
        _client.stop();

        return;
    }

    _client.print(aHttpMethod);
    _client.print(" ");
    _client.print(aRequest.path);
    _client.print(" HTTP/1.1\r\n");

    sendHeader("Connection", "close");
    if(aRequest.hostname != NULL)
    {
        sendHeader("HOST", aRequest.hostname.c_str());
    }

    if(aRequest.body != NULL)
    {
        sendHeader("Content-Length", (aRequest.body).length());
    } else if (strcmp(aHttpMethod, HTTP_METHOD_POST) == 0) {
        sendHeader("Content-Length", 0);
    }
    sendHeader("Cache-Control", "no-cache");

    sendHeader("accept", "application/json");

    sendHeader("User-Agent", "ES8266");

    if(strcmp(aHttpMethod, HTTP_METHOD_POST) == 0)
    {
        sendHeader("Content-Type", "application/x-www-form-urlencoded");
    }

    if (headers != NULL)
    {
        int i = 0;
        while (headers[i].header != NULL)
        {
            if(headers[i].value != NULL) {
                sendHeader(headers[i].header, headers[i].value);
            } else {
                sendHeader(headers[i].header);
            }
            i++;
        }
    }

    sendHeader("Connection", "close");

    _client.println();
    _client.flush();

    if(aRequest.body != NULL)
    {
        _client.println(aRequest.body);
    }

    memset(&buffer, 0, sizeof(buffer));

    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    unsigned long firstRead = millis();
    bool error = false;
    bool timeout = false;

    do {
        while (_client.available()) {
            char c = _client.read();
            lastRead = millis();

            if (c == -1) {
                error = true;
                break;
            }

            if(bufferPosition < sizeof(buffer)-1) {
                buffer[bufferPosition] = c;
            } else if ((bufferPosition == sizeof(buffer)-1)) {
                buffer[bufferPosition] = '\0';
                _client.stop();
                error=true;

            }
            bufferPosition++;
        }
        buffer[bufferPosition] = '\0';

        timeout = millis() - lastRead > TIMEOUT;

        if(!error && !timeout) {
            delay(200);
        }
    }while(_client.connected() && !timeout && !error);

    _client.stop();

    if(timeout) {
      Serial.println("Timed Out!");
        return;
    }

    String raw_response(buffer);

    String statusCode = raw_response.substring(9,12);

    int bodyPos = raw_response.indexOf("\r\n\r\n");
    if (bodyPos == -1) {
      Serial.println("Invalid Body.");
        return;
    }

    aResponse.body = "";
    aResponse.body += raw_response.substring(bodyPos + 4);
    aResponse.status = atoi(statusCode.c_str());

}
