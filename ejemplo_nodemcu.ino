
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

#include <DHT_U.h>
#include <DHT.h>

#define LED_BUILTIN 2

#define DHTPIN 12
#define DHTTYPE DHT22

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const char* ssid     = "";
const char* password = "";
const char* host = "2.139.156.21";
const int httpPort = 80;
const String station = "A-01";

int estado_led = 0;

DHT dht(DHTPIN, DHTTYPE);

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
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  wifi_setup();

  dht.begin();

}


void loop() {;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  if (isnan(h) || isnan(t)) {
    display.println("Error... ");
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    display.print("Node: ");
    display.println(station);
    display.print("T: ");
    display.print(t);
    display.println("");
    display.print("H: ");
    display.print(h);
    display.println("%");
    display.print("Ts: ");
    display.print(hic);
    display.println("");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  display.display();

  send_data(t, h);
  
  delay(60000);
}

void send_data(float tem, float hum) {
  WiFiClient client;
  if (!client.connect(host, httpPort)){
    Serial.println("connection failed");
    return;
  }
  String url = "http://2.139.156.21/iot/intro_data.php";
  String data = "station='" + station + "'&tem=" + String(tem) + "&hum=" + String(hum);
  Serial.print("Requesting URL: ");
  Serial.println(url);
//  Serial.println(data);
  
  client.print(String("POST ") + url + " HTTP/1.0\r\n"+
               "Host: " + host + "\r\n" +
               "Accept: *" + "/" + "*\r\n" +
               "Content-Lenght: " + data.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "\r\n" + data);
  delay(20);

  Serial.println("Respond:");
  while (client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();

  Serial.println("closing connection...");
  
}


void  wifi_setup(){
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

