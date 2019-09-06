//func1.cpp
//功能1
//copyright @ MIA project
//version 2.0

#ifndef _FUNC1
#define _FUNC1

#include "miaTool.h"
#include "showu8g2.h"
#include "button.h"
#include "led.h"
#include "fft_c.h"
#include "eepromMgr.h"
#include <arduino.h>
#include <LiquidCrystal.h>
#include <avr/pgmspace.h>
extern LiquidCrystal lcd1;

extern led led1;
extern button button1;
extern LiquidCrystal lcd1;
extern eepromMgr eepromMgr1;

const uint8_t toneTab[] = {0, 0, 0, 0, 30, 22, 20, 11, 13, 10, 2, 3};

void showSingleGraph(int num)
{
    lcd1.clear();
    for (int i = 0; i < 4; i++)
    {
        lcd1.setCursor(0, i);
        lcd1.print(F("1 2 3 4 5 6 7 8 9 | "));
    }
    //lcdPrint('*',( (toneTab[num] % 10) == 0 ? toneTab[num] / 10 - 1 : toneTab[num] / 10), ((toneTab[num] % 10) == 0 ? 19 : (toneTab[num] % 10) * 2 - 1));
    lcd1.setCursor(((toneTab[num] % 10) == 0 ? 19 : (toneTab[num] % 10) * 2 - 1), ((toneTab[num] % 10) == 0 ? toneTab[num] / 10 - 1 : toneTab[num] / 10));
    lcd1.print(F("*"));
}
//
void showSinglegraph1(int num, int duration)
{
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("Duration:"));
    lcd1.print(duration);
    lcd1.print(F("ms"));
    for (int i = 0; i < 7; i++)
    {
        lcd1.setCursor(2 * i + 2, 1);
        lcd1.print(i + 1);
    }
    lcd1.setCursor(16, 1);
    lcd1.print(F("i"));
    lcd1.setCursor(2, 2);
    lcd1.print(F("C4  E4  G4  B4"));
    lcd1.setCursor(4, 3);
    lcd1.print(F("D4  F4  A4  C5"));
    lcd1.setCursor((num - 3) * 2, ((num - 3) % 2 == 0 ? 2 : 3));
    lcd1.print(F("**"));
}

void func1(void)
{
    unsigned long endTime;
    int songPointer;
    int tempFrequency;
    int duration;
    uint8_t tempNum;
    uint8_t lastTempNum;
    uint8_t checkRight;
    double tempFuckingResult;
    uint8_t buttonState;
    int selectFlag;
    selectFlag = 0;
    unsigned long func1StartTime = millis();
    unsigned long toneCnt = 0;
    //这前面是选择界面和按键判断阶段;

    //select 乐器
    lcd1.clear();
    lcd1.noBlink();
    lcd1.setCursor(1, 0);
    lcd1.print(F("Select Instrument"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("F1:Ukulele"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("F2:Piano"));
    while (1)
    {
        buttonState = button1.buttonCheck();
        if (bitRead(buttonState, 1))
        {
            selectFlag = 1;
            break;
        }
        if (bitRead(buttonState, 2))
        {
            selectFlag = 2;
            break;
        }
        if (bitRead(buttonState, 3))
        {
            return;
        }
    }
    //
    lcd1.clear();
    lcd1.setCursor(1, 0);
    lcd1.print(F("Select mode"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("1.radom song"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("2.personality"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("select f1 or f2"));
    while (1)
    {
        buttonState = button1.buttonCheck();
        switch (buttonState)
        {
        case 2:
            randomSong(1);
            goto next;
        case 4:
            goto loadsong;
        case 8:
            return;
        default:
            break;
        }
    }

loadsong:
    if (exploreFile00())
    {
        return;
    }


next:
    songPointer = 0;
    //练习开始时间
    func1StartTime = millis();

    while (1)
    {
        tempNum = pubBuff[songPointer * 2 + 1];
        if (tempNum == 0)
        {
            break;
        }
        lastTempNum = tempNum;
        tempFrequency = frequencyTab[tempNum];
        tempNum = pubBuff[songPointer * 2 + 2];
        tempNum = tempNum >> 4;
        tempNum = 0b00010000 - tempNum;
        duration = tempNum * 62.5;
        endTime = millis() + duration;
        tone(speakerPin, tempFrequency, duration / 5);
        //2004界面U
        if (selectFlag == 1)
            showSingleGraph(lastTempNum);
        if (selectFlag == 2)
            showSinglegraph1(lastTempNum, duration);
        //
        u8g2.firstPage();
        do
        {
            showtone(lastTempNum);
            showBigTonea(tempNum, 80, 48);
        } while (u8g2.nextPage());
        if (checkRight)
        {
            delay(200);
            checkRight = 0;
        }

        while (millis() < (endTime - long(0.75 * duration)))
        {
            //按键响应
            buttonState = button1.buttonCheck();
            if (buttonState == 0b00001000)
            {
                goto func1end;
            }
            if (buttonState == 0b00000001)
            {
                long tempTime = millis();
                {
                    lcd1.clear();
                    lcd1.setCursor(0, 0);
                    lcd1.print(F("press ok to continue"));
                    lcd1.setCursor(8, 2);
                    lcd1.print(F("stop"));

                    while (button1.buttonCheck() != 0b00000001)
                    {
                        //empty
                    }
                }
                //钢琴界面
                if (selectFlag == 1)
                    showSingleGraph(lastTempNum);
                if (selectFlag == 2)
                    showSinglegraph1(lastTempNum, duration);
                //
                endTime += tempTime - millis();
            }
            //end
            led1.shownum(((endTime - millis()) / 100) % 10);
        }

        while (millis() < (endTime))
        {
            led1.shownum(((endTime - millis()) / 100) % 10);

            tempFuckingResult = fuckingCompute();

            if (abs((tempFuckingResult - double(tempFrequency))) < 10.0)
            {
                checkRight = 1;
            }
            //按键响应
            buttonState = button1.buttonCheck();
            if (buttonState == 0b00001000)
            {
                goto func1end;
            }
            if (buttonState == 0b00000001)
            {
                long tempTime = millis();
                {
                    lcd1.clear();
                    lcd1.setCursor(0, 0);
                    lcd1.print(F("press ok to continue"));
                    lcd1.setCursor(8, 2);
                    lcd1.print(F("stop"));
                    while (button1.buttonCheck() != 0b00000001)
                    {
                        //empty
                    }
                }
                if (selectFlag == 1)
                    showSingleGraph(lastTempNum);
                if (selectFlag == 2)
                    showSinglegraph1(lastTempNum, duration);
                endTime += tempTime - millis();
            }
            //end
        }

        if (checkRight)
        {
            songPointer++;
            toneCnt++;
        }
        else
        {
            //显示弹错了
            lcd1.clear();
            lcd1.setCursor(2, 0);
            lcd1.print(F("wrong    tone!!"));
            lcd1.setCursor(7, 2);
            lcd1.print(F("*****"));

            //警告音
            tone(speakerPin, 4096, 250);
            for (int i = 0; i < 1000; i += 20)
            {
                led1.ledcycle(7);
                //按键响应
                buttonState = button1.buttonCheck();
                if (buttonState == 0b00001000)
                {
                    goto func1end;
                }
                if (buttonState == 0b00000001)
                {
                    long tempTime = millis();
                    {
                        lcd1.clear();
                        lcd1.setCursor(0, 0);
                        lcd1.print(F("press ok to continue"));
                        lcd1.setCursor(8, 2);
                        lcd1.print(F("stop"));
                        while (button1.buttonCheck() != 0b00000001)
                        {
                            //empty
                        }
                    }
                    if (selectFlag == 1)
                        showSingleGraph(lastTempNum);
                    if (selectFlag == 2)
                        showSinglegraph1(lastTempNum, duration);
                    endTime += tempTime - millis();
                }
                //end
            }
        }
    }
    //正常结束退出

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("congratulations for "));
    lcd1.setCursor(0, 1);
    lcd1.print(F("finishing!We'll save"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("your practice record"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("press ok to continue"));
    //显示祝贺图片
    showCongratulation();
    //按键响应
    while (1)
    {
        buttonState = button1.buttonCheck();
        if (buttonState == 0b00000001)
        {
            break;
        }
    }

    //退出部分
func1end:
    lcd1.clear();
    //统计信息的更新及屏幕显示
    unsigned int tempuint = 0;
    unsigned long tempulong = 0;
    long startAddr = EEPROM.length() - 100;
    //单音练习的次数
    EEPROM.get(startAddr + 4, tempuint);
    tempuint++;
    lcd1.setCursor(0, 0);
    lcd1.print(F("open count"));
    lcd1.setCursor(12, 0);
    lcd1.print(tempuint);
    EEPROM.put(startAddr + 4, tempuint);
    //单音练习毫秒数
    EEPROM.get(startAddr + 6, tempulong);
    tempulong += millis() - func1StartTime;
    lcd1.setCursor(0, 1);
    lcd1.print(F("excise min"));
    lcd1.setCursor(12, 1);
    lcd1.print(tempulong / 60000);
    EEPROM.put(startAddr + 6, tempulong);
    //单音练习的音节数
    EEPROM.get(startAddr + 10, tempulong);
    tempulong += toneCnt;
    lcd1.setCursor(0, 2);
    lcd1.print(F("tone count"));
    lcd1.setCursor(12, 2);
    lcd1.print(tempulong);
    EEPROM.put(startAddr + 10, tempulong);

    lcd1.setCursor(0, 3);
    lcd1.print(F("press ok to continue"));
    //按键响应
    while (1)
    {
        buttonState = button1.buttonCheck();
        if (buttonState == 0b00000001)
        {
            break;
        }
    }
    return;
}

#endif