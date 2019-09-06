//button.cpp
//button 实现函数
//copyright @ MIA project
//version 1.0

#include "button.h"

button::button(int buttonOK, int buttonF1, int buttonF2, int buttonF3)
{
    buttonPin[0] = buttonOK;
    buttonPin[1] = buttonF1;
    buttonPin[2] = buttonF2;
    buttonPin[3] = buttonF3;
    for (uint8_t i = 0; i < 4; i++)
    {
        pinMode(buttonPin[i], INPUT_PULLUP);
    }
}
button::~button() {}

uint8_t button::buttonCheck(void)
{
    uint8_t returnValue = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        bitWrite(buttonState, i, (digitalRead(buttonPin[i])));
    }
    uint8_t compareResult = buttonState ^ lastButtonState;
    //delayhere
    delay(15);
    for (uint8_t i = 0; i < 4; i++)
    {
        if ((bitRead(compareResult, i) == 1) && (digitalRead(buttonPin[i]) == LOW))
        {
            bitSet(returnValue, i);
        }
    }
    lastButtonState = buttonState;
    return returnValue;
}

uint8_t button::buttonPeek(void)
{

    uint8_t returnValue = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        if ((digitalRead(buttonPin[i])) == LOW)
        {
            bitSet(returnValue, i);
        }
    }
    return returnValue;
}