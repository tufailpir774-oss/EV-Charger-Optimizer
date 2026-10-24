# Edge AI Based Smart EV Charging Station Optimizer

Embedded firmware for an ESP32-based smart EV charging station bay optimizer.
It reads bay sensors (voltage, current, temperature/humidity, plug status),
runs an on-device (edge) optimization routine, and publishes telemetry to
ThingsBoard over MQTT.

## Implemented Features

- Sensor acquisition: voltage (ADC), current (ADC), DHT22 temperature/humidity,
  plug-in/plug-out buttons.
- Actuation: relay control, status LEDs (green/yellow/red).
- State machine managing bay states (include/State.h, src/State.cpp).
- Edge AI / optimization module (include/edge_ai.h, src/edge_ai.cpp,
  include/optimization.h, src/optimization.cpp, src/model.h).
- Network & MQTT connectivity (src/Network.cpp).
- Telemetry publishing (src/Telemetry.cpp).
- RPC handling (src/rpc.cpp).
- Wokwi simulation support (	est/diagram.json, 	est/wokwi.toml).

## Hardware

- ESP32 DOIT DevKit V1
- DHT22 (pin 15)
- Voltage sensor on GPIO 34, current sensor on GPIO 35
- Relay on GPIO 26, buttons on GPIO 32/33
- LEDs on GPIO 18/19/21

## Software Stack

- PlatformIO, Arduino framework for ESP32
- Libraries: PubSubClient, DHT sensor library, Adafruit Unified Sensor, ArduinoJson

## Getting Started

1. Install PlatformIO (VS Code extension or CLI).
2. Clone this repository.
3. Copy examples/config.cpp to src/config.cpp and fill in your values.
4. Build and upload:
   `
   pio run -t upload
   `

## Configuration

Wi-Fi and ThingsBoard credentials are defined in src/config.cpp (see
include/config.h). Do **not** commit real credentials. Use the provided
examples/config.cpp template and keep src/config.cpp local.

## Simulation

Wokwi project files are under 	est/. Open 	est/diagram.json in Wokwi
after building the firmware (.pio/build/esp32doit-devkit-v1/).

## Security

- src/config.cpp is gitignored. Rotate any token previously committed.
- Never publish device access tokens or Wi-Fi passwords.

## Project Structure

`
include/   headers
src/       firmware source
lib/       private libraries (empty)
test/      Wokwi simulation files
`

## License

Not specified. Add a license before publishing.
