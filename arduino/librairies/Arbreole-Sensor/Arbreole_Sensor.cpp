#include "Arbreole_Sensor.h"

Arbreole_Sensor::Arbreole_Sensor(int pinLed, int pinSensor, int calibratingTime = 0) {
	m_pinLed = pinLed;
	m_pinSensor = pinSensor;
	m_sensorValue = 0;
	m_sensorState = LOW;
	m_calibratingTime = calibratingTime;
}

void Arbreole_Sensor::Begin(){
	pinMode(m_pinLed, OUTPUT);
    pinMode(m_pinSensor, INPUT);

    Serial.println("Calibrating the PIR Sensor");
    for (int i=0; i < m_calibratingTime; i++) {
        Serial.print(".");
        delay(1000);
    }
}

void Arbreole_Sensor::Update(){
	m_sensorValue = digitalRead(m_pinSensor);

    if (m_sensorValue == HIGH) {
        digitalWrite(m_pinLed,HIGH);

        if(m_sensorState==LOW){
            Serial.println("Motion detected: <1>");
            m_sensorState = HIGH;
        }
    } else {
        digitalWrite(m_pinLed,LOW);

        if(m_sensorState==HIGH){
            Serial.println("Motion ended: <0>");
            m_sensorState=LOW;
        }
  }
}
