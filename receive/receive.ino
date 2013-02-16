#include <VirtualWire.h>

int inPin = 7;
int ledPin = 13;

//VW_MAX_MESSAGE_LEN
uint8_t buf[1];
uint8_t buflen = 1;
 
void setup() {
  // serial for debugging only
  Serial.begin(9600);
  Serial.println("setup: bin collection receiving");

  pinMode(ledPin, OUTPUT);
  vw_set_rx_pin(inPin);
  vw_setup(600);	 // Bits per sec
  vw_rx_start();
  
}

void loop() {
  if (vw_get_message(buf, &buflen)) { // Non-blocking
    Serial.println( char(buf[0]));
    Serial.println(buf[0]);

    // R = empty your bin!
    if (buf[0] == 'R') {
      digitalWrite(ledPin, HIGH);
    } else {
      // - = nothing
      digitalWrite(ledPin, LOW);
    }
  }
}



