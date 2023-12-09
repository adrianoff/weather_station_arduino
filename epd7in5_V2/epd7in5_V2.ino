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
  Serial.begin(9600);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  Serial.print("CONNECTED");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}


void loop() {  
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
    char base_str[] = "0x00";

    epd.SendCommand(0x13);
    for(unsigned long i=0; i<96; i++) {
      offset = i*limit;

      sprintf(url_string, "/data?offset=%lu&limit=%d", offset, limit);
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

