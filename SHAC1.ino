#include <DHT.h>
#include <Arduino.h>
#define IR_SENSOR_VEHICLE 2  // IR Sensor for vehicle detection
#define IR_SENSOR_OBSTACLE 3 // IR Sensor for obstacle detection
#define RELAY_MOTOR 4        // Relay for Garage Door Motor

#define PIR_SENSOR_OUTSIDE 6 // PIR sensor outside Room 1 (for opening door)
#define PIR_SENSOR_INSIDE 7  // PIR sensor inside Room 1 (for light control)
#define LDR_SENSOR A0        // LDR Sensor for darkness detection
#define ROOM_LIGHT 8         // Relay for Room 1 light

#define DHTPIN 9             // DHT22 Humidity Sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define FAN 10               // Relay for Fan control

void setup() {
    Serial.begin(115200);
    dht.begin();

    pinMode(IR_SENSOR_VEHICLE, INPUT);
    pinMode(IR_SENSOR_OBSTACLE, INPUT);
    pinMode(PIR_SENSOR_OUTSIDE, INPUT);
    pinMode(PIR_SENSOR_INSIDE, INPUT);
    pinMode(LDR_SENSOR, INPUT);

    pinMode(RELAY_MOTOR, OUTPUT);
    pinMode(ROOM_LIGHT, OUTPUT);
    pinMode(FAN, OUTPUT);

    digitalWrite(RELAY_MOTOR, LOW);
    digitalWrite(ROOM_LIGHT, LOW);
    digitalWrite(FAN, LOW);
}

void loop() {
    // *Garage Control*
    if (digitalRead(IR_SENSOR_VEHICLE) == LOW) {
        digitalWrite(RELAY_MOTOR, HIGH);
        Serial.println("Vehicle detected! Opening garage door...");
    } else {
        digitalWrite(RELAY_MOTOR, LOW);
    }

    if (digitalRead(IR_SENSOR_OBSTACLE) == LOW) {
        Serial.println("Obstacle detected! Stopping garage door.");
    }

    //  *Room 1 Automation*
    if (digitalRead(PIR_SENSOR_OUTSIDE) == HIGH) {
        Serial.println("Motion detected outside! Opening Room 1 door.");
    }

    int ldrValue = analogRead(LDR_SENSOR);
    if (digitalRead(PIR_SENSOR_INSIDE) == HIGH && ldrValue < 500) {
        digitalWrite(ROOM_LIGHT, HIGH);
        Serial.println("Motion detected inside & it's dark! Turning ON Room 1 light.");
    } else {
        digitalWrite(ROOM_LIGHT, LOW);
        Serial.println("Room 1 conditions not met! Turning OFF light.");
    }

    // *Room 2 Fan Control Based on Humidity*
    float humidity = dht.readHumidity();
    if (humidity > 60) {
        digitalWrite(FAN, HIGH);
        Serial.println("Humidity High! Turning ON fan.");
    } else {
        digitalWrite(FAN, LOW);
        Serial.println("Humidity Low! Turning OFF fan.");
    }

    delay(5000);
}