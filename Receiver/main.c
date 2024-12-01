#include <heltec_unofficial.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials
const char* ssid = "TwoOSeven1E";
const char* password = "nenuchepanuponew@207";

// Store received data
String receivedData = "{\"temp\":0,\"hum\":0,\"pres\":0}";

// Web server
WebServer server(80);

// HTML page to serve with styling and formatted data display
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>LoRa Receiver Data</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background-color: #f0f2f5;
      margin: 0;
      padding: 0;
    }
    .header {
      background-color: #0078d7;
      color: white;
      padding: 20px;
      text-align: center;
    }
    .container {
      padding: 20px;
    }
    .data-box {
      background-color: white;
      padding: 20px;
      margin: 0 auto;
      max-width: 600px;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0, 120, 215, 0.3);
    }
    .data-title {
      font-size: 1.5em;
      margin-bottom: 10px;
      color: #0078d7;
    }
    .data-content {
      font-size: 1.2em;
      line-height: 1.6;
    }
    .data-content span {
      font-weight: bold;
    }
    @media (max-width: 600px) {
      .data-box {
        padding: 15px;
      }
      .data-title {
        font-size: 1.3em;
      }
      .data-content {
        font-size: 1em;
      }
    }
  </style>
</head>
<body>
  <div class="header">
    <h1>LoRa Receiver Data</h1>
  </div>
  <div class="container">
    <div class="data-box">
      <div class="data-title">Latest Data:</div>
      <div class="data-content">
        <p>Temperature: <span id="temp">--</span> C</p>
        <p>Humidity: <span id="hum">--</span> %</p>
        <p>Pressure: <span id="pres">--</span> hPa</p>
      </div>
    </div>
  </div>
  <script>
    function fetchData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temp').innerText = data.temp;
          document.getElementById('hum').innerText = data.hum;
          document.getElementById('pres').innerText = data.pres;
        })
        .catch(error => console.error('Error fetching data:', error));
    }
    setInterval(fetchData, 3000);
    fetchData();
  </script>
</body>
</html>
)rawliteral";

// Function prototypes
void handleRoot();
void handleData();

#define FREQUENCY 866.3
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9

void setup() {
  heltec_setup();
  both.println("LoRa Receiver Test");

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  both.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    both.print(".");
  }
  both.println("\nWi-Fi Connected!");
  String ip = WiFi.localIP().toString();
  both.printf("Access the web server at: http://%s/\n", ip.c_str());

  // Initialize Radio
  RADIOLIB_OR_HALT(radio.begin());
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  both.println("Web server started.");
}

void loop() {
  server.handleClient(); // Handle web server requests

  String receivedMessage;
  int state = radio.readData(receivedMessage);
  if (state == RADIOLIB_ERR_NONE) {
    both.printf("Raw Data Received: %s\n", receivedMessage.c_str());
    receivedData = receivedMessage; // Update the receivedData variable
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // No data received, do nothing
  } else {
    both.printf("Error reading data: %d\n", state);
  }
  // keeping the web server responsive
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  both.printf("Serving receivedData: %s\n", receivedData.c_str());
  server.send(200, "application/json", receivedData);
}
