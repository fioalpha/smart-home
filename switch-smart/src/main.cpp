#include <Arduino.h>
#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

const char* SSID = "SmartHome"; 
const char* PASSWORD = "19750044"; 
  

const char* BROKER_MQTT = "192.168.1.150"; 
const char* USER_MQTT = "fioalpha";
const char* PASS_MQTT = "manuel";

int BROKER_PORT = 1883; 
 
WiFiClient espClient; 
PubSubClient MQTT(espClient); 

const int SWITCH_ONE = 0;
const int SWITCH_TWO = 2;    

const String BASE_TOPIC = "/switch/bedroom";
const String LWT_TOPIC = "tele/LWT";
const String POWER_TOPIC = "cmnd/POWER";
const String STAT_RESULT_TOPIC = "stat/RESULT";
const String STAT_CURRENT_TOPIC = "stat/CURRENT";

int lastStatusSwitchOne = LOW;
int lastStatusSwitchTwo = LOW;
int statusSwitchOne = LOW;
int statusSwitchTwo = LOW;
char bufferConvertIntToChar[32];

  
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{

     
}

char* getChipId() 
{
    uint32_t chipId = ESP.getChipId();
    itoa(chipId, bufferConvertIntToChar, 10);
    return bufferConvertIntToChar;
}

String concatTopic(String topic) 
{
    return BASE_TOPIC + topic;
}

void reconnectMQTT() 
{
  
    while (!MQTT.connected()) 
    {
        if (MQTT.connect(getChipId(), USER_MQTT, PASS_MQTT)) 
        {
            MQTT.publish(concatTopic(LWT_TOPIC).c_str(), "{\"online\": \"true\"}");
        } 
        else
        {
          Serial.print( "....");
            delay(2000);
        }
    }
}

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   
    MQTT.setCallback(mqtt_callback);            
}


void reconectWiFi() 
{
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); 
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.println("Connect");
    }
}

void initWiFi() 
{
    delay(10);
    reconectWiFi();
}

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_ONE, INPUT);
  pinMode(SWITCH_TWO, INPUT);
  initWiFi();
  initMQTT();
}

String status(int status) {
    if(status == HIGH) return "{ \"power\": \"OFF\"}";
    else return "{ \"power\": \"ON\" }";
}

void loop() 
{   
    if (!MQTT.connected()) reconnectMQTT();     
    else reconectWiFi();

    statusSwitchOne = digitalRead(SWITCH_ONE);

    if(statusSwitchOne != lastStatusSwitchOne) {
        MQTT.publish(concatTopic(POWER_TOPIC).c_str(), status(statusSwitchOne).c_str());
        lastStatusSwitchOne = statusSwitchOne;
        delay(300);
    }

    statusSwitchTwo = digitalRead(SWITCH_TWO);

    if(statusSwitchTwo != lastStatusSwitchTwo) {
        MQTT.publish(concatTopic(POWER_TOPIC).c_str(), status(statusSwitchTwo).c_str());
        lastStatusSwitchTwo = statusSwitchTwo;
        delay(300);
    }
    
    MQTT.loop();
}