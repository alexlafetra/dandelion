#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Wire.h>

#undef CFG_TUH_RPI_PIO_USB
#define CFG_TUH_RPI_PIO_USB 1


//SPI Bus for the screen
#define SPI1_RX 12
#define SPI1_TX 11
#define OLED_CS 13
#define OLED_DC 16
#define OLED_RESET 7
#define SPI1_SCK 10

#define screenHeight 64
#define screenWidth 128

/*

  Code for the Stepchild's screen. It's a derivation of the Adafruit_SSD1306 class, so it inherits the same Adafruit_GFX
  behavior but has some added functionality like writing the screen buffer to the USB port for video capture

*/
//Adafruit GFX library
//there's a pico optimized version here, but it's unclear what's optimized in it/if it matters for the SSD1306: https://github.com/Bodmer/Adafruit-GFX-Library
#include <Adafruit_GFX.h>
//grab the display libraries
#include <Adafruit_SSD1306.h>

//setting up screen w/I^2C
#define SCREEN_ADDR 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SSD1306_NO_SPLASH 1

#define DISPLAY_UPRIGHT 2
#define DISPLAY_UPSIDEDOWN 0
#define DISPLAY_SIDEWAYS_R 3
#define DISPLAY_SIDEWAYS_L 1

//this is a derived class so that you can still call the same draw() functions on it!
class USBSerialCaptureCard: public Adafruit_SSD1306{
  public:
    bool sendScreenViaUSB = false;
    uint32_t timeLastFrameSent = 0;//stores the time the last frame was sent
    const uint8_t minMsDelay = 10;//min amount of time to wait (ms) before sending a new frame
    //ridiculous derived constructor (I^2C)
    USBSerialCaptureCard(uint8_t w, uint8_t h, TwoWire *twi = &Wire, int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
                   uint32_t clkAfter = 100000UL):Adafruit_SSD1306(w,h,twi,rst_pin,clkDuring,clkAfter){}
    //second ridiculous derived constructor (SPI)
    USBSerialCaptureCard(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                   int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL):Adafruit_SSD1306(w,h,spi,dc_pin,rst_pin,cs_pin,bitrate){}
    void init(){
      //start display
      begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR);
      //Set the display rotation (which is ~technically~ upside down)
      setRotation(DISPLAY_UPRIGHT);
      //turn text wrapping off, so our menus look ok
      setTextWrap(false);
      setTextColor(SSD1306_WHITE);
    }

    void display(){
      Adafruit_SSD1306::display();//call the original display method
    }
};

USBSerialCaptureCard display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI1, OLED_DC, OLED_RESET, OLED_CS);
// USBSerialCaptureCard display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "graphics/fonts/small.cpp"

//CPU 0 Setup
void setup() {

  //starting serial monitor output @ 9600baud for USB communication
  Serial.begin(9600);

  //Set USB device info
  // these two strings must be exactly 32 characters long:
  //                                   0123456789ABCDEF0123456789ABCDEF
  USBDevice.setManufacturerDescriptor("Alex LaFetra Thompson           ");
  USBDevice.setProductDescriptor     ("dandelion test                  ");


  //Start SPI to communicate w/ screen
  SPI1.setCS(OLED_CS);
  SPI1.setRX(SPI1_RX);
  SPI1.setTX(SPI1_TX);
  SPI1.setSCK(SPI1_SCK);
  SPI1.begin();

  display.init();
  display.clearDisplay();

  //wait for tinyUSB to connect, if the USB port is connected (not sure if this is necessary, need to test)
  while (!TinyUSBDevice.mounted()) {
    delay(1);
  }

  //seeding random number generator
  srand(1);

  Serial.setTimeout(1); // VERY important

  Serial.write(255);
}


//this cpu handles time-sensitive things
void loop(){
  if (!Serial) {
    display.clearDisplay();
    printSmall(0, 0, "Waiting for USB...", 1);
    display.display();
  }
  else{
    size_t readSize = Serial.readBytes(display.getBuffer(),1024);
    if (readSize == 1024) {
      // full frame received safely
      display.display();
      //tell laptop you're ready for the next frame
      // Serial.write(255);
    }
  }
}
