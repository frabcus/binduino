#include <VirtualWire.h>

int outputPin = 7;
int debugPin = 8; 
int binLightStatePin = 9;
int debugButtonPin = 10;

int count = 0;

boolean debugButtonState = false;
boolean binLightOn = false;
boolean netSetup = false;

// in seconds
int WEB_POLL_GAP = 60;

// an LED for debugging
void flashDebug(int c, int delayOn, int delayOff) {
  for (int i = 0; i < c; i++) {
    digitalWrite(debugPin, HIGH);
    delay(delayOn);
    digitalWrite(debugPin, LOW);
    delay(delayOff);
  }
}

void setup() {
  pinMode(debugPin, OUTPUT);
  flashDebug(1, 5, 50); // start

  // serial for debugging only
  Serial.begin(9600); 
  Serial.println("setup: bin collection sending");

  // virtual wire is a reliable protocol to run over the
  // wire (more resilient to noise than the built in UART)
  vw_set_tx_pin(outputPin);
  vw_setup(600);

  pinMode(binLightStatePin, OUTPUT);

  pinMode(debugButtonPin, INPUT);
  debugButtonState = digitalRead(debugButtonPin);
}

void loop() {
  uint8_t a = '-';
  uint8_t b = 'R';

  count++;

  // short circuit if in debug mode
  if (debugButtonState) {
    if ((count % 8) < 7) {
      binLightOn = true;
    } 
    else {
      binLightOn = false;
    }
  } 
  else {
    // setup the ethernet to get Liverpool bin collection times 
    if (!netSetup) { 
      livbinget_setup();
      flashDebug(2, 5, 50); // got mac and so on
      netSetup = true;
    }

    if (count % WEB_POLL_GAP == 1) {
      binLightOn = livbinget_fetch();
    }
  }

  if (binLightOn) {
    digitalWrite(binLightStatePin, HIGH);
    vw_send(&b, 1);
  } else {
    digitalWrite(binLightStatePin, LOW);
    vw_send(&a, 1);
  }
  vw_wait_tx();

  // Second clocks
  delay(1000);
}




