#ifndef __state_h__
#define __state_h__

class LEDDriver;
class MachineID;
class RestClient;

typedef struct {
  int wifiStats, cardPreset, validCard, machineId;

} MachineInputs;


class State {
protected:
  MachineInputs *_machineInputs;
  LEDDriver *_leddriver;
  int _relayPin;

public:
  state(MachineInputs *, LEDDriver *, int);

  virtual void operation();
  virtual State nextState(MachineInputs *inputs);
}

class StartUp : State {
}

class StandBy : State {

}

class ReadCard : State {

}

class ValidateCard : State {

}

class LogUnlock : State {

}

class LogLock: State {

}

class LogInvalid : State {

}

class Unlock : State {

}


#endif
