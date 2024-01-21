#include "mbed.h"
#include "setup.h"
#include "drivers/LCD_DISCO_F429ZI.h"
SPI spi(PF_9, PF_8, PF_7,PC_1,use_gpio_ssel); // mosi, miso, sclk, cs

// we sample every 0.5 second over a period of 20 seconds
const int totalSample = 40;

float sampleX[totalSample];
float sampleY[totalSample];
float sampleZ[totalSample];
float speed[totalSample];

float totalDistance = 0.0f;
float currentTime = 0.0f;
uint8_t write_buf[32];
uint8_t read_buf[32];

// gobal varible to populate the sample arrays
volatile int offset = -1;
// global varible to control the sample rate
volatile int sampleFlag = 0;

EventFlags flags;
//The spi.transfer function requires that the callback
//provided to it takes an int parameter
void spi_cb(int event){
  flags.set(SPI_FLAG);
  
};

LCD_DISCO_F429ZI lcd;

void spi_setUp(){
  // This is inspired by recitation 5
  // Setup the spi for 8 bit data, high steady state clock,
  // second edge capture, with a 1MHz clock rate
  spi.format(8,3);
  spi.frequency(1'000'000);
   
  //set lcd display
  lcd.SetTextColor(LCD_COLOR_WHITE);
  lcd.SetBackColor(LCD_COLOR_BLACK);

  write_buf[0]=CTRL_REG1;
  write_buf[1]=CTRL_REG1_CONFIG;
  spi.transfer(write_buf,2,read_buf,2,spi_cb,SPI_EVENT_COMPLETE );
  flags.wait_all(SPI_FLAG);

  write_buf[0]=CTRL_REG4;
  write_buf[1]=CTRL_REG4_CONFIG;
  spi.transfer(write_buf,2,read_buf,2,spi_cb,SPI_EVENT_COMPLETE );
  flags.wait_all(SPI_FLAG);   
}
// This is called at the sampling rate(every 0.5 sec)
// Tence we can increase the offset here so we can populate the sample arrays in the same rate
// This function is inspired by recitation 5 example 3
void get_gyro_data(){
   int16_t raw_gx,raw_gy,raw_gz;
    float gx, gy, gz;
    //reading the status register. bit 4 of the status register
    //is 1 when a new set of samples is ready
    write_buf[0]=0x27 | 0x80;
    
    do{
      spi.transfer(write_buf,2,read_buf,2,spi_cb,SPI_EVENT_COMPLETE );
      flags.wait_all(SPI_FLAG);

    }while((read_buf[1]&0b0000'1000)==0);

    //prepare the write buffer to trigger a sequential read
    write_buf[0]=OUT_X_L|0x80|0x40;

    //start sequential sample reading
    spi.transfer(write_buf,7,read_buf,8,spi_cb,SPI_EVENT_COMPLETE );
    flags.wait_all(SPI_FLAG);

    //read_buf after transfer: garbage byte, gx_low,gx_high,gy_low,gy_high,gz_low,gz_high
    //Put the high and low bytes in the correct order lowB,Highb -> HighB,LowB
    raw_gx=( ( (uint16_t)read_buf[2] ) <<8 ) | ( (uint16_t)read_buf[1] );
    raw_gy=( ( (uint16_t)read_buf[4] ) <<8 ) | ( (uint16_t)read_buf[3] );
    raw_gz=( ( (uint16_t)read_buf[6] ) <<8 ) | ( (uint16_t)read_buf[5] );

    //printf("RAW|\tgx: %d \t gy: %d \t gz: %d\n",raw_gx,raw_gy,raw_gz);

    gx=((float)raw_gx)*(8.75f*0.017453292519943295769236907684886f / 1000.0f);
    gy=((float)raw_gy)*(8.75f*0.017453292519943295769236907684886f / 1000.0f);
    gz=((float)raw_gz)*(8.75f*0.017453292519943295769236907684886f / 1000.0f);
    
    
    float filtered_gz = 0.0f;

    filtered_gz = FILTER_COEFFICIENT * gz + (1 - FILTER_COEFFICIENT) * filtered_gz;
    // printf("Filtered gz with offset : %4.5f\n", filtered_gz + 0.00274);
    sampleX[offset] = gx;
    sampleY[offset] = gy;
    sampleZ[offset] = gz;

    speed[offset] = abs(sampleZ[offset] * RADIUS);

    printf("Linear velocity %d \t v: %4.1f m/s\n", offset, speed[offset]);
    totalDistance += roundf(10.0f * speed[offset])/10.0f * 0.5;
}

// update the global variables on every function call
void set_sample_flag(){
    offset ++;
    sampleFlag = 1;
    currentTime += 0.5f;
}

int check_sample_flag(){
    return sampleFlag != 1 ? 0 : 1;
}

void reset_sample_flag(){
    sampleFlag = 0;
}
//sets the background layer 
//to be visible, transparent, and
//resets its colors to all black
void setup_background_layer(){
  lcd.SelectLayer(BACKGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_BLACK);
  lcd.SetTextColor(LCD_COLOR_GREEN);
  lcd.SetLayerVisible(BACKGROUND,ENABLE);
  lcd.SetTransparency(BACKGROUND,0x7Fu);
}

char display_buf[2][60];

// This is inspired by recitation 7
void lcd_display(){
    //creates c-strings in the display buffers, in preparation
    //for displaying them on the screen
    snprintf(display_buf[0],60,"Distance: %4.1fm", totalDistance);
    snprintf(display_buf[1],60,"Avg speed: %4.1fm/s", totalDistance/20.0f);

    //display the buffered string on the screen
    lcd.DisplayStringAt(0, LINE(0), (uint8_t *)display_buf[0], LEFT_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)display_buf[1], LEFT_MODE);
}

// every 20 seconds we collect total distance and average pace 
// then reset the global variable for the next 20 seconds
void output_data_and_reset_cycle(){
    if(currentTime == 20.0){
        // display in the monitor
        printf("Total distance is:\t %4.1f meters\n", totalDistance);
        printf("Total Average speed is:\t %4.1f m/s\n", totalDistance/20.0f);
        // display on LCD 
        lcd_display();
        offset = -1;
        totalDistance = 0;
        currentTime = 0.0f;
    }
}