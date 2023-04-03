
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>




// Replace the next variables with your SSID/Password combination
const char* ssid = "WORKSTATION";
const char* password = "Superman_27";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.11";

WiFiClient espClient;
PubSubClient client(espClient);

// Create variable to hold mqtt messages
#define MSG_BUFFER_SIZE  (1000)
char msg[MSG_BUFFER_SIZE];

// Connecting to the WIFI network
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP8266Client-"; clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), "mqtt", "")) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } 
    
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void loop() {
  // Connect to the mqtt client
  if (!client.connected()) 
  {
    reconnect();
  }
  
  client.loop();

  // Get the current object temperatur of the sensor
  float voltage = 256.2;
  float current = 1.01;
  float power = 600.2;
  float temp =182.30;
  int TestStatus =1;
int PanelInput = 1;
int StationStatus =1;
int Scycle=34;
int Tcycle=125;
int Fault =3;

int rssi  = WiFi.RSSI();

  //temperature="+String(objt)

    //String message = String("weather,location=us temperature="+String(objt));
  // Create the message that will be send using mqtt
  //String message = String("measurement,Device=Station-001,Location=Baddi,Sensor=Panel-001 Voltage=235.00,Current=1.01,Power=263.00,Temperature=154.40,TestStatus=1i,PanelInput=1i,StationStatus=1i,StationCycles=28i,ThermostatCycle=320i,Fault=2i,WiFiStrength=-45i");
  String message = String("measurement,Device=Station-001,Location=Baddi,Sensor=Panel-001 Voltage="+String(voltage)+",Current="+String(current)+",Power="+String(power)+",Temperature="+String(temp)+",TestStatus="+String(TestStatus)+"i,PanelInput="+String(PanelInput)+"i,StationStatus="+String(StationStatus)+"i,StationCycles="+String(Scycle)+"i,ThermostatCycle="+String(Tcycle)+"i,Fault="+String(Fault)+"i,WiFiStrength="+String(rssi)+"i");
  
  message.toCharArray(msg, message.length());
  Serial.println(msg);
  
  
  
  // Send the message on the sensors topic
  client.publish("philips/sensor", msg);

  delay(500);
}
