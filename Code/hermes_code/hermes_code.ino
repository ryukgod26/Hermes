#include <Wire.h>
#include<Adafruit_VL53L0X.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3c

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_VL53L0X sensor_config = Adafruit_VL53L0X();

void setup(){
  Serial.begin(9600);
  Wire.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.println("OLED Display Intialization failed.");
    // while(1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Booting System...");
  display.display();
  delay(500);

  if(!sensor_config.begin()){
    Serial.println("Failed to Load the IR Sensor");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Sensor Error");
    display.display();
    // while(1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  sensor_config.rangingTest(&measure,false);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Object Distance: ");

  display.setCursor(0, 24);

  if (measure.RangeStatus != 4){
    display.setTextSize(3);
    display.print(measure.RangeMilliMeter);
    display.setTextSize(1);
    display.print(" mm");
  } else{
    display.setTextSize(2);
    display.print("Out of range.");
  }

  display.display();
  delay(100);
}