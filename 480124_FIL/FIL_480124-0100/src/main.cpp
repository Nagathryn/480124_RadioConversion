

#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include <OneButton.h>

#define I2S_DOUT 25
#define I2S_BCLK 27 // I2S
#define I2S_LRC 26

// #define ADC_SOUND     35
#define B_PLAY 21
#define B_NEXT 18
#define B_PREVIOUSE 19

#define ENCODER_DT 2
#define ENCODER_CLK 4

int iLevelSound;
int iStatePLAY = 0;
int iLastStatePLAY;
int lastButtonState;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
unsigned long lastButtonPress = 0;

OneButton bouton1(B_PLAY);
OneButton bouton2(B_NEXT);
OneButton bouton3(B_PREVIOUSE);

BluetoothA2DPSink a2dp_sink;

void bpPlay();
void bpNext();
void bpPerviouse();

void setup()
{

  Serial.begin(115200);

  bouton1.attachClick(bpPlay);
  bouton2.attachClick(bpNext);
  bouton3.attachClick(bpPerviouse);

  i2s_pin_config_t my_pin_config = {
      .bck_io_num = 27,
      .ws_io_num = 26,
      .data_out_num = 25,
      .data_in_num = I2S_PIN_NO_CHANGE};

  static const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 44100, // corrected by info from bluetooth
      .bits_per_sample = (i2s_bits_per_sample_t)32,
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
  a2dp_sink.start("LaFabrique_dev_2 ");

  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  lastStateCLK = digitalRead(ENCODER_CLK);

  delay(200);
}

void loop()
{

  bouton1.tick();
  bouton2.tick();
  bouton3.tick();

  currentStateCLK = digitalRead(ENCODER_CLK);

  if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
  {

    if (digitalRead(ENCODER_DT) != currentStateCLK)
    {
      if (a2dp_sink.get_volume() - 5 >= 0)
        a2dp_sink.set_volume(a2dp_sink.get_volume() - 5);
    }
    else
    {
      if (a2dp_sink.get_volume() + 5 <= 127)
        a2dp_sink.set_volume(a2dp_sink.get_volume() + 5);
    }
  }

  lastStateCLK = currentStateCLK;
}

/// @brief Fonction de callback pour le changement de play a pause 
void bpPlay()
{
  int btn1State = bouton1.state();

  if (btn1State != iLastStatePLAY)
  {
    iLastStatePLAY = btn1State;

    if (btn1State == HIGH)
    {
      iStatePLAY = (iStatePLAY == 0) ? 1 : 0;
      if (iStatePLAY)
        a2dp_sink.pause();
      else
        a2dp_sink.play();
    }
  }
}

/// @brief fonction de callback pour le passage au morceau suivant
void bpNext()
{
  a2dp_sink.next();
}

/// @brief fonction de callback pour le passage au morceau précedent 
void bpPerviouse()
{
  a2dp_sink.previous();
}

/// @brief Initialisation des parametres de lecture de l'adc
void adsInit()
{

}

void adsRead()
{

}