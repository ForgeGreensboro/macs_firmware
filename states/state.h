#ifndef __state_h__
#define __state_h__

class LEDDriver;
class RestClient;

typedef struct {
  int wifiStats, cardPreset, validCard, machineId;

} MachineInputs;


class State {
protected:
  RestClient *_restClient;
  LEDDriver *_leddriver;
  int _relayPin;

public:
  State(RestClient *, LEDDriver *, int);

  virtual void operation();
  virtual State nextState(MachineInputs inputs);
};

class StartUp : State {
protected:
  int tryCount = 0;
};

class StandBy : State {

};

class ReadCard : State {

};

class ValidateCard : State {

};

class LogUnlock : State {

};

class LogLock: State {

};

class LogInvalid : State {

};

class Unlock : State {

};


#endif
