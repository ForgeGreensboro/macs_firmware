#ifndef __macs_h__
#define __macs_h__

#include <IPAddress.h>
#include "LEDDriver.h"

class Command;
class SeeedRFID;
class RestClient;
class String;
class MachineID;


class Macs {
public:
    Macs();
    Macs(const char * &, const char * &, const char * &);
    ~Macs();

    SeeedRFID * getRfId();

    void init();
    void run();

    void setSSID(char * ssid) { _ssid = ssid; }
    char * getSSID() { return _ssid; }
    void setPassword(char * password) { _password = password; }
    char * getPassword() { return _password; }
    void setHostname(char * hostname) { _hostname = hostname; }
    char * getHostname() { return _hostname; }
    int getStatus() { return _status; }
    void setMachineId(unsigned long machine_id) { _machine_id = machine_id; }
    unsigned long getMachineId() { return _machine_id; }
    unsigned long getCurrentTag() { return _currentTag; }
    bool getRelayState() { return _relayState; }

    LEDDriver * getLEDDriver();

private:
    const int RFID_RX_PIN = 13;
    const int RFID_TX_PIN = 12;
    const int RELAY_PIN = 14;
    const int SERAL_SPEAD = 115200;
    const int EEPROM_SIZE = 2048;

    char * _ssid;
    char * _password;
    char * _hostname;
    IPAddress _remoteAddress;

    unsigned long _currentTag = 0;
    bool wifiStatusFlag = false;

    int _status = 0;
    bool _relayState = false;

    unsigned long _machine_id = 12;

    Command *_cmd = nullptr;
    SeeedRFID *_rfid = nullptr;
    RestClient *_restClient = nullptr;
    MachineID *_machineid = nullptr;
    LEDDriver *_leddriver = nullptr;

    char * eepromRead(int, int);
    bool validateCard(unsigned long card);

    void logValid(unsigned long card);
    void logInvalid(unsigned long card);
    void logLock();

    char _url[64];
    const char * _pattern = "/api/members/%d/machines/%d/";
};

#endif
