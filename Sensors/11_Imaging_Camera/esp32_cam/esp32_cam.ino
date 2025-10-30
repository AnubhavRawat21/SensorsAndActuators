#include "esp_camera.h"      // ESP32 camera library
#include <WiFi.h>             // WiFi connectivity
#include "esp_http_server.h"  // HTTP server for streaming

// WiFi credentials - replace with your network details
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Camera GPIO pin definitions for AI-Thinker ESP32-CAM board
#define PWDN_GPIO_NUM     32  // Power down pin
#define RESET_GPIO_NUM    -1  // Reset pin (not used)
#define XCLK_GPIO_NUM      0  // External clock
#define SIOD_GPIO_NUM     26  // I2C data line
#define SIOC_GPIO_NUM     27  // I2C clock line
#define Y9_GPIO_NUM       35  // Data bit 9
#define Y8_GPIO_NUM       34  // Data bit 8
#define Y7_GPIO_NUM       39  // Data bit 7
#define Y6_GPIO_NUM       36  // Data bit 6
#define Y5_GPIO_NUM       21  // Data bit 5
#define Y4_GPIO_NUM       19  // Data bit 4
#define Y3_GPIO_NUM       18  // Data bit 3
#define Y2_GPIO_NUM        5  // Data bit 2
#define VSYNC_GPIO_NUM    25  // Vertical sync
#define HREF_GPIO_NUM     23  // Horizontal reference
#define PCLK_GPIO_NUM     22  // Pixel clock

// HTTP server handle for streaming
httpd_handle_t stream_httpd = NULL;

// Handler function for MJPEG video streaming
static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;           // Frame buffer pointer
  esp_err_t res = ESP_OK;            // Response status
  size_t _jpg_buf_len = 0;           // JPEG buffer length
  uint8_t * _jpg_buf = NULL;         // JPEG buffer pointer
  char * part_buf[64];               // HTTP multipart boundary buffer

  // Set response type to MJPEG stream
  res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
  if(res != ESP_OK) return res;

  // Continuous streaming loop
  while(true) {
    // Capture a frame from camera
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      // Check if frame needs JPEG conversion
      if(fb->format != PIXFORMAT_JPEG) {
        // Convert frame to JPEG format
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);  // Return frame buffer
        fb = NULL;
        if(!jpeg_converted) {
          Serial.println("JPEG compression failed");
          res = ESP_FAIL;
        }
      } else {
        // Frame is already in JPEG format
        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;
      }
    }
    
    // Send HTTP headers with image size
    if(res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    
    // Send JPEG image data
    if(res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    
    // Send multipart boundary for next frame
    if(res == ESP_OK) {
      res = httpd_resp_send_chunk(req, "\r\n--frame\r\n", 12);
    }
    
    // Clean up frame buffer and JPEG buffer
    if(fb) {
      esp_camera_fb_return(fb);  // Return frame buffer to camera
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf) {
      free(_jpg_buf);  // Free converted JPEG buffer
      _jpg_buf = NULL;
    }
    
    // Exit loop if there's an error
    if(res != ESP_OK) break;
  }
  return res;
}

// Function to start the camera web server
void startCameraServer() {
  // Configure HTTP server with default settings
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 81;  // Server runs on port 81

  // Define URI handler for /stream endpoint
  httpd_uri_t stream_uri = {
    .uri       = "/stream",      // URL path
    .method    = HTTP_GET,       // HTTP method
    .handler   = stream_handler, // Handler function
    .user_ctx  = NULL            // User context
  };

  // Start HTTP server and register stream handler
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}

void setup() {
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  
  // Configure camera settings
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;  // LED PWM channel
  config.ledc_timer = LEDC_TIMER_0;      // LED PWM timer
  config.pin_d0 = Y2_GPIO_NUM;           // Assign data pins
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;       // External clock pin
  config.pin_pclk = PCLK_GPIO_NUM;       // Pixel clock pin
  config.pin_vsync = VSYNC_GPIO_NUM;     // Vertical sync pin
  config.pin_href = HREF_GPIO_NUM;       // Horizontal reference pin
  config.pin_sscb_sda = SIOD_GPIO_NUM;   // I2C data pin
  config.pin_sscb_scl = SIOC_GPIO_NUM;   // I2C clock pin
  config.pin_pwdn = PWDN_GPIO_NUM;       // Power down pin
  config.pin_reset = RESET_GPIO_NUM;     // Reset pin
  config.xclk_freq_hz = 20000000;        // Clock frequency 20MHz
  config.pixel_format = PIXFORMAT_JPEG;  // Output format JPEG
  config.frame_size = FRAMESIZE_HVGA;    // Resolution 480x320
  config.jpeg_quality = 12;              // JPEG quality (10-63, lower = better)
  config.fb_count = 2;                   // Number of frame buffers

  // Initialize camera with configuration
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    return;  // Stop if camera initialization fails
  }

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Print dots while connecting
  }
  
  // Print connection success and streaming URL
  Serial.println("\nWiFi connected");
  Serial.print("Stream: http://");
  Serial.print(WiFi.localIP());  // Display IP address
  Serial.println(":81/stream");  // Display stream endpoint
  
  // Start the camera streaming server
  startCameraServer();
}

void loop() {
  // Main loop - keeps program running
  delay(100);  // Small delay to prevent watchdog timer issues
}