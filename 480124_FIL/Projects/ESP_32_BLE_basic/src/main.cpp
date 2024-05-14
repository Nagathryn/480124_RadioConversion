

#include <Arduino.h>
#include "BluetoothA2DPSink.h"

// Digital I/O used
#define SD_CS         15
#define SPI_MOSI      4    // SD Card
#define SPI_MISO      16
#define SPI_SCK       2

#define I2S_DOUT      25
#define I2S_BCLK      27    // I2S
#define I2S_LRC       26

//#define ADC_SOUND     35
#define B_PLAY        4
#define B_NEXT        2
#define B_PREVIOUSE   16

#define ENCODER_DT     35
#define ENCODER_CLK    34

int iLevelSound;
int iStatePLAY = 0;
int iLastStatePLAY;

int etatPrecedentLigneCLK;  

int lastButtonState;

BluetoothA2DPSink a2dp_sink;

void setup() {

    Serial.begin(115200);
    pinMode(B_PLAY, INPUT);
    pinMode(B_NEXT, INPUT);
    pinMode(B_PREVIOUSE, INPUT);

    pinMode(ENCODER_DT, INPUT);
    pinMode(ENCODER_CLK, INPUT);

  i2s_pin_config_t my_pin_config = {
    .bck_io_num = 27,
    .ws_io_num = 26,
    .data_out_num = 25,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
    static const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100, // corrected by info from bluetooth
        .bits_per_sample = (i2s_bits_per_sample_t) 32, 
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };

  a2dp_sink.set_pin_config(my_pin_config);
  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.start("LaFabrique");

  delay(200);
}

void loop()
{
  int read;
  read = digitalRead(B_PLAY);

  int readNEXT;
  readNEXT = digitalRead(B_NEXT);

  int readPERVIOUS;
  readPERVIOUS = digitalRead(B_PREVIOUSE);

  int etatActuelDeLaLigneCLK = digitalRead(ENCODER_CLK);
  int etatActuelDeLaLigneDT  = digitalRead(ENCODER_DT);

  /*iLevelSound = analogRead(ADC_SOUND);
  int iMapSound;
  iMapSound = map(iLevelSound,0,4095,0,18);
  a2dp_sink.set_volume(iMapSound);*/

    if(read != iLastStatePLAY)
    {
      iLastStatePLAY = read;

      if(read == HIGH)
      {
        iStatePLAY = (iStatePLAY == 0) ? 1 : 0;
        if(iStatePLAY)a2dp_sink.pause();
        else a2dp_sink.play();
      }

  }

    if(etatActuelDeLaLigneCLK != etatPrecedentLigneCLK) {

      etatPrecedentLigneCLK = etatActuelDeLaLigneCLK;

      if(etatActuelDeLaLigneCLK == LOW) {

        if(etatActuelDeLaLigneCLK != etatActuelDeLaLigneDT) {
          a2dp_sink.set_volume(a2dp_sink.get_volume()+5);
        }
        else {
            a2dp_sink.set_volume(a2dp_sink.get_volume()-5);
        }

        delay(1);
        
      }
    }

    if(readNEXT == HIGH){
        a2dp_sink.next();
    }

    if(readPERVIOUS == HIGH){
        a2dp_sink.previous();
    }

  delay(10);

}