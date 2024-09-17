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
const int led_pin_2_red = 22;
const int led_pin_3_red = 1;
const int led_pin_4_red = 3;
const int led_pin_5_red = 21;
const int led_pin_6_red = 19;

const int led_pin_1_blue = 32;
const int led_pin_2_blue = 33;
const int led_pin_3_blue = 27;
const int led_pin_4_blue = 14;
const int led_pin_5_blue = 12;
const int led_pin_6_blue = 13;

int last_received_message = 0;
bool last_is_braking = false;
int led_state = LOW;

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
  pinMode(led_pin_2_red, OUTPUT);
  pinMode(led_pin_3_red, OUTPUT);
  pinMode(led_pin_4_red, OUTPUT);
  pinMode(led_pin_5_red, OUTPUT);
  pinMode(led_pin_6_red, OUTPUT);

  pinMode(led_pin_1_blue, OUTPUT);
  pinMode(led_pin_2_blue, OUTPUT);
  pinMode(led_pin_3_blue, OUTPUT);
  pinMode(led_pin_4_blue, OUTPUT);
  pinMode(led_pin_5_blue, OUTPUT);
  pinMode(led_pin_6_blue, OUTPUT);


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

  if (data.is_braking && time_since_last_message < 200) {
    led_state = HIGH;
  } else {
    led_state = LOW;
  }

  digitalWrite(led_pin_3_red, led_state);
  digitalWrite(led_pin_4_red, led_state);
  digitalWrite(led_pin_3_blue, !led_state);
  digitalWrite(led_pin_4_blue, !led_state);
  delay(100);
  digitalWrite(led_pin_2_red, led_state);
  digitalWrite(led_pin_5_red, led_state);
  digitalWrite(led_pin_2_blue, !led_state);
  digitalWrite(led_pin_5_blue, !led_state);
  delay(50);
  digitalWrite(led_pin_1_red, led_state);
  digitalWrite(led_pin_6_red, led_state);
  digitalWrite(led_pin_1_blue, !led_state);
  digitalWrite(led_pin_6_blue, !led_state);

}