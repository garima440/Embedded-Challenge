#include "mbed.h"
#include "func.h"
#include "setup.h"
int main() {
  // LCD and SPI setup 
  spi_setUp();  
  // set a ticker to set the sample flag to be true every 0.5 second
  Ticker t; 
  t.attach(&set_sample_flag, 500ms);

  //sets the background layer 
  //to be visible, transparent, and
  //resets its colors to all black
  setup_background_layer();

  while (1) {
    // if the sample flag is 1, we sample the Z axis data 
    if (check_sample_flag()){
        // this is called at the sampling rate(every 0.5 sec) based on the sample flag
        get_gyro_data();
        // reset the sample flag for next sampling
        reset_sample_flag();
        // display data every 20 seconds and the reset the global variables
        output_data_and_reset_cycle();
    }
  }
}
