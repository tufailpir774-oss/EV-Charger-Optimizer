#include <WiFi.h>
#include "network.h"
#include "config.h"
#include "rpc.h"


WiFiClient espClient;
PubSubClient mqtt(espClient);

static const char RPC_REQUEST_TOPIC[] = "v1/devices/me/rpc/request/+";
static const char RPC_REQUEST_PREFIX[] = "v1/devices/me/rpc/request/";
static unsigned long lastMqttAttemptMs = 0;
static const unsigned long MQTT_RECONNECT_INTERVAL_MS = 5000;

static void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  String topicString(topic);
  if (!topicString.startsWith(RPC_REQUEST_PREFIX)) return;

  String requestId = topicString.substring(sizeof(RPC_REQUEST_PREFIX) - 1);
  if (requestId.length() == 0) return;

  String payloadString(reinterpret_cast<const char*>(payload), length);
  Serial.print("<< MQTT RPC request ");
  Serial.print(requestId);
  Serial.print(" on ");
  Serial.println(topic);
  handleRpc(requestId, payloadString.c_str());
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? " connected." : " FAILED (will retry).");
}

void connectMQTT() {
  unsigned long now = millis();
  if (now - lastMqttAttemptMs < MQTT_RECONNECT_INTERVAL_MS) return;
  lastMqttAttemptMs = now;

  mqtt.setBufferSize(1024);
  mqtt.setCallback(mqttCallback);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("MQTT connection skipped: WiFi disconnected.");
    return;
  }

  Serial.print("Connecting to ThingsBoard MQTT...");
  if (mqtt.connect(BAY_ID, TB_TOKEN, NULL)) {
    Serial.println(" connected.");
    if (mqtt.subscribe(RPC_REQUEST_TOPIC)) {
      Serial.print("RPC subscription success: ");
      Serial.println(RPC_REQUEST_TOPIC);
    } else {
      Serial.println("RPC subscription failed.");
    }
  } else {
    Serial.print(" failed, rc=");
    Serial.println(mqtt.state());
  }
}

