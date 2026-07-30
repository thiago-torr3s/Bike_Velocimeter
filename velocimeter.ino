#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h>

#define hall A3
#define wheelradius 37  //the distance in centimeters from de center of the wheel to the ground
#define pi 3.141592
#define OLED_RESET -1

int velocity;
float frequency;
unsigned long time1 = 0;
unsigned long time2 = 0;
int norepeat = 0;

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
static const unsigned char PROGMEM image_paint_6_bits[] = { 0x80 };
int x2, y2;
void drawScreen_1(int vel);


void setup() {
  pinMode(hall, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  delay(100);
}

void loop() {

  if (digitalRead(hall) == LOW && norepeat == 0) {  //registers the time when a magnet is detected
    norepeat = 1;
    time2 = time1;
    time1 = millis();
  } else if (digitalRead(hall) == HIGH) {
    norepeat = 0;
  }

  //calculating the frequency
  if (time1 == 0 || time2 == 0) {  //when the bike isnt moving at the start
    frequency = 0;
  } else {
    frequency = (1.0 / (float)(time1 - time2)) * 1000;  // frequency in Hz;
  }

  //when the bike isnt moving during the session
  if ((millis() - time1) / 1000.0 > 3.3) {
    frequency = 0;
  }

  velocity = 2 * pi * (wheelradius * 0.01) * frequency * 3.6;  //velocity in km/h

  Serial.print("VELOCIDADE: ");
  Serial.print(velocity);
  Serial.println();

  drawScreen_1(velocity);

  delay(50);
}

//screen made with Lopaka
void drawScreen_1(int vel) {
  display.clearDisplay();  

  display.drawCircle(35, 41, 32, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setFont(&Org_01);

  display.setCursor(75, 15);
  display.print("VELOCITY");

  display.setCursor(88, 50);
  display.print("Km/h");

  display.setTextSize(3);
  display.setCursor(81 + 11, 36);
  display.print(vel);

  display.fillRect(-1, 54, 73, 10, 0);

  display.drawBitmap(35, 42, image_paint_6_bits, 1, 1, 1);

  display.setTextSize(1);
  display.setCursor(30, 16);
  display.print("20");  
  display.setCursor(8, 52);
  display.print("0");  
  display.setCursor(52, 52);
  display.print("40"); 
  display.setCursor(11, 27);
  display.print("10");  
  display.setCursor(49, 27);
  display.print("30");  

  x2 = 28 * cos((199.0 - 5.45 * vel) * (pi / 180.0)) + 35;
  y2 = -28 * sin((199.0 - 5.45 * vel) * (pi / 180.0)) + 41;
  display.drawLine(x2, y2, 35, 41, 1);

  display.display();
}
