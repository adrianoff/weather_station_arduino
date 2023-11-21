/**
 *  @filename   :   epd7in5-demo.ino
 *  @brief      :   7.5inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include "epd7in5_V2.h"


char ssid[] = "adrianov_wifi";
char pass[] = "31415926";
char serverAddress[] = "adrianov.pro";  // server address
int port = 3333;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
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
        delay(10000);
        return;
    }

    client.get("/test");
    int statusCode = client.responseStatusCode();
    client.skipResponseHeaders();

    long int i = 0;
    int c1, c2 = -1;
    // epd.SendCommand(0x13);
    // while (true) {
    //   i++;
    //   c1, c2 = -1;

    //   if (i < 100) {
    //     c1 = client.read();
    //     c2 = client.read();
    //     Serial.println(i);
    //   }

    //   if (c1 == -1 || c2 == -1 || i > 48000) {
    //     epd.SendData(0x00);
    //     //Serial.print("0x00"); Serial.print(" "); Serial.print(i); Serial.print("\n");
    //   } else {
    //     char str[] = "0x00";
    //     str[2] = (char)c1;
    //     str[3] = (char)c2;

    //     Serial.print(str); Serial.print(" "); Serial.print(i); Serial.print("\n");
    //     int p = (int)strtol(str, NULL, 16);
    //     epd.SendData(p);
    //   }
    // }

    epd.SendCommand(0x13);
    for(unsigned long i=0; i<48000; i++)	{
        if (i < 50) {
          c1 = client.read();
          c2 = client.read();

          char str[] = "0x00";
          str[2] = (char)c1;
          str[3] = (char)c2;

          Serial.print(str); Serial.print(" "); Serial.print(i); Serial.print("\n");
          int p = (int)strtol(str, NULL, 16);
          epd.SendData(p);
        } else {
          epd.SendData(0x00);
        }
    }
    epd.SendCommand(0x12);
    delay(100);
    epd.WaitUntilIdle();

    delay(5000);
    epd.Clear();

    epd.Sleep();
    delay(20000);
}

