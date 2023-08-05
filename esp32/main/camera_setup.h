#ifndef _CAMERA_SETUP_H_
#define _CAMERA_SETUP_H_

#include "esp_http_server.h"

#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#define PART_BOUNDARY "123456789000000000000987654321"

esp_err_t stream_handler(httpd_req_t *req);

void startCameraServer();

void initCamera();

#endif