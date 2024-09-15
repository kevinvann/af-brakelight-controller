#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xFC, 0xE8, 0xC0, 0x7B, 0x51, 0x79};

const int led_pin_1_red = 23;
const int led_pin_1_blue = 22;

int led_state = HIGH;

void receive_message(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,
                     void* arg) {
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],
                mac[4], mac[5]);
  for (int i = 0; i < static_cast<int>(count); ++i) {
    Serial.print(static_cast<char>(buf[i]));
  }
  Serial.println();

  led_state = !led_state;

}

void
setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(led_pin_1_red, OUTPUT);
  pinMode(led_pin_1_blue, OUTPUT);

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(receive_message, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
}

void loop() {

  digitalWrite(led_pin_1_blue, led_state);
  digitalWrite(led_pin_1_red, !led_state);

}