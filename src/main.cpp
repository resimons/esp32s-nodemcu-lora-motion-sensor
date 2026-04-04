#include <esp_wifi.h>
#include <LoRa.h>

#define LORA_FREQ 433E6
#define WARMUP_MS 30000  // PIR sensor stabilisation period (ms)
#define SPI_FREQ 20E6

const int PIR_SENSOR_INPUT_PIN = GPIO_NUM_35; /* PIR sensor O/P pin */

bool lastMotion = false;
unsigned long warmupEnd = 0;


constexpr size_t SSID_LEN = 23;
constexpr size_t MAC_STR_LEN = 18;

char ssid[SSID_LEN];
uint8_t macAddr[6];
char sMacAddr[MAC_STR_LEN];

void sendMessage(const String &outgoing);

void publish_alive();

void publish_motion(uint8_t motion);

void setup() {
    Serial.begin(115200);

    pinMode(PIR_SENSOR_INPUT_PIN, INPUT);

    // Get deviceId
    snprintf(ssid, SSID_LEN, "MCUDEVICE-%llX", ESP.getEfuseMac());
    esp_read_mac(macAddr, ESP_MAC_WIFI_STA);
    snprintf(sMacAddr, MAC_STR_LEN, "%02x:%02x:%02x:%02x:%02x:%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4],
             macAddr[5]);
    Serial.println(ssid);
    Serial.println(sMacAddr);

    LoRa.setPins(SS, GPIO_NUM_2, GPIO_NUM_4);
    LoRa.setSPIFrequency(SPI_FREQ);
    if (!LoRa.begin(LORA_FREQ)) {
        delay(5000);
        ESP.restart();
    }

    Serial.println("Lora found");

    LoRa.setTxPower(20);
    LoRa.setPreambleLength(8);
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setSyncWord(0x12);

    warmupEnd = millis() + WARMUP_MS;
    Serial.println("PIR warming up...");

    publish_alive();
}

void loop() {
    if (millis() < warmupEnd) {
        delay(1000);
        return;
    }

    const int sensor_output = digitalRead(PIR_SENSOR_INPUT_PIN);
    if (sensor_output == LOW) {
        Serial.println("No object in sight");

        if (lastMotion) {
            Serial.println("Motion has changed to stopped.");
            lastMotion = false;
            publish_motion(0);
        }
    } else {
        Serial.println("Object detected");

        if (!lastMotion) {
            Serial.println("Motion has changed to detected.");
            lastMotion = true;
            publish_motion(1);
        }
    }
    delay(1000);
}

void publish_alive() {
    char payload[128];
    snprintf(payload, sizeof(payload),
             R"({"device":"%s","type":"iamalive","mac":"%s"})",
             ssid, sMacAddr);
    sendMessage(payload);
}

void publish_motion(uint8_t motion) {
    Serial.println("Sending motion data");

    char payload[128];
    snprintf(payload, sizeof(payload),
             R"({"motion":%u,"sensor":"ks0052","device":"%s","mac":"%s"})",
             motion, ssid, sMacAddr);

    sendMessage(payload);
}

void sendMessage(const String &outgoing) {
    LoRa.beginPacket(); // start packet
    LoRa.print(outgoing); // add payload
    LoRa.endPacket(); // finish packet and send it
}
