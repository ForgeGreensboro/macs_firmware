#include "configure.h"
#include "macs.h"

#define SSID_OFFSET 0
#define SSID_LENGTH 32

#define PASSWORD_OFFSET SSID_OFFSET + SSID_LENGTH
#define PASSWORD_LENGTH 54

#define SERVER_ADDRESS PASSWORD_OFFSET + PASSWORD_LENGTH
#define SERVER_ADDRESS_LENGTH 256

#define MACHINE_ID SERVER_ADDRESS + SERVER_ADDRESS_LENGTH
#define MACHINE_LENGTH 32



char * eepromRead(int, int);
Macs app(SSID, PASSWORD, HOST_ADDRESS);

void setup() {

  app.init();

}

void loop() {

  app.run();
}
