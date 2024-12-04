#include <heltec_unofficial.h>
#include <DHT.h>

// LoRa Parameters
#define FREQUENCY 866.3
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9

// DHT11 Sensor Configuration
#define DHTPIN 33       // GPIO pin connected to the DHT22 data pin
#define DHTTYPE DHT22   // DHT22 sensor type

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  heltec_setup();
  both.println("LoRa Sender Ready");

  // Initialize DHT11 sensor
  dht.begin();

  // Initialize LoRa
  RADIOLIB_OR_HALT(radio.begin());
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  RADIOLIB_OR_HALT(radio.setOutputPower(20)); // Max power for range testing

  // Display initial message
  display.clear();
  display.drawString(0, 0, "LoRa Sender Ready");
  display.display();
}

void loop() {
  // Wait at least 2 seconds between DHT22 readings
  delay(2000);

  // Read temperature and humidity from DHT22
  float temperature = dht.readTemperature();  // Temperature in Celsius
  float humidity = dht.readHumidity();        // Humidity in %

  // Check if any reads failed and exit early (to try again)
  if (isnan(temperature) || isnan(humidity)) {
    both.println("Failed to read from DHT sensor!");
    display.clear();
    display.drawString(0, 0, "Sensor Error!");
    display.display();
    return;
  }

  // Create JSON message without pressure
  String message = "{\"temp\":" + String(temperature) +
                   ",\"hum\":" + String(humidity) + "}";

  // Send the message over LoRa
  both.printf("Sending: %s\n", message.c_str());
  RADIOLIB(radio.transmit(message.c_str()));

  // Display sent data on OLED
  display.clear();
  display.drawString(0, 0, "Sending Data...");
  display.drawString(0, 15, "Temp: " + String(temperature) + " C");
  display.drawString(0, 30, "Hum: " + String(humidity) + " %");
  display.display();

  // Wait 3 seconds before sending the next message
  delay(3000);
}