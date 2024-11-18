#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include "epd7in5_V2.h"


char ssid[] = "adrianov_wifi";
char pass[] = "31415926";
char serverAddress[] = "adrianov.pro";
int port = 3333;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  /*********************/
    // Serial.begin(9600);
    // char url_string[256];
    // String resp;

    // String sessionId;
    // char sessionIdBuf[6] = "fsddfs";

    // sessionId = "asdfasd";

    // Serial.println("YTEYS");
    // sprintf(url_string, "/pixels?offset=%lu&limit=%d&session_id=%6d", 1, 2, sessionIdBuf);
    // Serial.println(sessionId);
    // Serial.println(url_string);
    // return;
  /*********************/


  Serial.begin(9600);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  Serial.println("CONNECTED");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}


void loop() {  
    /*********/
    delay(10000);
    //return;
    /*********/
    Epd epd;
    
    Serial.println("e-Paper init...");
    if (epd.Init() != 0) {
        Serial.println("e-Paper init failed.");
        delay(1000);
        return;
    }

    epd.Clear();

    unsigned long offset = 0;
    unsigned long limit = 1000;
    char url_string[256];
    String resp;

    String sessionId;
    char sessionIdBuf[33];

    char base_str[] = "0x00";

    Serial.println("Getting Session");
    client.get("/start_session");
    int statusCode = client.responseStatusCode();
    client.skipResponseHeaders();
    sessionId = client.responseBody();
    
    Serial.println("SessionId:");
    Serial.println(sessionId);

    sessionId.getBytes(sessionIdBuf, 33);
    Serial.println(sessionIdBuf);

    epd.SendCommand(0x13);
    for(unsigned long i=0; i<96; i++) {
      offset = i*limit;

      sprintf(url_string, "/pixels?offset=%lu&limit=%d&session_id=", offset, limit);
      strcat(url_string, sessionIdBuf);
      Serial.println(offset);
      Serial.println(url_string);
      
      client.get(url_string);
      int statusCode = client.responseStatusCode();
      client.skipResponseHeaders();

      resp = client.responseBody();
      //Serial.println(offset);
      for (int k=0; k<limit; k=k+2) {
        base_str[2] = resp[k];
        base_str[3] = resp[k+1];
        int p = (int)strtol(base_str, NULL, 16);
        epd.SendData(p);
      }
    }
    epd.SendCommand(0x12);
    delay(100);
    epd.WaitUntilIdle();

    epd.Sleep();

    delay(10000);
}

