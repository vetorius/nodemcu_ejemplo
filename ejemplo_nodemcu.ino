
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#define LED_BUILTIN 2

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const char* ssid     = "LSGV";
const char* password = "Nhmq100ad";

int estado_led = 0;

void setup()   {                
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  WiFi.begin(ssid, password);
  display.print("Connecting");
  display.display();
  digitalWrite(LED_BUILTIN, HIGH);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (estado_led != 0){
      estado_led = 0;
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      estado_led = 1;
      digitalWrite(LED_BUILTIN, LOW);
    }
    display.print(".");
    display.display();
  }
  
  digitalWrite(LED_BUILTIN, LOW);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connected ");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();

}


void loop() {
  
}

