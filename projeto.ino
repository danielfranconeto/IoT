#include <DHT.h>
#include <ESP8266WiFi.h>

const char *ssid =  "VIVO-D810";
const char *pass =  "7hWbJJAKPh";

const char* server = "api.thingspeak.com";
String apiKey = "VF54H383HQ1MUYX4";

String thingtweetAPIKey="4JSK0RS6A3RSFRD5";

WiFiClient client;

#define DHTPIN 5          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (t > 25.0f) {
    updateTwitterStatus("Tweet enviado pela NodeMCU sobre a temperatura acima de 25ºC no Lab de Robótica");
  }
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
    if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();
  Serial.println("Waiting...");
  delay(1000);

  if( h < 30){
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
  }else{
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
  
}

void updateTwitterStatus(String tsData)
{
  if (client.connect(server, 80))
  {
    // Create HTTP POST Data
    tsData = "api_key=" + thingtweetAPIKey + "&status=" + tsData;
    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
  }
}
