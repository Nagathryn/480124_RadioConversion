

#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include <ezButton.h>

#define I2S_DOUT      25
#define I2S_BCLK      27    // I2S
#define I2S_LRC       26

//#define ADC_SOUND     35
#define B_PLAY        21
#define B_NEXT        18
#define B_PREVIOUSE   19

#define ENCODER_DT     2
#define ENCODER_CLK    4

int iLevelSound;
int iStatePLAY = 0;
int iLastStatePLAY;
int lastButtonState;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

ezButton button1(B_PLAY); 
ezButton button2(B_NEXT);  
ezButton button3(B_PREVIOUSE);  

BluetoothA2DPSink a2dp_sink;

void setup() {

  Serial.begin(115200);

  int iDebounce = 100;
  button1.setDebounceTime(iDebounce); 
  button2.setDebounceTime(iDebounce); 
  button3.setDebounceTime(iDebounce);

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
  a2dp_sink.start("LaFabrique_Grundig");

	pinMode(ENCODER_CLK,INPUT);
	pinMode(ENCODER_DT,INPUT);
  lastStateCLK = digitalRead(ENCODER_CLK);
   
  delay(200);
}

void loop()
{

  button1.loop(); 
  button2.loop(); 
  button3.loop(); 

  int btn1State = button1.getState();
  int btn2State = button2.getState();
  int btn3State = button3.getState();

if(btn1State != iLastStatePLAY)
    {
      iLastStatePLAY = btn1State;

      if(btn1State == HIGH)
      {
        iStatePLAY = (iStatePLAY == 0) ? 1 : 0;
        if(iStatePLAY)a2dp_sink.pause();
        else a2dp_sink.play();
      }
  }

    if(button2.isPressed()) {
        a2dp_sink.next();
    }

    if(button3.isPressed()){
        a2dp_sink.previous();
    }

currentStateCLK = digitalRead(ENCODER_CLK);

	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		if (digitalRead(ENCODER_DT) != currentStateCLK) {
      if(a2dp_sink.get_volume() -5 >= 0) a2dp_sink.set_volume(a2dp_sink.get_volume() - 5);
		} 
    else {
			if(a2dp_sink.get_volume() + 5 <= 127) a2dp_sink.set_volume(a2dp_sink.get_volume() + 5);
			currentDir ="CW";
		}
	}

	lastStateCLK = currentStateCLK;

}