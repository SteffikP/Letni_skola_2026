#include <SHT40.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
//#include <SH1106.h>
//#include <SH1106Brzo.h>
//#include <SH1106Spi.h>
//#include <SH1106Wire.h>
#include <SSD1306.h>
//#include <SSD1306Brzo.h>
#include <SSD1306I2C.h>
#include <SSD1306Spi.h>
#include <SSD1306Wire.h>

#define led 4  
#define MY_SDA 8
#define MY_SCL 10
#define MY_ADC 3

SHT40 sht40;
//OLED display init using i2c
SSD1306Wire display(0x3c, MY_SDA, MY_SCL);

char buff[30];
char buffpudy[30];
int analogVolts = 0;
float floatVolts = 0.0;


void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);   
  Serial.begin(115200);
  while(!Serial);  
  analogReadResolution(12);

  Serial.begin(115200);
  Wire.begin(MY_SDA,MY_SCL);
  display.init();
  display.setContrast(255);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.display();


 if (!sht40.begin(&Wire)) {
    Serial.println("SHT40 initialization failed!");
    Serial.println("Please check your wiring and I2C connections.");
    while(1) {
      delay(1000);
    }
  }

  Serial.println("SHT40 initialized successfully");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

  analogVolts = analogReadMilliVolts(MY_ADC);
  floatVolts = analogVolts/1000.0;

  if(floatVolts > 2.6) {
    snprintf(buffpudy, 20, "Kvetina: sucha");
  }
  else if (floatVolts < 2.1)
  {
    snprintf(buffpudy, 20, "Kvetina: prelita");
  }
  else
  {
    snprintf(buffpudy, 20, "Kvetina: zalita");
  }

  display.clear();
  
  snprintf(buff, 20, "Teplota: %.1f °C", sht40.readTemperatureC());
  display.drawString(0, 0, buff);

  snprintf(buff, 20, "Vlhkost: %.1f %%", sht40.readHumidityRH());
  display.drawString(0, 16, buff);

  display.drawString(0, 32, buffpudy);

  display.display();
 
  digitalWrite(led, HIGH);
  // print out the values you read to the terminal:
  Serial.printf("ADC volts value = %f\n", floatVolts);
  digitalWrite(led, LOW);
  
  delay(100);  // delay in between reads for clear read from serial
}
