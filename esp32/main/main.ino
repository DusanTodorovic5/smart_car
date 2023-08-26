
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <BluetoothSerial.h>
#include "json_messages.h"
#include <WebSocketsServer.h>
#include "fb_gfx.h"
#include "esp_timer.h"
#include "camera_setup.h"
#include "states.h"

State state;
uint8_t delay_counter = 0;

String ssid = "9604e6";
String password = "280841632";

const int port = 1234;
WebSocketsServer server = WebSocketsServer(port);
BluetoothSerial SerialBT;
uint8_t gnum = -1;
char msgBuf[100];

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            // Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
              gnum = num;
                IPAddress ip = server.remoteIP(num);
                // Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

              // send message to client
              server.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            // Serial.printf("[%u] get Text: %s\n", num, payload);

            switch (new_message((char*)payload)) {
              case 1: 
                // Serial.printf("Recv [%u] with voltage [%u]\n", voltage_message.message_type, voltage_message.voltage);
                Serial.write((uint8_t *) &voltage_message, sizeof(voltage_message));
                // sprintf(msgBuf, "Recv [%u] with voltage [%u]\n", voltage_message.message_type, voltage_message.voltage);
                // server.sendTXT(num, msgBuf);
                break;
              case 2: 
                // sprintf(msgBuf, "Recv [%u] with direction [%u]\n", direction_message.message_type, direction_message.direction);
                Serial.write((uint8_t *) &direction_message, sizeof(direction_message));
                // server.sendTXT(num, msgBuf);
                break;
              case 3: 
                // Serial.printf("Recv [%u] with values [%u],[%u],[%u],[%u]\n", 
                //               led_message.message_type, 
                //               led_message.left_dir_light,
                //               led_message.right_dir_light,
                //               led_message.front_light,
                //               led_message.auto_lights
                //             );
                Serial.write((uint8_t *) &led_message, sizeof(led_message));
                break;
            }
            
            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
    case WStype_BIN:
		case WStype_ERROR:			
		case WStype_FRAGMENT_TEXT_START:
		case WStype_FRAGMENT_BIN_START:
		case WStype_FRAGMENT:
		case WStype_FRAGMENT_FIN:
			break;
    }

}

void setup() {
  Serial.begin(115200);
  // SerialBT.begin("ESP32");
  delay(1000);
  state = READY;
}

void loop() {
  if (SerialBT.available()) {
    String recvMessage = "";
    char incomingChar;

    while ((incomingChar = SerialBT.read()) != '\n') {
      recvMessage += incomingChar;
    }

    // Serial.printf("Received: %s\n", recvMessage.c_str());

    if (bt_message((char*)recvMessage.c_str())) {
      // Serial.printf("\tSSID: %s\n\tPASSWORD: %s\n", password_bt.c_str(), ssid_bt.c_str());

      ssid = ssid_bt;
      password = password_bt;

      WiFi.disconnect();
      
      state = READY;
    }
  }
  std::string sss = "";
  while (Serial.available() && sss.length() < 40) {
    sss += (char)Serial.read();
  }
  if (gnum != -1 && sss != "") {
    server.sendTXT(gnum, sss.c_str());
  }


  switch (state) {
    case READY:
      // Serial.println();
      // Serial.print("Connecting to ");
      // Serial.println(ssid.c_str());

      WiFi.begin(ssid.c_str(), password.c_str());

      delay_counter = 0;
      while (WiFi.status() != WL_CONNECTED && delay_counter < 10) {
        delay(1000);
        // Serial.print(".");
        delay_counter++;
      }

      // Serial.println("WiFi connected");
      // Serial.println("IP address: ");
      // Serial.println(WiFi.localIP());

      SerialBT.printf("WiFi connected\nIP address: %s\n", WiFi.localIP());

      server.begin();

      initCamera();

      // Serial.println("Server started");
      server.onEvent(onWebSocketEvent);
    
      state = WIFI_CONNECTED;
      break;
    case WIFI_CONNECTED:
      server.loop();
      break;
    case BLUETOOTH_CONNECTED:
      WiFi.disconnect();
      break;
  } 
}
