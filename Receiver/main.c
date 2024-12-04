
#include <heltec_unofficial.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi Credentials
const char* ssid = "";
const char* password = "";

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
</head>
<body>
  <div id="content"></div>
  <script src="https://sravanivalivati.github.io/webserver/main.js"></script>
  <script>
    function loadExternalHTML(url, elementId) {
      fetch(url)
        .then(response => response.text())
        .then(data => {
          document.getElementById(elementId).innerHTML = data;
        })
        .catch(error => console.error('Error loading external HTML:', error));
    }

    // Load the GitHub HTML content
    loadExternalHTML('https://sravanivalivati.github.io/webserver/index.html', 'content');
    
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
  // Do not include delay to keep the web server responsive
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  both.printf("Serving receivedData: %s\n", receivedData.c_str());
  server.send(200, "application/json", receivedData);
}

