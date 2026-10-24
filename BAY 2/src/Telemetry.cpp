#include <ArduinoJson.h>
#include "telemetry.h"
#include "network.h"
#include "state.h"
#include "config.h"

static void printTelemetryReadout()
{
  char line[40];

  Serial.println("--- Telemetry (" + String(BAY_ID) + ") -----------------");

  snprintf(line, sizeof(line), "Voltage     : %.1f V", voltage);
  Serial.println(line);
  snprintf(line, sizeof(line), "Current     : %.1f A", current);
  Serial.println(line);
  snprintf(line, sizeof(line), "Power       : %.1f W", power);
  Serial.println(line);
  snprintf(line, sizeof(line), "Temperature : %.1f C", temperature);
  Serial.println(line);
  Serial.println("Status      : " + String(bayStatus));
  snprintf(line, sizeof(line), "PredArrProb : %.2f", predictedArrivalProb);
  Serial.println(line);
  snprintf(line, sizeof(line), "PredDurMin  : %d min", predictedDurationMin);
  Serial.println(line);
  Serial.println("-------------------------------------");
}

void publishTelemetry()
{
  //local readout every loop tick (works even before cloud connect)
  printTelemetryReadout();

  //check if the device is connected to cloud or not
  if (!mqtt.connected()) {
    Serial.println("Telemetry publish skipped: MQTT disconnected.");
    return;
  }

  //store the values as key value pair formate
  JsonDocument doc;
  doc["bayId"] = BAY_ID;
  doc["voltage"] = round(voltage * 10) / 10.0;
  doc["current"] = round(current * 10) / 10.0;
  doc["power"] = round(power * 10) / 10.0;
  doc["temperature"] = round(temperature * 10) / 10.0;
  doc["bayStatus"] = bayStatus;
  //adding 2 more values to the buffer 
  doc["predictedArrivalProb"] = round(predictedArrivalProb *100)/100.0;
  doc["predictedDurationMin"] = predictedDurationMin;

  //add load decission level, overload active or not
  doc["throttleLevel"] = throttleLevel;
  doc["loadDecision"] = loadDecision;
  doc[" overloadCurrentA "] =  overloadCurrentA ;
  char buffer[350];
  serializeJson(doc, buffer);

  //push the data to the cloud , topic , ->buffer
  if (!mqtt.publish("v1/devices/me/telemetry", buffer)) {
    Serial.println("Telemetry publish failed.");
  }
}
