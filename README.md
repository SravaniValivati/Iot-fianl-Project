# Weather Monitoring with LoRa32 and Web Server

# LoRa Sender and Receiver System

This project demonstrates a LoRa-based communication system where a sender node reads sensor data and transmits it to a receiver node. The receiver processes the data and hosts a web server to display the received information.

---

## Prerequisites

### Hardware Requirements
- Heltec or compatible LoRa boards for both sender and receiver.
- DHT22 temperature and humidity sensor for the sender.

### Software Requirements
- **Platform**: Arduino IDE (recommended)  
  Download from [Arduino's official website](https://www.arduino.cc/en/software).
- Required Libraries (install via Arduino Library Manager):
  - `heltec_unofficial`
  - `DHT`
  - `RadioLib`
  - `WiFi`
  - `WebServer`

---

## Setup Instructions

### Sender Node

1. **Hardware Setup**:
   - Connect the DHT22 sensor's data pin to GPIO 33 on the Heltec board.
   - Ensure the LoRa module is properly integrated with the board.

2. **Code Setup**:
   - Open the sender code (`sender.ino`) in the Arduino IDE.
   - Install the required libraries via `Sketch > Include Library > Manage Libraries`.

3. **Compile and Flash**:
   - Connect the sender board to your computer via USB.
   - Select the correct board and port in Arduino IDE:
     - Go to `Tools > Board > Heltec ESP32` (or your specific board model).
     - Go to `Tools > Port > [Your Board Port]`.
   - Click the **Upload** button to compile and flash the code to the board.

4. **Run Sender Node**:
   - After uploading, open the Serial Monitor via `Tools > Serial Monitor`.
   - Confirm that the sender displays "LoRa Sender Ready" and starts transmitting data.

---

### Receiver Node

1. **Wi-Fi Configuration**:
   - Edit the receiver code (`receiver.ino`) to include your Wi-Fi credentials:
     ```cpp
     const char* ssid = "YourWiFiSSID";
     const char* password = "YourWiFiPassword";
     ```

2. **Code Setup**:
   - Open the receiver code in the Arduino IDE.
   - Install the required libraries using `Sketch > Include Library > Manage Libraries`.

3. **Compile and Flash**:
   - Connect the receiver board to your computer via USB.
   - Select the correct board and port in Arduino IDE.
   - Click the **Upload** button to compile and flash the code to the board.

4. **Run Receiver Node**:
   - Open the Serial Monitor via `Tools > Serial Monitor`.
   - Confirm that the receiver connects to Wi-Fi and displays the IP address for the web server.

5. **Access Web Server**:
   - Open a web browser and enter the IP address displayed in the Serial Monitor.
   - View the live-updating data received from the sender.

---

## Running the Code

1. Power on both the sender and receiver nodes.
2. **Sender**:
   - Verify that it reads data from the DHT22 sensor and transmits it via LoRa.
3. **Receiver**:
   - Confirm that the LoRa module is receiving data.
   - Access the web server using the provided IP address to monitor the data in real-time.

---

## Troubleshooting

1. **Sender Not Transmitting**:
   - Check the DHT22 sensor connections.
   - Verify that the LoRa parameters are correctly configured.

2. **Receiver Not Receiving**:
   - Ensure the sender and receiver use the same frequency and LoRa parameters.
   - Verify that the receiver is in range of the sender.

3. **Web Server Not Loading**:
   - Confirm that the receiver is connected to Wi-Fi.
   - Double-check the IP address displayed in the Serial Monitor.

---

## Stopping the System

1. Disconnect power to the sender and receiver boards.
2. To reinitialize, follow the setup instructions above.

---
## Contribution of Team Members
  # Sai Chandra
    - Set up the LoRa sender with necessary libraries.
    - Configured the sender for seamless communication.
    - Conducted initial testing to ensure proper data transmission.
  # Deepak
    - Configured the receiver to accept messages from the transmitter.
    - Initialized the DHT22 sensor to capture temperature, humidity, and pressure values.
    - Verified successful data capture by the receiver from the LoRa sender.
  # Sravani
    - Set up the ESP32 web server to display data.
    - Handled real-time data updates through API integration.
    - Configured Wi-Fi for stable communication.
    - Developed the HTML/CSS interface with real-time JavaScript updates.
## Notes

- Ensure both nodes are configured to use the same LoRa parameters (frequency, bandwidth, spreading factor).
- Use Arduino IDE for compiling and flashing the code for both sender and receiver nodes.

---
