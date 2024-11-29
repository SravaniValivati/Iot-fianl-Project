#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials

// Replace with your network credentials
const char* ssid = "SpectrumSetup-C70F";
const char* password = "honestresort661";


// Web server on port 80
WebServer server(80);


// HTML Template
const char* htmlTemplate = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>LoRa Data Viewer</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
    .data { margin: 20px; padding: 20px; border: 1px solid #ccc; display: inline-block; }
  </style>
</head>
<body>
  <h1>LoRa Receiver Data</h1>
  <div class="data">
    <h2>Latest Data:</h2>
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
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("Access your server at: http://");
  Serial.println(WiFi.localIP());

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
    server.send(200, "application/json", jsonData); // Send JSON data
  });

  // Start the server
  server.begin();
  Serial.println("Server started!");
}

void loop() {

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    lastUpdate = millis();
    temperature = String(random(20, 35) + random(0, 100) / 100.0); // Random temp between 20.00 and 34.99
    humidity = String(random(30, 80) + random(0, 100) / 100.0);    // Random humidity between 30.00 and 79.99
    pressure = String(random(950, 1050) + random(0, 100) / 100.0); // Random pressure between 950.00 and 1049.99
  }

  // Handle web server requests
  server.handleClient();
}
