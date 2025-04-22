//Citation: https://www.kincony.com/forum/showthread.php?tid=6898
//Citation: https://stackoverflow.com/questions/76415696/how-to-generating-sine-wave-in-max98357a-with-esp32
#include "Arduino.h"
#include "driver/i2s.h" //I2S communication protocol with speaker
// #include "Audio.h"

#define I2S_DOUT      47   // Data out pin
#define I2S_BCLK      38   // Bit clock pin
#define I2S_LRC       48   // Left-right clock pin


// #define I2S_DOUT      42   // Data out pin
// #define I2S_BCLK      41   // Bit clock pin
// #define I2S_LRC       40   // Left-right clock pin


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


// Generate a sine wave and play
void genSineWave() {
    const int amplitude = 28000; // amplitude of sine wave
    const int freq = 440;        // frequency of note (A4) in Hz
    int16_t tempBuff[128];
 
    for (int i = 0; i < 128; i++) { // plays a sine wave - sounds like a buzzer
      float angle = (2.0 * PI * freq * i) / SAMPLE_RATE;
      tempBuff[i] = (int16_t)(amplitude * sin(angle));
    }

    size_t bytes_written;
    //plays the sound for 100 bytes
    while (true) {
      Serial.println("Speaker playing...");
      for(int i = 0; i < 100; i++){
        i2s_write(I2S_NUM_0, tempBuff, sizeof(tempBuff), &bytes_written, portMAX_DELAY);
        delay(20);
      }

      // introducing intermittent 1 second delay
      Serial.println("Sound off...");
      i2s_driver_uninstall(I2S_NUM_0); // turns the I2S communication off, preventing sound from playing
      delay(1000);
      setupI2S(); // turns I2S communication back on
  
    }
}

void setup() {
    Serial.begin(115200);
    setupI2S();
    // setVolume(25000);  // 70 - 72 dBA
}

void loop() {
  genSineWave(); // Start playing
}
