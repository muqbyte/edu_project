// #include <ESP8266WiFi.h> //please uncomment
// #include <MQTT.h>    // BY Joel Gaehwiller //please uncomment

// Library and constants for DHT11
// #include "DHT.h"
#define DHTPIN D5    // Digital pin connected to the DHT sensor IO D5
DHT dht(DHTPIN,DHT11);
unsigned long previousMillis = 0;



#define sub1 "pendidikan/data/suraya/temp"          // Send data every set interval...publish everytime
#define sub2 "pendidikan/data/suraya/hum"     // Send data upon Request.....publish upon request
#define sub3 "mekanikal/data/tadir/button"      // Listen to request.....subscrice
const char ssid[]     = "SEA-IC";         // replace with your WiFi SSID
const char password[] = "seaic2022";          // replace with your WiFi password

const char clientId[] = "tadir123";              // replace with your MQTT Client Id
const char server[]   = "txio.uitm.edu.my";  // replace with your MQTT Broker txio.uitm.edu.my/seaic.uitm.edu.my

WiFiClient net;
MQTTClient client;

//ID
String loraId ="7xx"; //01 ~ 99

// Variables
float tmp, hmd;
//String txData;

// time constants
unsigned long lastMillis = 0;
///////////////////////////////////////////////


void messageReceived(String &topic, String &payload) {
//Serial.println("incoming: " + topic + " - " + payload); 
  Serial.println("Topic: " +  topic);
  Serial.println("Payload: " +  payload);

}


void connect() {
  
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

  Serial.print("Connecting to MQTT broker ...");
  while (!client.connect(clientId)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" connected!");

 // client.subscribe(topic);
 client.subscribe(sub1);
 client.subscribe(sub2);
 client.subscribe(sub3);
}


void checkConnect(){
  client.loop();

  if (!client.connected()) {
    connect();
  }
}


boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}


void setup() {
  // STEP 1: initialize Serial Monitor
  Serial.begin(115200);

  // STEP 2 - Initialize WiFi connectivity
  WiFi.begin(ssid, password);
  client.begin(server, net);
  client.onMessage(messageReceived);
  
  // STEP 3 - Connecting to MQTT broker
  connect();
}

void loop() {
  //check MQTT connection
  checkConnect();
  DataProcessing(2000);
}

void DataProcessing(unsigned long interval){
 unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval){
 previousMillis = currentMillis;
 hmd = dht.readHumidity();
 tmp = dht.readTemperature();
 if (isnan(hmd) || isnan(tmp)){
 Serial.println("Failed to read from DHT sensor.");
 return;
 }
 Serial.print("Humidity: ");
 Serial.print(hmd);
Serial.print(" %\t");
 Serial.print("Temperature: ");
 Serial.print(tmp);
 Serial.print(" *C\n");
 client.publish(sub1, String(tmp));
 client.publish(sub2, String(hmd));

}
}