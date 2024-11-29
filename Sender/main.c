
#include <heltec_unofficial.h>

#define FREQUENCY 866.3       // Frequency in MHz
#define BANDWIDTH 125.0       // Bandwidth in kHz
#define SPREADING_FACTOR 9    // Spreading Factor
#define TRANSMIT_POWER 14     // Transmit Power in dBm

void setup() {
  heltec_setup(); // Initialize Heltec device
  both.println("Transmitter ready");
  
  // Initialize LoRa module
  RADIOLIB_OR_HALT(radio.begin());
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  RADIOLIB_OR_HALT(radio.setOutputPower(TRANSMIT_POWER));
}

void loop() {
  // Generate random values for temperature, humidity, and pressure
  float temperature = random(15, 40) + random(0, 100) / 100.0; // Random temp between 15.00 and 40.99
  float humidity = random(20, 100) + random(0, 100) / 100.0;   // Random humidity between 20.00 and 99.99
  float pressure = random(900, 1100) + random(0, 100) / 100.0; // Random pressure between 900.00 and 1099.99

  // Format data as a JSON-like string
  String message = String("{\"temp\":") + temperature +
                   ",\"hum\":" + humidity +
                   ",\"pres\":" + pressure + "}";

  // Transmit the message
  both.printf("Sending: %s\n", message.c_str());
  RADIOLIB(radio.transmit(message.c_str()));

  delay(3000); // Wait 3 seconds before sending the next set of values
}
