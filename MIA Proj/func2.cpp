//func2.cpp
//功能2
//copyright @ MIA project
//version 3.0

#include "showu8g2.h"
#include "miaTool.h"
#include "LiquidCrystal.h"
#include "button.h"
#include "eepromMgr.h"
#include "led.h"

extern led led1;
extern LiquidCrystal lcd1;
extern button button1;
extern Servo servo1;
extern U8G2_ST7920_128X64_1_8080 u8g2;

char rhythmTab[6][21] = {{"                    "}, {"1.Syncopated        "}, {"2.8 AND 16          "}, {"3.Dotted            "}, {"4.Triplets          "}, {"                    "}};

const unsigned int LastTimeTab[2][4] = {500, 500, 500, 500, 667, 333, 667, 333};

void Ukraini();
void Tradition();
void Metronome();
void FileAddition();
void tradiFunc(uint8_t mytype);

void func2()
{
func2begin:
    //led1.shownum(2);
    //按钮状态
    uint8_t tempStatus;
    //选择菜单
    uint8_t countLine;
    countLine = 1;
    showMenuCover(3);
    lcd1.clear();
    lcd1.noBlink();
    lcd1.setCursor(7, 0);
    lcd1.print(F("Rhythm"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("Ukulele"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("Tradition"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("FileAddition"));
    lcd1.setCursor(18, 1);
    lcd1.blink();
    while (1)
    {
        lcd1.setCursor(18, countLine);
        tempStatus = button1.buttonCheck();
        //确认键
        if (bitRead(tempStatus, 0))
        {
            switch (countLine)
            {
            case 1:
                Ukraini();
                led1.ledwave(6);
                goto func2begin;
            case 2:
                lcd1.clear();
                lcd1.noBlink();
                lcd1.setCursor(5, 0);
                lcd1.print(F("Tradition"));
                lcd1.setCursor(0, 1);
                lcd1.print(F("F1:Tradition Rhythm"));
                lcd1.setCursor(0, 2);
                lcd1.print(F("F2:Metronome"));
                while (1)
                {
                    tempStatus = button1.buttonCheck();
                    if (bitRead(tempStatus, 1))
                    {
                        Tradition();
                        led1.ledwave(6);

                        break;
                    }
                    if (bitRead(tempStatus, 2))
                    {
                        Metronome();
                        led1.ledwave(6);
                        break;
                    }
                    if (bitRead(tempStatus, 3))
                    {
                        led1.ledwave(6);
                        break;
                    }
                }
                goto func2begin;
            case 3:
                FileAddition();
                led1.ledwave(6);
                goto func2begin;
            }
        }
        if (bitRead(tempStatus, 1) && countLine < 3)
            countLine++;
        if (bitRead(tempStatus, 2) && countLine > 1)
            countLine--;
        if (bitRead(tempStatus, 3))
            break;
    }
}

//尤克里里专属节奏练习
void Ukraini()
{
    //按钮状态
    uint8_t tempStatus;
    uint8_t choose;
    uint8_t cnt = 0;
    uint8_t cnt1 = 0;
    unsigned long tempTime1;
    double tempTime2;
    unsigned int angle;
    angle = 72;
    lcd1.noBlink();
    lcd1.clear();
    lcd1.setCursor(6, 0);
    lcd1.print(F("Ukulele"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("F1:Normal"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("F2:Shuffle"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("F3:Back"));
    while (1)
    {
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 1))
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("0.5s"));
            lcd1.setCursor(5, 1);
            lcd1.print(F("0.5s"));
            lcd1.setCursor(10, 2);
            lcd1.print(F("0.5s"));
            lcd1.setCursor(15, 3);
            lcd1.print(F("0.5s"));
            choose = 0;
            break;
        }
        if (bitRead(tempStatus, 2))
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("0.67s"));
            lcd1.setCursor(5, 1);
            lcd1.print(F("0.33s"));
            lcd1.setCursor(10, 2);
            lcd1.print(F("0.67s"));
            lcd1.setCursor(15, 3);
            lcd1.print(F("0.33s"));
            choose = 1;
            break;
        }
        if (bitRead(tempStatus, 3))
        {
            goto Ukrainiend;
        }
    }

    tempTime1 = millis();
    tempTime2 = millis();
    servo1.write(90 - angle / 2);
    delay(300);
    lcd1.blink();
    while (1)
    {
        if (millis() > tempTime1)
        {

            led1.ledwave((cnt + 2) % 2 == 0 ? 1 : 0);

            lcd1.setCursor(5 * cnt + 4, cnt);

            u8g2.firstPage();
            do
            {
                showBArrow(((cnt + 2) % 2 == 0 ? 0 : 1), 32 * cnt);

            } while (u8g2.nextPage());

            tone(speakerPin, ((cnt + 2) % 2 == 0 ? 262 : 523), LastTimeTab[choose][cnt]);
            tempTime1 += LastTimeTab[choose][cnt];
            cnt = (cnt + 1) % 4;
        }
    fuck07:
        if (millis() > tempTime2)
        {

            servo1.write(cnt1 < 24 ? (90 - angle / 2 + cnt1 * angle / 24) : (90 + angle * 3 / 2 - cnt1 * angle / 24));
            cnt1 = (cnt1 + 1) % 48;
            tempTime2 += (LastTimeTab[choose][cnt1 / 24] / 24.0);
            goto fuck07;
        }

        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 3))
        {
            goto Ukrainiend;
        }
    }

Ukrainiend:
    servo1.write(90);
}

//传统节奏练习菜单界面
void Tradition()
{
    //按钮状态
    uint8_t tempStatus;
    uint8_t markCount;
    uint8_t lastMarkCount;
    markCount = 0;
    lastMarkCount = markCount;
    lcd1.noBlink();
    lcd1.clear();
    lcd1.setCursor(2, 0);
    lcd1.print(F("Tradition Rhythm"));
    for (int i = 0; i < 3; i++)
    {
        lcd1.setCursor(0, i + 1);
        lcd1.print(rhythmTab[markCount + i]);
    }
    while (1)
    {
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 0))
            break;
        if (bitRead(tempStatus, 1) && markCount < 3)
            markCount++;
        if (bitRead(tempStatus, 2) && markCount > 0)
            markCount--;
        if (bitRead(tempStatus, 3))
            goto TraditionEnd;
        lcd1.setCursor(18, 2);
        lcd1.blink();
        if (markCount != lastMarkCount)
        {
            for (int i = 0; i < 3; i++)
            {
                lcd1.noBlink();
                lcd1.setCursor(0, i + 1);
                lcd1.print((rhythmTab[markCount + i]));
            }
            lastMarkCount = markCount;
        }
    }
    lcd1.noBlink();

    tradiFunc(markCount);
    servo1.write(90);

TraditionEnd:
    return;
}

//节拍器
void Metronome()
{
    //变量定义
    unsigned int cnt;
    unsigned int cnt1;
    unsigned long endTime;
    unsigned long tempTime;
    unsigned long tempTime1;
    unsigned long startTime;
    //每分钟节拍数
    unsigned int N;
    uint8_t count[3] = {0, 6, 0};
    //0表示退出,1为百位，2为十位，3为个位，4表示进入
    unsigned int count1;
    //角度
    unsigned int angle;
    //按钮
    uint8_t tempStatus;

//屏幕显示
MetronomeBegin:
    count1 = 1;
    cnt = 0;
    cnt1 = 0;
    count[0] = 0;
    count[1] = 6;
    count[2] = 0;
    lcd1.clear();
    lcd1.noBlink();
    lcd1.setCursor(5, 0);
    lcd1.print(F("Metronome"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("Please enter Number"));
    lcd1.setCursor(8, 2);
    for (uint8_t i = 0; i < 3; i++)
    {
        lcd1.print(count[i]);
    }

    lcd1.blink();
    showMenuCover(3);
    while (1)
    {
        lcd1.setCursor(count1 + 7, 2);
        lcd1.print(count[count1 - 1]);
        lcd1.setCursor(count1 + 7, 2);
        if (count1 == 0)
            break;
        //进入节拍器
        if (count1 == 4)
        {
            N = count[0] * 100 + count[1] * 10 + count[2];
            if (N > 233 || N < 30)
            {
                lcd1.clear();
                lcd1.noBlink();
                lcd1.setCursor(4, 0);
                lcd1.print(F("OUT of RANGE"));
                lcd1.setCursor(0, 1);
                lcd1.print(F("The correct range is"));
                lcd1.setCursor(3, 2);
                lcd1.print(F("30bpm ~ 233bpm"));
                delay(1000);
                goto MetronomeBegin;
            }
            angle = 4000 / N + 16;
            endTime = 60000 / N;

            u8g2.firstPage();
            do
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(10, 20, "Metronome");
                u8g2.setFont(u8g2_font_ncenB12_tr);
                u8g2.setCursor(35, 50);
                u8g2.print(N);
                u8g2.drawStr(68, 50, "bpm");
            } while (u8g2.nextPage());
            tempTime = millis();
            startTime = millis();
            tempTime1 = millis();

            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("Count:"));
            lcd1.setCursor(8, 0);
            lcd1.print(cnt1);
            lcd1.setCursor(0, 1);
            lcd1.print(F("Cumulative Time (s):"));
            lcd1.setCursor(0, 3);
            lcd1.print(double(60.0)/N);
            lcd1.print(F(" secs/ beat"));
            lcd1.noBlink();
            led1.ledwave(1);

            while (1)
            {
                if (cnt == 12 || cnt == 36)
                    tone(speakerPin, 392, 15);
                if (cnt == 0)
                    tone(speakerPin, 196, 30);
                if (cnt == 24)
                    tone(speakerPin, 698, 30);

            fuck06:
                if (millis() > tempTime)
                {
                    if (cnt < 24)
                        servo1.write(90 - angle / 2 + cnt * angle / 24);
                    if (cnt > 23 && cnt < 48)
                        servo1.write(90 + angle * 3 / 2 - cnt * angle / 24);
                    cnt = (cnt + 1) % 48;
                    tempTime += endTime / 24;
                    goto fuck06;
                }
                if (millis() > tempTime1)
                {
                    lcd1.setCursor(8, 0);
                    lcd1.print(cnt1);
                    led1.ledwave(2);
                    cnt1++;
                    tempTime1 += endTime;
                }
                lcd1.setCursor(0, 2);
                lcd1.print((millis() - startTime) / 1000);
                tempStatus = button1.buttonCheck();
                if (bitRead(tempStatus, 3))
                    return;
            }
        }
        //////////////////////////节拍器结束
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 0))
        {
            count1++;
        }
        if (bitRead(tempStatus, 1))
        {
            count[count1 - 1]++;
            if (count[count1 - 1] == 10)
            {
                count[count1 - 1] = 0;
            }
        }
        if (bitRead(tempStatus, 2))
        {
            if (count[count1 - 1] == 0)
            {
                count[count1 - 1] = 9;
            }
            else
            {
                count[count1 - 1]--;
            }
        }

        if (bitRead(tempStatus, 3))
            count1--;
    }
    servo1.write(90);
}

//文件读取歌曲练习
void FileAddition()
{
    //变量定义
    unsigned long startTime;
    unsigned long tempTime1;
    unsigned long tempTime2;
    const unsigned int angle = 72;
    uint8_t cnt = 0;
    uint8_t cnt1 = 0;
    //计数
    unsigned int rhythmPointer = 0;
    //取pubBuff中的值
    uint8_t rhythmNum;
    //持续时间
    unsigned long rhythmDuration;
    //音调
    unsigned int rhythmFrequency;
    //按钮状态
    uint8_t tempStatus;

    //pubBuff赋值
    if (exploreFile00())
    {
        return;
    }

    lcd1.noBlink();

    tempTime1 = millis();
    tempTime2 = millis();
    servo1.write(90 - angle / 2);
    delay(300);
    tempTime1 = millis();
    tempTime2 = millis();
    lcd1.clear();
    while (1)
    {
        //更新rhythmPointer、rhythmNum、rhythmduration
        rhythmNum = pubBuff[rhythmPointer * 2 + 1];
        if (rhythmNum == 0)
        {
            break;
        }
        rhythmFrequency = frequencyTab[rhythmNum];
        tone(speakerPin, rhythmFrequency, rhythmDuration);
        rhythmNum = pubBuff[rhythmPointer * 2 + 2];
        rhythmNum = rhythmNum >> 4;
        rhythmNum = 0b00010000 - rhythmNum;
        rhythmDuration = rhythmNum * 62.5;
        if (millis() > tempTime1)
        {
            lcd1.noBlink();
            lcd1.setCursor(0, 0);
            lcd1.print(F("Fist Line:"));
            lcd1.print(rhythmPointer % 6 + 1);
            lcd1.setCursor(0, 1);
            lcd1.print(F("Duration:"));
            lcd1.print(rhythmDuration);
            lcd1.print(F("ms  "));
            if (rhythmPointer % 6 == 0)
            {
                for (int i = 0; i < 12; i++)
                {
                    lcd1.setCursor((i % 6) * 3 + 2, (i < 6 ? 2 : 3));
                    rhythmNum = pubBuff[(rhythmPointer + i) * 2 + 1];
                    if (rhythmNum == 0)
                    {
                        do{
                            lcd1.print(F(" "));
                            i++;
                            lcd1.setCursor((i % 6) * 3 + 2, (i < 6 ? 2 : 3));
                        }while(i<12);
                        break;
                    }
                    if (rhythmNum != 11)
                    {
                        lcd1.print(char(('1' + rhythmNum - 4)));
                    }
                    else
                    {
                        lcd1.print(F("i"));
                    }
                }
                //屏幕显示
                u8g2.firstPage();
                do
                {
                    for (int i = 0; i < 12; i++)
                    {
                        rhythmNum = pubBuff[(rhythmPointer + i) * 2 + 1];
                        if (rhythmNum == 0)
                        {
                            break;
                        }
                        rhythmNum = pubBuff[(rhythmPointer + i) * 2 + 2];
                        rhythmNum = rhythmNum >> 4;
                        rhythmNum = 0b00010000 - rhythmNum;
                        //128*64
                        showSpecificTone(rhythmNum, (i < 6 ? (14 + 20 * i) : (14 + 20 * (i - 6))), (i < 6 ? (16) : (48)));
                    }
                } while (u8g2.nextPage());
            }

            rhythmPointer++;
            cnt++;
            tempTime1 += rhythmDuration;
        }
    fuck05:
        if (millis() > tempTime2)
        {

            servo1.write(cnt1 < 24 ? (90 - angle / 2 + cnt1 * angle / 24) : (90 + angle * 3 / 2 - cnt1 * angle / 24));
            cnt1 = (cnt1 + 1) % 48;
            tempTime2 += (rhythmDuration / 24);
            goto fuck05;
        }
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 3))
            goto FileAdditionend;
    }

FileAdditionend:
    servo1.write(90);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

const uint16_t func2timeTab[4][6] = {
    {250, 500, 250, 250, 500, 250},
    {500, 250, 250, 500, 250, 250},
    {750, 250, 750, 250, 750, 250},
    {333, 333, 333, 333, 333, 333}};

//传统节奏辅助函数
void func2show2004(uint8_t mytype, uint8_t i)
{
    lcd1.setCursor(5, 1);
    if (func2timeTab[mytype][i] == 500)
    {
        lcd1.print(F("    quaver    "));
    }
    if (func2timeTab[mytype][i] == 250)
    {
        lcd1.print(F("semiquaver    "));
    }
    if (func2timeTab[mytype][i] == 333)
    {
        lcd1.print(F("triplet quaver"));
    }
    if (func2timeTab[mytype][i] == 750)
    {
        lcd1.print(F("dotted quaver "));
    }
    lcd1.setCursor(10, 2);
    lcd1.print(func2timeTab[mytype][i]);
    lcd1.print(F("ms  "));
}

//传统节奏辅助函数
void func2show12864(uint8_t mytype)
{
    u8g2.firstPage();
    do
    {
        switch (mytype)
        {
        case 0:
            showBigTone(0, 19, 48);
            showBigTone(1, 59, 48);
            showBigTone(0, 99, 48);
            break;
        case 1:
            showBigTone(1, 19, 48);
            showBigTone(0, 59, 48);
            showBigTone(0, 99, 48);
            break;
        case 2:
            showBigTone(1, 45, 48);
            u8g2.drawDisc(64, 48, 3);
            showBigTone(0, 83, 48);
            break;
        case 3:
            showBigTone(1, 25, 56);
            showBigTone(1, 58, 56);
            showBigTone(1, 91, 56);
            u8g2.setFont(u8g2_font_ncenB14_tr);
            u8g2.drawStr(64, 15, "3");
            u8g2.drawEllipse(59, 20, 28, 10, U8G2_DRAW_UPPER_LEFT);
            u8g2.drawEllipse(78, 20, 28, 10, U8G2_DRAW_UPPER_RIGHT);
            break;

        default:
            break;
        }
    } while (u8g2.nextPage());
}

//传统节奏辅助函数
uint8_t willStop(uint8_t mytype, uint8_t i)
{
    static uint8_t oldi = 255;
    if (i == oldi)
    {
        return 0;
    }
    oldi = i;
    if (mytype != 2)
    {
        if (i == 0 || i == 3)
        {
            return 1;
        }
    }
    else
    {
        if (i % 2 != 1)
        {
            return 1;
        }
    }
    return 0;
}

//传统节奏实现
void tradiFunc(uint8_t mytype)
{
    //按钮状态
    uint8_t tempStatus;
    unsigned long tempTime1;
    unsigned long tempTime2;
    unsigned long startTime;
    const unsigned int angle = 50;
    uint8_t practiceMode = 0;
    unsigned int cnt = 0;
    uint8_t cnt1 = 0;
    uint8_t i = 0;

    lcd1.noBlink();
    lcd1.clear();
    lcd1.setCursor(7, 0);
    lcd1.print(F("Rhythm"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("Duration:"));
    //舵机初始化
    servo1.write(90 - angle / 2);

    //12864显示处
    func2show12864(mytype);

    tempTime1 = millis();
    tempTime2 = tempTime1 + 3;

    while (1)
    {

        //每轮卡住处
        //卡住条件判断
        if (willStop(mytype, (cnt / 25) % 6))
        {
            //中间刷屏
            lcd1.setCursor(5, 1);
            lcd1.print(F("              "));
            lcd1.setCursor(10, 2);
            lcd1.print(F("stop! "));
            startTime = millis();
            for (uint8_t k = 0; k < 66; k++)
            {
                //适时归位舵机
                if (k == 30)
                {
                    servo1.write(90 - angle / 2);
                }
                //跑马灯模式
                led1.ledcycle(7);
                //按键响应
                tempStatus = button1.buttonCheck();
                if (bitRead(tempStatus, 3))
                {
                    goto func2tradiEnd;
                }
                if (bitRead(tempStatus, 0))
                {
                    practiceMode = ~practiceMode;
                    if (practiceMode)
                    {
                        noTone(speakerPin);
                        lcd1.setCursor(2, 3);
                        lcd1.print(F("#Practice Mode#"));
                    }
                    else
                    {
                        lcd1.setCursor(2, 3);
                        lcd1.print(F("               "));
                    }
                }
                /////////////////////////对practiceMode 的响应
                tempStatus = button1.buttonPeek();
                if (practiceMode)
                {
                    if ((tempStatus & 0b00000110))
                    {
                        tone(speakerPin, int(((tempStatus & 0b00000110) << 1)) * 262);
                    }
                    else
                    {
                        noTone(speakerPin);
                    }
                }
            }
            //停顿结尾处理
            cnt1 = 0;
            startTime = millis() - startTime;
            tempTime1 += startTime;
            tempTime2 += startTime;
            //强制同步
            if (cnt >= 149)
            {
                tempTime1 = millis();
                cnt = 0;
                cnt1 = 0;
                tempTime2 = tempTime1 + 3;
            }
        }

        //其余部分
        if (millis() > tempTime1)
        {
            ///////////////////////////////////2004显示
            func2show2004(mytype, i);
            //发声处理
            if (~practiceMode)
            {
                tone(speakerPin, ((mytype != 2 ? (i / 3) : 0) + i % 2) % 2 ? 262 : 523, func2timeTab[mytype][i] - 5);
            }
            //以6为循环更新i
            i = (i + 1) % 6;
            ////////////////////////////////其他部分时间计算
            tempTime1 += func2timeTab[mytype][i];
            led1.ledwave((cnt / 25) % 2);
        }

        //退出判断及练手模式判断
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 3))
        {
            break;
        }
        if (bitRead(tempStatus, 0))
        {
            practiceMode = ~practiceMode;
            if (practiceMode)
            {
                noTone(speakerPin);
                lcd1.setCursor(2, 3);
                lcd1.print(F("#Practice Mode#"));
            }
            else
            {
                lcd1.setCursor(2, 3);
                lcd1.print(F("               "));
            }
        }
        //////////////////响应practiceMode
        tempStatus = button1.buttonPeek();
        if (practiceMode)
        {
            if ((tempStatus & 0b00000110))
            {
                tone(speakerPin, int(((tempStatus & 0b00000110) << 1)) * 262);
            }
            else
            {
                noTone(speakerPin);
            }
        }

        //舵机模块
    fuck01:
        if (millis() > tempTime2)
        {
            servo1.write(cnt1 < 25 ? (90 - angle / 2 + cnt1 * angle / 25) : (90 + angle * 3 / 2 - cnt1 * angle / 25));
            cnt1 = (cnt1 + 1) % 50;
            cnt++;
            //////////////////////////////////舵机时间计算
            tempTime2 += (func2timeTab[mytype][cnt / 25] / 25);
            //控制led舞动
            led1.ledwave((cnt / 25) % 2);
            goto fuck01;
        }
    }

func2tradiEnd:
    led1.shownum(2);
    return;
}
