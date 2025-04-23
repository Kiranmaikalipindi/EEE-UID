#include <Servo.h>
#include <DHT.h>

// PIN
#define IR_PIN 2
#define SERVO_PIN 9
#define LDR_PIN A0
#define LED_PIN 13            
#define DHT_PIN 4

#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

Servo garageServo;

bool doorOpen = false;
unsigned long lastDetectedTime = 0;
const unsigned long CLOSE_DELAY = 4000; // 4 seconds delay

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  garageServo.attach(SERVO_PIN);
  garageServo.write(0); // Start closed

  Serial.begin(9600);
  delay(2000); 
  dht.begin();
}


void loop() {
  int irValue = digitalRead(IR_PIN);
  int ldrValue = analogRead(LDR_PIN);
  float temperature = dht.readTemperature();

  // Garage: Garage Door (IR + Servo)
 if (irValue == LOW) {
    garageServo.write(180);
    doorOpen = true;
    lastDetectedTime = millis();
    Serial.println("Garage: Object Detected - Shutter OPENED to 180°");
  } else {
    if (doorOpen && (millis() - lastDetectedTime >= CLOSE_DELAY)) {
      garageServo.write(0);
      doorOpen = false;
      Serial.println("Garage: No Object for 4s - Shutter CLOSED to 0°");
    }
  }
  // Room 1: LED Control via LDR
 
  if (ldrValue < 700) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Room1: Dark - LED ON");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Room1: Bright - LED OFF");
  }

  // Room 2: Fan Suggestion via Temperature
  if (isnan(temperature)) {
    Serial.println("Room2: Normal Temperature - Fan not needed");
  } else if (temperature >= 30) {
    Serial.print("Room3: Hot (");
    Serial.print(temperature);
    Serial.println("°C) - Suggest turning ON Fan");
  }

  delay(2000);
}
