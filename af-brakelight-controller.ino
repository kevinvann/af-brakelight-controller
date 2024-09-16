#include <WifiEspNow.h>
#include <WiFi.h>

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{ 0xFC, 0xE8, 0xC0, 0x7C, 0x4C, 0xB1 };

typedef struct struct_message {
  bool is_braking = false;
} struct_message;

// Create a struct_message called myData
struct_message data;

const int led_pin_1_red = 23;
const int led_pin_1_blue = 22;

int last_received_message = 0;

void receive_message(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,
                     void* arg) {
  // Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],
  //               mac[4], mac[5]);
  memcpy(&data, buf, sizeof(data));
  last_received_message = millis();
  // Serial.print("is_braking: ");
  // Serial.println(data.is_braking);
  // Serial.println();
}

void setup() {
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
  int time_since_last_message = millis() - last_received_message;

  // if esp transmitter is off and not sending messages, default to regular lights
  if (data.is_braking && time_since_last_message < 500) {
    digitalWrite(led_pin_1_blue, LOW);
    digitalWrite(led_pin_1_red, HIGH);
  } else {
    digitalWrite(led_pin_1_blue, HIGH);
    digitalWrite(led_pin_1_red, LOW);
  }
}