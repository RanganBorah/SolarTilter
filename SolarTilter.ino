#include <Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Servo tiltServo;   
Servo panServo;   

int tiltAngle = 90;
int panAngle  = 90;

const int STEP_DEGREES = 2;
const int HOLD_DELAY_MS = 150;

unsigned long lastTiltUpTime = 0;
unsigned long lastTiltDownTime = 0;
unsigned long lastPanLeftTime = 0;
unsigned long lastPanRightTime = 0;

Adafruit_INA219 ina219;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

float peakPower_mW = 0.1;   
unsigned long lastSensorUpdate = 0;
const unsigned long SENSOR_INTERVAL_MS = 300;  
void setup() {
  tiltServo.attach(PA6);
  panServo.attach(PA7);
  tiltServo.write(tiltAngle);
  panServo.write(panAngle);

  pinMode(PA0, INPUT_PULLUP);   
  pinMode(PA1, INPUT_PULLUP);   
  pinMode(PA2, INPUT_PULLUP);  
  pinMode(PA3, INPUT_PULLUP);  

    Wire.begin();

    ina219.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Solar Tracker Ready");
  display.display();
  delay(500);
}

void loop() {
  unsigned long now = millis();

   if (digitalRead(PA0) == LOW) {
    if (now - lastTiltUpTime > HOLD_DELAY_MS) {
      tiltAngle = constrain(tiltAngle + STEP_DEGREES, 0, 180);
      tiltServo.write(tiltAngle);
      lastTiltUpTime = now;
    }
  }
  if (digitalRead(PA1) == LOW) {
    if (now - lastTiltDownTime > HOLD_DELAY_MS) {
      tiltAngle = constrain(tiltAngle - STEP_DEGREES, 0, 180);
      tiltServo.write(tiltAngle);
      lastTiltDownTime = now;
    }
  }
  if (digitalRead(PA2) == LOW) {
    if (now - lastPanLeftTime > HOLD_DELAY_MS) {
      panAngle = constrain(panAngle - STEP_DEGREES, 0, 180);
      panServo.write(panAngle);
      lastPanLeftTime = now;
    }
  }
  if (digitalRead(PA3) == LOW) {
    if (now - lastPanRightTime > HOLD_DELAY_MS) {
      panAngle = constrain(panAngle + STEP_DEGREES, 0, 180);
      panServo.write(panAngle);
      lastPanRightTime = now;
    }
  }

  if (now - lastSensorUpdate > SENSOR_INTERVAL_MS) {
    lastSensorUpdate = now;

    float busVoltage = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();
    float power_mW = ina219.getPower_mW();

    if (power_mW > peakPower_mW) {
      peakPower_mW = power_mW;
    }
    float efficiency = (power_mW / peakPower_mW) * 100.0;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("V: ");
    display.print(busVoltage, 2);
    display.println(" V");

    display.print("I: ");
    display.print(current_mA, 1);
    display.println(" mA");

    display.print("P: ");
    display.print(power_mW, 1);
    display.println(" mW");

    display.print("Eff: ");
    display.print(efficiency, 0);
    display.println(" %");

    display.display();
  }

  delay(20);
}