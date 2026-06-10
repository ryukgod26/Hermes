#include <Wire.h>
#include<Adafruit_VL53L0X.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#include<DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3c
#define ECHO_PIN 9
#define TRIG_PIN 10
#define DHT_PIN A1
#define BTN_PIN 3
#define DHTTYPE DHT11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHT_PIN,DHTTYPE);

// Adafruit_VL53L0X sensor_config = Adafruit_VL53L0X();

bool temp_state = 0; //State in which Screen Shows temperature data
bool lastBtnState = HIGH;
bool currentBtnState = HIGH;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  dht.begin();

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.println("OLED Display Intialization failed.");
    while(1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("Booting System...");
  display.display();
  delay(500);

  // if(!sensor_config.begin()){
  //   Serial.println("Failed to Load the IR Sensor");
  //   display.clearDisplay();
  //   display.setCursor(0, 10);
  //   display.println("Sensor Error");
  //   display.display();
  //   // while(1);
  // }
}

void loop() {

  currentBtnState = digitalRead(BTN_PIN);

  if(lastBtnState == HIGH && currentBtnState == LOW){
    temp_state = !temp_state;
    delay(50);
  }
  lastBtnState = currentBtnState;

  // VL53L0X_RangingMeasurementData_t measure;

  // sensor_config.rangingTest(&measure,false);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);


  if(temp_state == false){
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    display.println("DHT SENSOR\n");
    
    if(isnan(h) || isnan(t)){
      display.println("Failed to read\n from DHT Sensor");
    } else{
      display.print("Temp: ");
      display.print(t);
      display.print(" °C");
      
      display.println("");

      display.print("Hum: ");
      display.print(h);
      display.print(" %");
    }

  } else{
    digitalWrite(TRIG_PIN, LOW);
    delay(2);

    digitalWrite(TRIG_PIN, HIGH);
    delay(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    int dist = duration * 0.034 / 2;

    display.println("OBJECT DISTANCE\n");
    display.setTextSize(2);

    if(dist <= 0|| dist > 400){
      display.setTextSize(1);
      display.println("OUT OF RANGE!!");
    } else{
      display.print(dist);
      display.println(" cm");
    }
  }

  display.display();
  delay(100);

}