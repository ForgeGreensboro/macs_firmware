#include <SeeedRFID.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <sstream>

#include "command.h"
#include "macs.h"
#include "rest_client.h"

#include "MachineID.h"
#include "LEDDriver.h"
#include <Ticker.h>
#include "states/state.h"

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

void wifiStatusFlip(void);

void cardPresent(void);
void cardNotPresent(void);
volatile byte cardPresentActive = 0;
int numberofCardPresentInterrupts = 0;

volatile byte cardMissingActive = 0;
int numberofCardMissingIterrupts = 0;

Macs::Macs()
{
   _rfid = new SeeedRFID(RFID_RX_PIN, RFID_TX_PIN);
   _restClient = new RestClient();

   _machineid = new MachineID();
   _leddriver = new LEDDriver();

   pinMode(RFID_TX_PIN, INPUT);
   pinMode(RELAY_PIN, OUTPUT);
}

Macs::Macs(const char * &ssid, const char * &password, const char *  &hostname) : Macs()
{
  _ssid = (char *)ssid;
  _password = (char *)password;
  _hostname = (char *) hostname;
}

Macs::~Macs()
{
    _rfid = nullptr;
    _restClient = nullptr;
    _machineid = nullptr;
}

void Macs::init()
{
   _cmd = new Command(this);

    Serial.begin(115200);
    EEPROM.begin(512);


      // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();

    Serial.println("Connect to WIFI");
    Serial.println();
    Serial.println();

    Serial.print("Connecting to ");
    Serial.println(_ssid);

    attachInterrupt(RFID_TX_PIN, cardPresent, RISING);
    // attachInterrupt(RFID_TX_PIN, cardNotPresent, FALLING);

    _leddriver->setWifiStatus(1);

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

    Serial.print("Machine ID set on ioexpander: ");
    Serial.println(_machineid->getId());

    _machine_id = (unsigned long) _machineid->getId();

    _leddriver->setWifiStatus(0);

}

void Macs::run()
{
    if(cardPresentActive > 0){
      cardPresentActive--;
      numberofCardPresentInterrupts++;
      numberofCardMissingIterrupts--;

      Serial.print("An interrupt has occured. Total: ");
      Serial.println(numberofCardPresentInterrupts);
    }

    if(cardMissingActive > 0)
    {
      cardMissingActive--;
      numberofCardMissingIterrupts++;
      numberofCardPresentInterrupts--;
    }

    bool cardIsAvailable = _rfid->isAvailable();
    if (
        (digitalRead(RFID_TX_PIN) != 1 || cardIsAvailable ) && _relayState
        )
    {

        _leddriver->setRelayStatus(0);
        _leddriver->setSystemReadyStatus(0);
         Serial.println("Turning off Relay, and Locking machine.");
         _relayState = false;
         _currentTag = 0;
         logLock();
         delay(50);

    }
    if(cardIsAvailable && digitalRead(RFID_TX_PIN) == 1)
    {
        _currentTag = _rfid->cardNumber();

        Serial.print("RFID card number: ");
        Serial.println(_currentTag);

        Serial.println("verifying member.");

        _relayState = validateCard(_currentTag);

        if(_relayState)
        {
          Serial.println("Machine is unlocked!");
          _leddriver->setSystemReadyStatus(1);
          logValid(_currentTag);
          delay(50);
        }else {
          Serial.println("Machine is locked!");
          _leddriver->setRelayStatus(1);
         logInvalid(_currentTag);
         delay(50);
        }
    }

    if (Serial.available() > 0) {
       if (_cmd->getCommandLineFromSerialPort()) _cmd->DoMyCommand();
    }
delay(50);
    _leddriver->setRelayStatus(_relayState ? 1 : 0 );
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

    Serial.println(_url);
    // Ticker flipper;

    http_request_t aRequest;
    http_response_t aResponse;

    aRequest.hostname = _hostname;
    aRequest.path = _url;
    aRequest.port = 80;

    _leddriver->setWifiStatus(1);

    _restClient->get(aRequest, aResponse);

    _leddriver->setWifiStatus(0);

    Serial.println(aResponse.status);

    return aResponse.status == 200;
}

void Macs::logValid(unsigned long card)
{
  const String url = "/api/logs/";

  http_request_t aRequest;
  http_response_t aResponse;

  aRequest.hostname =_hostname;
  aRequest.path = url;
  aRequest.port = 80;

  aRequest.body = "machine=";
  aRequest.body += _machine_id;
  aRequest.body += "&member=";
  aRequest.body += card;
  aRequest.body += "&event=1";


  _leddriver->setWifiStatus(1);
  _restClient->post(aRequest, aResponse);
  _leddriver->setWifiStatus(0);
}

void Macs::logInvalid(unsigned long card)
{
  const String url = "/api/logs/";

  http_request_t aRequest;
  http_response_t aResponse;

  aRequest.hostname =_hostname;
  aRequest.path = url;
  aRequest.port = 80;

  aRequest.body = "machine=";
  aRequest.body += _machine_id;
  aRequest.body += "&member=";
  aRequest.body += card;
  aRequest.body += "&event=3";


  _leddriver->setWifiStatus(1);
  _restClient->post(aRequest, aResponse);
  _leddriver->setWifiStatus(0);
}

void Macs::logLock()
{
  const String url = "/api/logs/";

  http_request_t aRequest;
  http_response_t aResponse;

  aRequest.hostname =_hostname;
  aRequest.path = url;
  aRequest.port = 80;

  aRequest.body = "machine=";
  aRequest.body += _machine_id;
  aRequest.body += "&member=";
  aRequest.body += _currentTag;
  aRequest.body += "&event=2";


  _leddriver->setWifiStatus(1);
  _restClient->post(aRequest, aResponse);
  _leddriver->setWifiStatus(0);
}

LEDDriver * Macs::getLEDDriver() { return _leddriver; }

void cardPresent(void) {
  cardPresentActive++;
}

void cardNotPresent(void) {
  cardMissingActive++;
}
