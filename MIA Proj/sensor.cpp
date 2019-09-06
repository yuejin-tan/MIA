//sensor.cpp
//传感器模块组
//copyright @ MIA project
//version 3.0
#include "sensor.h"

voicesensor::voicesensor(int pinA,int pinD) : voicePinA(pinA),voicePinD(pinD)
{
    pinMode(voicePinA, INPUT);
    pinMode(voicePinD, INPUT);
}
voicesensor::~voicesensor() {}

int voicesensor::hasvoice()
{
    return analogRead(voicePinD) > 750 ? 0 : 1;
}
