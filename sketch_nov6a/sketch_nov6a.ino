/*
  Simple GET client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a GET request

  created 14 Feb 2016
  modified 22 Jan 2019
  by Tom Igoe
  
  this example is in the public domain
 */
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <WiFiNINA.h>


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
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
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("making GET request");
  client.get("/test");

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  client.skipResponseHeaders();

  // Serial.println("---");
  // Serial.print((int)strtol("0x0F", NULL, 16));
  // Serial.println("---");

  while (true) {

    //switch 

    // int num = (int)strtol(0xFF, NULL, 16);
    // Serial.println(num);

    int c1 = client.read();
    int c2 = client.read();
    if (c1 == -1 || c2 == -1) {
      break;
    }

    char str[] = "0x00";
    str[2] = (char)c1;
    str[3] = (char)c2;

    Serial.println(str);
    Serial.println((int)strtol(str, NULL, 16));
    //Serial.println((char)c1);
    delay(1000);
  }

  //String response = client.responseBody();


  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Content length: ");
  Serial.println(client.contentLength());
  //Serial.print("Response: ");
  // Serial.println(response);
  Serial.println("Wait five seconds");
  delay(10000);
}