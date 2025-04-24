// Citation: https://www.instructables.com/Creating-a-Wireless-Network-With-ESP32-AP-Mode/
// Go to File > Examples > ESP 32 > camaera > CameraWebServer

// Speaker Citation
//Citation: https://www.kincony.com/forum/showthread.php?tid=6898
//Citation: https://stackoverflow.com/questions/76415696/how-to-generating-sine-wave-in-max98357a-with-esp32
#include "esp_camera.h"
#include <WiFi.h>
#include "Arduino.h"
#include "driver/i2s.h" //I2S communication protocol with speaker
// #include "Audio.h"

// Speaker PCB 2 Pins
#define I2S_DOUT      47   // Data out pin
#define I2S_BCLK      38   // Bit clock pin
#define I2S_LRC       48   // Left-right clock pin

//-------------------------------------------------------------Temperature Sensor Constants----------------------------------------------
const int resistorValue = 10000; // 10k resistor used with the thermistor
const int nomTemp = 25; // temperature that the thermistor was tested under
const int nomRes = 10000; // resistance of the thermistor standard temp
const float sthh_const = 3950.0; // constant used in steinhart-hart equation
const float kel_const = 273.15; // kelvin constant
const int thresholdTemp = 85; // threshold temperature
const int tempPin = 2; // ESP32 Temperature Sensor Analog Pin 
int Fah = 0;
//-------------------------------------------------------------Speaker  Setup----------------------------------------------
#define SAMPLE_RATE   44100  // Audio sample rate (Hz)
// Audio audio;

// I2S Configuration
void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), 
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, 
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void genSineWave() {
    const int amplitude = 300000; // amplitude of sine wave
    const int freq = 440;        // frequency of note (A4) in Hz
    int16_t tempBuff[128];
 
    for (int i = 0; i < 128; i++) { // plays a sine wave - sounds like a buzzer
      float angle = (2.0 * PI * freq * i) / SAMPLE_RATE;
      tempBuff[i] = (int16_t)(amplitude * sin(angle));
    }

    size_t bytes_written;
    //plays the sound for 100 bytes
    while (true) {
      // Serial.println("Speaker playing...");
      for(int i = 0; i < 100; i++){
        i2s_write(I2S_NUM_0, tempBuff, sizeof(tempBuff), &bytes_written, portMAX_DELAY);
        delay(20);
      }

      // introducing intermittent 1 second delay
      // Serial.println("Sound off...");
      i2s_driver_uninstall(I2S_NUM_0); // turns the I2S communication off, preventing sound from playing
      delay(1000);
      setupI2S(); // turns I2S communication back on
      if(Fah < thresholdTemp){
        break;
      }
    }
}



// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE  // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
#define CAMERA_MODEL_ESP32_CAM_BOARD
// #define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// WiFi credentials
const char* ssid = "direcTori";
const char* password = "123456789";
// const int max_connection = 3;
// const int channel = 4;
// const int ssid_hidden = 0;
WiFiServer server(80);

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  setupI2S();
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 26000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

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

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

// #if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
//   s->set_vflip(s, 1);
//   s->set_hmirror(s, 1);
// #endif

// #if defined(CAMERA_MODEL_ESP32S3_EYE)
//   s->set_vflip(s, 1);
// #endif

// setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  // WiFi.begin(ssid, password);
  // WiFi.setSleep(false);

  // Serial.print("WiFi connecting");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("Before Tori is on");

  // uncomment the below code for ESP32 as its own router
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP(); // esp32 supports its own wifi
  Serial.print("Software AP IP address: ");
  Serial.println(IP);
  // WiFi.enableLongRange(true);
  startCameraServer();

  Serial.print("The Camera web server is ready! Go to 'http://");
  Serial.print(IP);
  // Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(1000);
  int sensorValue = analogRead(tempPin); // stores the raw resistance value of the thermistor
  // Serial.println("RSSI: ");
  // Serial.println(WiFi.RSSI());
   // Calculation: The thermistor and 10k ohm form a voltage divider circuit. 
  // The first part of the equation is finding the volatge across the thermistor. Vin * (thermistor res/(thermistor res + 10k)).
  // The 4095 is to convert to 12 bits. This is to return the voltage value
  float volt = (sensorValue * 3.3)/ 4095; // converts the resistance to a voltage value
  float resistance = (volt * resistorValue)/ (3.3 - volt); //this finds the updated resistance value using the above voltage 
  // converts resistance to temperature using the Steinhart-Hart equation
  float kelvin = 1 / (((log(resistance / nomRes)) / sthh_const) + (1 / (nomTemp + kel_const)));
  float Cels = kelvin - kel_const; // convert Kelvin to Celsius 
  float Fah = 1.8 * Cels + 32.0; // convert temp to Fahrenheit

  Serial.print("Temperature: ");
  // Serial.print(kelvin);
  // Serial.print(" K | ");
  Serial.print(Cels);
  Serial.print(" °C | ");
  Serial.print(Fah);
  Serial.println(" °F");

  if(Fah >= thresholdTemp ){
    // digitalWrite(7,HIGH); 
    // Serial.println("THE CAR IS TOOOOOO HOT ");
    Serial.println("The car is over 85. Lower windows!");
    // setupI2S();
    genSineWave(); // Start playing
    // windowLowering = 1; // lower the window
  }
  else{
    // digitalWrite(7,LOW); 
    Serial.println("The car is under 85");
    // i2s_driver_uninstall(I2S_NUM_0);
    // windowLowering = 0; // dont lower the window 
  }
  // genSineWave(); // Start playing
 

  // delay(10000);
}
