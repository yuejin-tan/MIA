//led.cpp
//led 实现函数
//copyright @ MIA project
//version 4.0

#include "led.h"


led::led(int a, int b, int c, int d, int e, int f, int g, int dp)
{
    ledPintab[0] = a;
    ledPintab[1] = b;
    ledPintab[2] = c;
    ledPintab[3] = d;
    ledPintab[4] = e;
    ledPintab[5] = f;
    ledPintab[6] = g;
    ledPintab[7] = dp;

    for (int i = 0; i < 8; i++)
    {
        pinMode(ledPintab[i], OUTPUT);
    }
}

led::~led()
{
}

void led::shownum(int number)
{

    for (int i = 0; i < 8; i++)
    {

        digitalWrite(ledPintab[i], bitRead(numtab[number], 7 - i));
    }
}

void led::ledwave(int direction)
{
    static int flag = 0;
    if (direction == 1 || direction == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            digitalWrite(ledPintab[i], bitRead(dirtab[direction], 7 - i));
        }
        flag = direction;
    }
    if (direction == 2)
    {
        if (flag == 0)
        {
            ledwave(1);
            flag == 1;
        }
        else
        {
            ledwave(0);
            flag == 0;
        }
    }
}
void led::ledcycle(int cycleNum)
{
    static int flag = 0;
    if (cycleNum >= 0 && cycleNum <= 5)
    {
        for (int i = 0; i < 8; i++)
        {

            digitalWrite(ledPintab[i], 1);
            digitalWrite(ledPintab[cycleNum], 0);
        }
        flag = cycleNum;
    }

    if (cycleNum == 6)
    {
        for (int i = 0; i < 8; i++)
            digitalWrite(ledPintab[i], 1);
    }
    if (cycleNum == 7)
    {

        ledcycle((flag + 1) % 6);
    }
    if (cycleNum == 8)
    {

        ledcycle((flag + 5) % 6);
    }
}
void led::ledshow(unsigned int ledshowNum)
{
    for (int i = 0; i < 8; i++)
    {
        if (bitRead(ledshowNum, i) == 1)
            digitalWrite(ledPintab[i], 0);
        if (bitRead(ledshowNum, i) == 0)
            digitalWrite(ledPintab[i], 1);
    }
}
void led::ledDp(int Num)
{
    switch (Num)
    {
    case 0:
        digitalWrite(ledPintab[7], 1);
        break;
    case 1:
        digitalWrite(ledPintab[7], 0);
        break;
    case 2:
        digitalWrite(ledPintab[7], !digitalRead(ledPintab[7]));
        break;
    default:
        break;
    }
}