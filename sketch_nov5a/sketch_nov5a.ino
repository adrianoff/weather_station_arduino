#define SECRET_SSID ""
#define SECRET_PASS ""

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

const char input[] = "{ \"a\":[true, 42, \"apple\"]}";
StaticJsonDocument<100> doc;

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;  
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(LED_BUILTIN, OUTPUT);

  DeserializationError error = deserializeJson(doc, input);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  int data_0 = doc["a"][1];
  Serial.println(data_0);

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    Serial.print("WIFI status: ");
    Serial.println(status);
    delay(5000);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Device IP Addresss: ");
  Serial.println(ip);

  char serverAddress[] = "adrianov.pro";
  int port = 80;
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, serverAddress, port);

  Serial.println("making GET request");
  client.get("/");

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Done.");
}

// the loop function runs over and over again forever
void loop() {
  //Serial.println("TEST1");
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(2000);                      
  digitalWrite(LED_BUILTIN, LOW);   
  delay(2000);             
  LowPower.deepSleep(10000);
}