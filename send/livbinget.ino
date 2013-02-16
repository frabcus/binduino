// (c) Copyright 2010-2012 MCQN Ltd.
// Released under Apache License, version 2.0
//
// Simple example to show how to use the HttpClient library
// Get's the web page given at http://<kHostname><kPath> and
// outputs the content to the serial port

#include <b64.h>
#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

// This example downloads an HTTP URL

// Name of the server we want to connect to
const char kHostname[] = "box.scraperwiki.com";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/aprruoq/c07af7ac02ee4d1/http/light.state";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

void livbinget_setup()
{
  Serial.println("Ethernet.begin: getting IP");
  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Ethernet.begin: failed, trying again in 10 seconds");
    delay(10000);
    flashDebug(3, 100, 50);
  }  
  Serial.println("Ethernet.begin: ok");
}

boolean livbinget_fetch()
{
  flashDebug(3, 5, 50); // polling net

  boolean ret = false;
  int err =0;
  
  EthernetClient c;
  HttpClient http(c);
  
  // Connect to the host
  err = http.get(kHostname, kPath);
  if (err != 0)
  {
    Serial.print("http.get: connect failed: ");
    Serial.println(err);
    return false;
  }
  Serial.println("http.get: connect ok");

  err = http.responseStatusCode();
  if (err >= 0)
  {
    Serial.print("http.responseStatusCode: ");
    Serial.println(err);

    // Usually you'd check that the response code is 200 or a
    // similar "success" code (200-299) before carrying on,
    // but we'll print out whatever response we get

    err = http.skipResponseHeaders();
    if (err >= 0)
    {
      int bodyLen = http.contentLength();
      Serial.print("http.contentLength: ");
      Serial.println(bodyLen);
      Serial.println();
      Serial.println("http: receiving body...");
    
      // Now we've got to the body, so we can print it out
      unsigned long timeoutStart = millis();
      char c;
      // Whilst we haven't timed out & haven't reached the end of the body
      while ( (http.connected() || http.available()) &&
             ((millis() - timeoutStart) < kNetworkTimeout) )
      {
          if (http.available())
          {
              c = http.read();
              // Print out this character
              Serial.print("http.read: got a char:");
              Serial.println(c);
              
              // R is for Recyle Now!
              // - is for do nothing
              if (c == 'R') {
                ret = true;
              }
             
              bodyLen--;
              // We read something, reset the timeout counter
              timeoutStart = millis();
          }
          else
          {
              // We haven't got any data, so let's pause to allow some to
              // arrive
              delay(kNetworkDelay);
          }
      }
    }
    else
    {
      Serial.print("Failed to skip response headers: ");
      Serial.println(err);
    }
  }
  else
  {    
    Serial.print("Getting response failed: ");
    Serial.println(err);
  }
  http.stop();
  return ret;
}



