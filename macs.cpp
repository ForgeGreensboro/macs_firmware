#include <SeeedRFID.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include "command.h"
#include "macs.h"
#include "rest_client.h"

#define RFID_RX_PIN 13
#define RFID_TX_PIN 12

#define SSID_OFFSET 0
#define SSID_LENGTH 32

#define PASSWORD_OFFSET SSID_OFFSET + SSID_LENGTH
#define PASSWORD_LENGTH 54

#define SERVER_ADDRESS PASSWORD_OFFSET + PASSWORD_LENGTH
#define SERVER_ADDRESS_LENGTH 256

#define MACHINE_ID SERVER_ADDRESS + SERVER_ADDRESS_LENGTH
#define MACHINE_LENGTH 32

Macs::Macs()
{
   _rfid = new SeeedRFID(RFID_RX_PIN, RFID_TX_PIN);
   _restClient = new RestClient();

   pinMode(RFID_TX_PIN, INPUT);
   pinMode(RELAY_PIN, OUTPUT);
}

Macs::~Macs()
{
    _rfid = nullptr;
}

void Macs::init()
{
   _cmd = new Command(this);

    Serial.begin(115200);
    EEPROM.begin(512);

    Serial.println();
    Serial.println();
    Serial.println();

    Serial.print("EEPROM SSID: ");
    Serial.println(eepromRead(SSID_OFFSET, SSID_LENGTH));

      // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();

    Serial.println("Connect to WIFI");
    Serial.println();
    Serial.println();

    Serial.print("Connecting to ");
    Serial.println(_ssid);

    _status = WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Netmask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
}

void Macs::run()
{

    // Serial.print("Relay State");
    // Serial.println(_relayState ? "On" : "Off");
    bool cardIsAvailable = _rfid->isAvailable();
    if (
        (digitalRead(RFID_TX_PIN) == 0 || cardIsAvailable )
        && _currentTag != 0
        && _relayState
        )
    {

         Serial.println("Turning off Relay, and Locking machine.");
         _relayState = false;
         _currentTag = 0;

    }
    if(cardIsAvailable)
    {
        _currentTag = _rfid->cardNumber();

        Serial.print("RFID card number: ");
        Serial.println(_currentTag);

        Serial.println("verifying member.");

        _relayState = validateCard(_currentTag);

    }

    if (Serial.available() > 0) {
       if (_cmd->getCommandLineFromSerialPort()) _cmd->DoMyCommand();
    }

    digitalWrite(RELAY_PIN, _relayState ? 1 : 0);
}

char * Macs::eepromRead(int address, int length)
{
  char value[length];

  for(int x = 0; x < length; x++)
  {
    value[x] = EEPROM.read(address + x);
  }

  return value;
}

bool Macs::validateCard(unsigned long card)
{
    memset(_url, 0, sizeof(_url));

    sprintf(_url, _pattern, card, _machine_id);

    http_request_t aRequest;
    http_response_t aResponse;

    aRequest.ip = IPAddress(192,168,1,111);
    aRequest.hostname = "192.168.1.111";
    aRequest.path = _url;
    aRequest.port = 80;

    _restClient->get(aRequest, aResponse);

    Serial.println(_url);
    Serial.println(aResponse.status);

    return aResponse.status == 200;
}