#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials

// Replace with your network credentials
const char* ssid = "SpectrumSetup-C70F";
const char* password = "honestresort661";


// LoRa Parameters
#define FREQUENCY 866.3
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9

// Store received LoRa data
String temperature = "0.0";
String humidity = "0.0";
String pressure = "0.0";

// Web server on port 80
WebServer server(80);

// HTML Template
const char* htmlTemplate = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Weather Station</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
    .data { margin: 20px; padding: 20px; border: 1px solid #ccc; display: inline-block; }
  </style>
</head>
<body>
  <h1>Weather Station</h1>
  <div class="data">
    <h2>Latest Weather Data:</h2>
    <p id="temperature">Temperature: -- °C</p>
    <p id="humidity">Humidity: -- %</p>
    <p id="pressure">Pressure: -- hPa</p>
  </div>
  <script>
    setInterval(() => {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temperature').textContent = `Temperature: ${data.temperature} °C`;
          document.getElementById('humidity').textContent = `Humidity: ${data.humidity} %`;
          document.getElementById('pressure').textContent = `Pressure: ${data.pressure} hPa`;
        })
        .catch(err => console.error('Error fetching data:', err));
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

void setup() {
  heltec_setup();
  both.println("LoRa Receiver Web Server");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    both.print(".");
  }
  both.println("\nWiFi Connected!");
  both.printf("Access the server at: http://%s/\n", WiFi.localIP().toString().c_str());

  // Initialize LoRa
  RADIOLIB_OR_HALT(radio.begin());
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));

  // Define server routes
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlTemplate); // Serve the HTML page
  });

  server.on("/data", HTTP_GET, []() {
    String jsonData = "{";
    jsonData += "\"temperature\":\"" + temperature + "\",";
    jsonData += "\"humidity\":\"" + humidity + "\",";
    jsonData += "\"pressure\":\"" + pressure + "\"";
    jsonData += "}";
    server.send(200, "application/json", jsonData); // Serve JSON data
  });

  // Start the server
  server.begin();
}

void loop() {
  // Check for LoRa packets
  if (radio.readData(temperature, humidity, pressure) == RADIOLIB_ERR_NONE) { 
    both.printf("Received - Temperature: %s °C, Humidity: %s %%, Pressure: %s hPa\n",
                temperature.c_str(), humidity.c_str(), pressure.c_str());
  }

  // Handle web server requests
  server.handleClient();
}
