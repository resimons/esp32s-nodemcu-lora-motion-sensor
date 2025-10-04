#include <SPI.h>              // include libraries
#include <WiFi.h>
#include <LoRa.h>

#define LORA_FREQ 433E6

const int PIR_SENSOR_INPUT_PIN = GPIO_NUM_35;  /* PIR sensor O/P pin */

int warm_up;
int lastMotion;

char ssid[23];
uint8_t macAddr[6];
char sMacAddr[18];

void sendMessage(String outgoing);
void publish_alive();
void publish_motion(uint8_t motion);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  pinMode(PIR_SENSOR_INPUT_PIN, INPUT);

   // Get deviceId
  snprintf(ssid, 23, "MCUDEVICE-%llX", ESP.getEfuseMac());
  WiFi.macAddress(macAddr);   // The MAC address is stored in the macAddr array.
  snprintf(sMacAddr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
  Serial.println(ssid);
  Serial.println(sMacAddr);

  LoRa.setPins(SS, GPIO_NUM_2, GPIO_NUM_4);
  LoRa.setSPIFrequency (20000000);
  LoRa.setTxPower (20);
  if (!LoRa.begin(LORA_FREQ)) {
    delay (5000);
    ESP.restart();
  }

  Serial.println("Lora found");

  LoRa.setPreambleLength(8);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(0x12);

  publish_alive();
}

void loop()
{
  int sensor_output;
  sensor_output = digitalRead(PIR_SENSOR_INPUT_PIN);
  if( sensor_output == LOW )
  {
    if( warm_up == 1 )
     {
      Serial.print("Warming Up\n\n");
      warm_up = 0;
      delay(2000);
    }
    Serial.print("No object in sight\n\n");

    if (lastMotion == 1) {
      Serial.print("Motion has changed to stopped.\n\n");
      lastMotion = 0;
      publish_motion(lastMotion);
    }
    delay(1000);
  }
  else
  {
    Serial.print("Object detected\n\n");   
    warm_up = 1;

    if (lastMotion == 0) {
      Serial.print("Motion has changed to detected.\n\n");
      lastMotion = 1;
      publish_motion(lastMotion);
    }
    delay(1000);
  } 
}

void publish_alive() {

  // maximum message length 128 Byte
  String payload = "";
  payload += "{\"device\":";
  payload += "\"";
  payload += ssid;
  payload += "\"";
  payload += ",\"type\":";
  payload += "\"iamalive\"";
  payload += ",\"mac\":";
  payload += "\"";
  payload += sMacAddr;
  payload += "\"";
  payload += "}";
  sendMessage(payload);
}

void publish_motion(uint8_t motion) {
  Serial.println("Sending motion data");

  String payload = "";
  payload += "{\"motion\":";
  payload += String(motion);
  payload += ",\"sensor\":";
  payload += "\"ks0052\"";
  payload += ",\"device\":";
  payload += "\"";
  payload += ssid;
  payload += "\"";
  payload += ",\"mac\":";
  payload += "\"";
  payload += sMacAddr;
  payload += "\"";
  payload += "}";

  sendMessage(payload);
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}

