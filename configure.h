#ifndef __configure_h__
#define __configure_h__

class Configure {
public:
    char * getSSID();
    char * getPassword();
    char * getHostName();
    int getMachineId();
private:
};

#endif