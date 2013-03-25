#include <VirtualWire.h>

int inPin = 7;
int elPowerPin = 13;
int elSwitchPin = 4;

boolean lightState = false;
boolean newLightState = false;

//VW_MAX_MESSAGE_LEN
uint8_t buf[1];
uint8_t buflen = 1;
 
void setup() {
  // serial for debugging only
  Serial.begin(9600);
  Serial.println("setup: bin collection receiving");

  pinMode(elPowerPin, OUTPUT);
  vw_set_rx_pin(inPin);
  vw_setup(600);	 // Bits per sec
  vw_rx_start();
  
  pinMode(elSwitchPin, OUTPUT);
  digitalWrite(elSwitchPin, LOW);
}

void loop() {
  if (vw_get_message(buf, &buflen)) { // Non-blocking
    Serial.print( char(buf[0]));
    Serial.println(buf[0]);

    // R = empty your bin!
    if (buf[0] == 'R') {
      newLightState = true;
    } else {
      newLightState = false;
    }
    if (newLightState != lightState) {
      lightState = newLightState;
      if (newLightState) {
        digitalWrite(elPowerPin, HIGH);
        digitalWrite(elSwitchPin, LOW);
        delay(500);
        digitalWrite(elSwitchPin, HIGH);
      } else {
        digitalWrite(elPowerPin, LOW);
      }
    }
  }
}



