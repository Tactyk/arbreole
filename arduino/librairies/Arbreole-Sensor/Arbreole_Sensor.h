#ifndef __ARBREOLE_SENSOR__
#define __ARBREOLE_SENSOR__

#include <Arduino.h>

class Arbreole_Sensor
{

    public:
        Arbreole_Sensor(int pinled, int inputpin, int calibratingTime);

        void
            Begin(),
            Update();

    private:
        int
            m_pinLed,
            m_pinSensor,
            m_sensorValue,
            m_sensorState,
            m_calibratingTime,
            m_updateInterval;
        unsigned long
            m_currentMillis,
            m_previousMillis;
};

#endif