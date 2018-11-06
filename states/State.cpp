#include "state.h"
#include "LEDDriver.h"
#include "rest_client.h"

State::State(RestClient * restClient, LEDDriver * ledDriver, int relayPin){
    _restClient = restClient;
    _leddriver = ledDriver;
    _relayPin = relayPin;
}

virtual void StartUp::operation()
{
  if(tryCount == 0)
  {
    Serial.print("Attemping to connect to WEP network, SSDI: ");
    Serial.print()
  }
  Serial.print(".");
}

virtual State StartUp::nextState(MachineInputs inputs)
{
  if(inputs.wifiStatus == WL_IDLE_STATUS)
    return this;
  if(inputs.wifiStats == WL_CONNECTED)
    return StandBy(_restClient, _leddriver, relayPin);
}
