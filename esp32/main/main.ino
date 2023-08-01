#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WebSocketsServer.h>


const char* ssid = "Duke 2.4";
const char* password = "furfin30nature";

const int port = 1234;
const int camera_port = 1400;

struct header_s {
  uint8_t type;
};

struct direction_s : public header_s { // type 1
  uint8_t dir;
  uint8_t angle;
};

struct send_s : public header_s { // type 2, sending something
  char msg[32];
};

WiFiServer server(port);
// WiFiServer camera_feed(camera_port);
WebSocketsServer webSocket = WebSocketsServer(camera_port);

uint8_t zero = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_HD;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 20;
  config.fb_count = 1;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  webSocket.begin();
  // camera_feed.begin();

  Serial.println("Server started");

  // if (!client.connect(WiFi.localIP(), port)) {
  //   Serial.println("Connection failed.");
  //   return;
  // }

}

void loop() {
  char buffer[256];
  WiFiClient client = server.available();
  // WiFiClient camera_client = camera_feed.available();

  client.setTimeout(1000);
  int recv_bytes = 0;

  while (client) {
    recv_bytes = client.read((uint8_t*)buffer, sizeof(header_s));

    if (recv_bytes > 0) {
      switch (((header_s*)buffer)->type) {
        case 66: {
          recv_bytes = client.read((uint8_t*)(buffer + sizeof(header_s)), sizeof(direction_s) - sizeof(header_s));

          direction_s* dir = (direction_s*)buffer;

          Serial.print("Direction received: "); Serial.println(dir->dir);
          Serial.print("Angle received: "); Serial.println(dir->angle);
        }
          
          break;
        case 65: {
          send_s send_msg;
          send_msg.type = 2;
          send_msg.msg[0] = 'p'; send_msg.msg[1] = 'o'; send_msg.msg[2] = 'r'; send_msg.msg[3] = 'u'; send_msg.msg[4] = 'k'; send_msg.msg[5] = 'a';  send_msg.msg[6] = '\0';
          recv_bytes = client.write((const uint8_t*)&send_msg, sizeof(send_s));
          Serial.println("Message sent!");
        }
          
          break;
        default:
          Serial.print("Weird type received: "); Serial.println(((header_s*)buffer)->type);
          break;
      }

      memset(buffer, '\0', 256);
      
    }

    webSocket.loop();
    camera_fb_t *frame = esp_camera_fb_get();

    if (frame) {
      webSocket.broadcastBIN(frame->buf, frame->len);

      Serial.println("frame set!");

      esp_camera_fb_return(frame);
    } else {
      Serial.println("FAILED TO CATCH FRAME!");
    }

  }
}
