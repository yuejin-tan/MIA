//func3.cpp
//功能三
//copyright @ MIA project
//version 1.0

#include "fft_c.h"
#include "led.h"
#include "LiquidCrystal.h"
#include "button.h"
#include "showu8g2.h"
#include "miaTool.h"
extern led led1;
extern LiquidCrystal lcd1;
extern button button1;

extern U8G2_ST7920_128X64_1_8080 u8g2;

void func3(void)
{
    int buttonState = 0;
    double fuckingResult = 0;
    double lastFuckingResult = 0;

    lcd1.clear();
    lcd1.setCursor(0, 1);
    lcd1.print(F("BT Link SetUp..."));
    while (bluetooth1.BTLinkStatus() == 0)
    {
        led1.ledcycle(7);
        buttonState = button1.buttonCheck();
        if (buttonState == 8)
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("no BT link, exit"));
            delay(1000);
            return;
        }
    }
    Serial1.begin(9600);

    //尝试握手
    Serial1.write('a');

    lcd1.clear();
    lcd1.setCursor(2, 1);
    lcd1.print(F("Link initing"));

func4loop1:
    while (Serial1.available() < 1)
    {
        //按键响应
        buttonState = button1.buttonCheck();
        if (buttonState == 0b00001000)
        {
            goto func3end;
        }
        if (buttonState == 0b00000001)
        {
            {

                lcd1.clear();
                lcd1.setCursor(7, 2);
                lcd1.print(F("stoping"));

                while (button1.buttonCheck() != 0b00000001)
                {
                    //empty
                }
                lcd1.clear();
                lcd1.setCursor(2, 1);
                lcd1.print(F("link initing"));
            }
        }
    }
    if (Serial1.read() != 'a')
    {

        Serial1.write('a');
        goto func4loop1;
    }
    Serial1.write('b');
    //握手完成

    lcd1.clear();
    lcd1.setCursor(5, 0);
    lcd1.print(F("link ok!"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("getting linkMode..."));
    lcd1.setCursor(0, 2);

    //获取三位模式标识码
    while (Serial1.available() < 8)
    { //按键响应
        buttonState = button1.buttonCheck();
        if (buttonState == 0b00001000)
        {
            goto func3end;
        }
    }
    lcd1.print(F("Mode: "));
    for (uint8_t i = 0; i < 8; i++)
    {
        lcd1.print(char(Serial1.read()));
    }

    lcd1.setCursor(0, 3);
    lcd1.print(F("enjoy ..."));
    Serial1.write('b');
    delay(1000);

    while (1)
    {
        //按键响应
        buttonState = button1.buttonCheck();
        if (buttonState == 0b00001000)
        {
            goto func3end;
        }
        if (buttonState == 0b00000001)
        {
            {
                lcd1.clear();
                lcd1.setCursor(7, 1);
                lcd1.print(F("stoping"));
                while (button1.buttonCheck() != 0b00000001)
                {
                    //empty
                }
                lcd1.clear();
            }
        } //end

        //改进版触发判断
        fuckingResult = fuckingCompute();

        if (abs(fuckingResult - lastFuckingResult) < 10)
        { //平均消减误差
            lastFuckingResult = (fuckingResult + lastFuckingResult) / 2;
            for (int i = 0; i < 8; i++)
            {
                if (abs(frequencyTab[i + 4] - lastFuckingResult) < 10)
                {
                    Serial1.write(('1' + i));
                    lcd1.clear();
                    lcd1.print(F("tone  "));
                    lcd1.print(i + 1);
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("detected"));
                    lcd1.setCursor(0, 2);
                    lcd1.print(F("try to get respond.."));
                    lcd1.setCursor(0, 3);
                    lcd1.print(F("press back to ignore"));
                    while (Serial1.read() != ('1' + i))
                    {
                        buttonState = button1.buttonCheck();
                        if (buttonState == 0b00001000)
                        {
                            break;
                        }
                    }
                    lcd1.clear();
                    lcd1.setCursor(0, 0);
                    lcd1.print(F("succeed done with"));
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("tone  "));
                    lcd1.print(i + 1);
                    fuckingResult = 0;
                    break;
                }
            }
        }
        else
        {
            if (abs (lastFuckingResult -fuckingResult)> 5)
            {
                lcd1.clear();
                lcd1.print(F("now freqency:"));
                lcd1.setCursor(0, 2);
                lcd1.print(fuckingResult);
            }
        }
        lastFuckingResult = fuckingResult;
    }
func3end:
    Serial1.write('z');
    Serial1.flush();
    Serial1.end();
}
