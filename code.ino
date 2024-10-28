#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define BUZZER_PIN 8
#define RELAY_PIN 9

SoftwareSerial gpsSerial(3, 4); // RX, TX
TinyGPSPlus gps;

double BorderLat = 12.83870; // Border Latitude Threshold

const int rs = 12, en = 11, d4 = 2, d5 = 6, d6 = 7, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  gpsSerial.begin(9600);
  Serial.println("GPS module initialized. Waiting for data...");
  lcd.print("Waiting for GPS");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    Serial.print(c);
    if (gps.encode(c)) {
      if (gps.location.isUpdated()) {
        double Currlat = gps.location.lat();

        delay(2000); // Wait for data to stabilize

        lcd.clear();
        lcd.setCursor(0, 0);

        if (Currlat > BorderLat) {
          lcd.print("Safe Zone");
          digitalWrite(BUZZER_PIN, LOW);
          digitalWrite(RELAY_PIN, HIGH);
        } else {
          lcd.print("Unsafe Zone");
          digitalWrite(BUZZER_PIN, HIGH);
          digitalWrite(RELAY_PIN, LOW);
        }

        lcd.setCursor(0, 1);
        lcd.print(Currlat, 5); // Print latitude with 5 decimal places

        Serial.println();
        Serial.print("Latitude: ");
        Serial.println(Currlat, 5);
      }
    }
  }
}

