#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Moisture sensor
#define MOISTURE_PIN 34
int dryValue = 4095;      // calibration of moisture sensor (in air)
int wetValue = 1500;      // calibration of moisture sensor (in water)

// pH sensor
#define PH_SENSOR 35
float calibration = 1;   // Calibration of pH sensor

// RGB LED pins
#define BLUE_PIN 17
#define GREEN_PIN 5
#define RED_PIN 19

void setup() {
  Serial.begin(115200);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display work failed :(");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.println("==================================================== The ESP32 Farmers's Friend ========================================================================");

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("The ESP32 Farmers's Friend :)");
  display.display();
  delay(3000);
}
void loop() {
  //Moisture Sensor
  int moistureValue = analogRead(MOISTURE_PIN);
  int moisturePercent = map(moistureValue, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  String soilType;
  if (moisturePercent < 15) {
    soilType = "Very Dry";
  } else if (moisturePercent < 35) {
    soilType = "Dry";
  } else if (moisturePercent < 60) {
    soilType = "Moist";
  } else if (moisturePercent < 85) {
    soilType = "Wet";
  } else {
    soilType = "Very Wet";
  }
  //pH Sensor
int analogValue = analogRead(PH_SENSOR);
float voltage = analogValue * (3.3 / 4095.0);
float phValue = 7 + ((2.5 - voltage) * calibration);

String phType;

digitalWrite(RED_PIN, LOW);
digitalWrite(GREEN_PIN, LOW);
digitalWrite(BLUE_PIN, LOW);

if (phValue <= 6) {
  phType = "Acidic";
  digitalWrite(RED_PIN , HIGH);
} else if (phValue > 6 && phValue < 8) {
  phValue = 7;
  phType = "Neutral";
  digitalWrite(GREEN_PIN, HIGH);
} else {
  phType = "Basic";
  digitalWrite(BLUE_PIN , HIGH);
}
  //Serial Output
  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("% --> ");
  Serial.println(soilType);

  Serial.print("pH Value: ");
  Serial.print(round(phValue));
  Serial.print(" --> ");
  Serial.println(phType);

  Serial.println(moistureValue);

  Serial.println("----------------------------------------");

  //OLED Output
  display.clearDisplay();

  display.setCursor(0, 0);
  display.setTextSize(1.5);
  display.print("Moisture: ");
  display.print(moisturePercent);
  display.print("%");

  display.setCursor(0, 12);
  display.setTextSize(1.5);
  display.print("Soil: ");
  display.print(soilType);

  display.setCursor(0, 28);
  display.setTextSize(1.5);
  display.print("PH: ");
  display.print(round(phValue));
  display.print(" (");
  display.print(phType);
  display.print(")");

  display.display();

  delay(1000);
}