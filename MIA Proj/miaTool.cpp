//miaTool.cpp
//miaTool 的实现
//copyright @ MIA project
//version 4.0
#include "miaTool.h"

extern Servo servo1;
const int frequencyTab[15] = {
    8192, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698};

void exploreShow(int line)
{

    for (uint8_t j = 1; j < 4; j++)
    {
        lcd1.setCursor(0, j);
        lcd1.print(F("                    "));
    }
    for (int i = 1; i < 4; i++)
    {
        if (!eepromMgr1.eepromShow(line + i - 2))
        {
            lcd1.setCursor(0, i);
            lcd1.print(nameBuff);
        }
    }
}

void randomSong(int type)
{
    if (type == 1)
    {
        pubBuff[0] = 0b01010000;
        randomSeed(analogRead(A0));
        for (int i = 0; i < 100; i++)
        {
            int tempNum = random() % 8 + 4;
            //
            pubBuff[i * 2 + 1] = tempNum;
            pubBuff[i * 2 + 2] = 0B00001111;
        }
        pubBuff[201] = 0;
    }
}

bluetooth::bluetooth(int bluetoothPin0) : bluetoothPin(bluetoothPin0)
{
    pinMode(bluetoothPin, INPUT_PULLUP);
}

bluetooth::~bluetooth()
{
}

int bluetooth::BTLinkStatus(void)
{
    return digitalRead(bluetoothPin);
}

bgm::bgm() : bgmPointer(0), tempTime(0), bgmLenth(163), cnt1(0), tempTime2(0), duration(0)
{
}

bgm::~bgm()
{
    noTone(speakerPin);
}

static const byte bgmTab[163] PROGMEM = {
    0x50, 0x05, 0x0f, 0x05, 0x8f, 0x04, 0x8f, 0x04, 0x0f, 0x04, 0x8f, 0x02, 0x8f, 0x05, 0x8f, 0x06, 0x8f, 0x05, 0x8f, 0x06, 0x8f, 0x01, 0x0f, 0x01, 0x0f, 0x02, 0x0f, 0x02, 0x8f, 0x01, 0x8f, 0x02, 0x0f, 0x04, 0x0f, 0x08, 0x8f, 0x09, 0x8f, 0x06, 0x8f, 0x06, 0x8f, 0x08, 0x8f, 0x05, 0x0f, 0x05, 0x0f, 0x06, 0x0f, 0x06, 0x8f, 0x05, 0x8f, 0x06, 0x0f, 0x08, 0x0f, 0x09, 0x8f, 0x09, 0x8f, 0x09, 0x8f, 0x0b, 0x8f, 0x09, 0x0f, 0x08, 0x8f, 0x06, 0x8f, 0x05, 0x0f, 0x05, 0x8f, 0x06, 0x8f, 0x08, 0x0f, 0x01, 0x0f, 0x05, 0x8f, 0x06, 0x8f, 0x05, 0x8f, 0x06, 0x8f, 0x04, 0x0f, 0x04, 0x0f, 0x06, 0x0f, 0x06, 0x8f, 0x05, 0x8f, 0x06, 0x0f, 0x08, 0x0f, 0x09, 0x8f, 0x0a, 0x8f, 0x09, 0x8f, 0x08, 0x8f, 0x09, 0x0f, 0x08, 0x8f, 0x06, 0x8f, 0x05, 0x0f, 0x05, 0x8f, 0x06, 0x8f, 0x08, 0x0f, 0x01, 0x0f, 0x05, 0x8f, 0x06, 0x8f, 0x05, 0x8f, 0x06, 0x8f, 0x04, 0x0f, 0x04, 0x0f, 0x05, 0x8f, 0x05, 0x8f, 0x05, 0x8f, 0x06, 0x8f, 0x08, 0x0f, 0x08, 0x8f, 0x09, 0x8f, 0x0a, 0x0f, 0x09, 0x0f, 0x0a, 0x0f, 0x0a, 0x0f};

void bgm::startBgm(void)
{

    bgmPointer = 0;
    uint8_t buff = 0;
    for (; bgmPointer < bgmLenth; bgmPointer++)
    {
        buff = pgm_read_byte(bgmTab + bgmPointer);
        pubBuff[bgmPointer] = buff;
    }
    tempTime = millis();
    tempTime2 = millis();
    bgmPointer = 1;
    cnt1 = 0;
    servo1.write(90 - 30 / 2);
}

void bgm::nextBgm(void)
{
    unsigned int tempFrequency;

    if (millis() >= tempTime)
    {
        uint8_t tempNum;
        tempFrequency = 0;
        duration = 0;
        if (bgmPointer >= bgmLenth)
        {
            bgmPointer = 1;
        }
        tempNum = pubBuff[bgmPointer];
        tempFrequency = frequencyTab[tempNum];
        bgmPointer++;

        tempNum = pubBuff[bgmPointer];
        tempNum = 0b00010000 - (tempNum >> 4);
        duration = tempNum * 20;
        tone(speakerPin, tempFrequency, duration);
        tempTime += duration * 1.2;
        bgmPointer++;
        if ((bgmPointer / 2) % 2)
        {
            cnt1 = 24;
        }
        else
        {
            cnt1 = 0;
        }
    }
bgm1:
    if (millis() > tempTime2)
    {
        servo1.write(cnt1 < 24 ? (90 - 30 / 2 + cnt1 * 30 / 24) : (90 + 3 * 30 / 2 - cnt1 * 30 / 24));
        cnt1 = (cnt1 + 1) % 48;
        tempTime2 += (duration / 24) * 1.2;
        goto bgm1;
    }
}

void bgm::stopBgm(void)
{
    noTone(speakerPin);
}

int exploreFile00()
{
    int buttonState = 0;
    int line = 1, oldline = 0;
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("embeded exploer     "));
    while (1)
    {
        while (line != oldline)
        {
            exploreShow(line);
            oldline = line;
        }
        buttonState = button1.buttonCheck();
        switch (buttonState)
        {
        case 1:
            if (!eepromMgr1.songLoad(line))
            {
                goto explorerout2;
            }
            goto explorerout1;
        case 2:
            line++;
            break;
        case 4:
            if (line > 1)
            {
                line--;
            }
            break;
        case 8:
            goto explorerout1;
        }
        //
        lcd1.setCursor(18,2);
        lcd1.blink();
    }
explorerout1:
    lcd1.noBlink();
    return 1;
explorerout2:
    lcd1.noBlink();
    return 0;
}

void superUser(void)
{
    uint8_t keyStatus = 0;
    int passwordCnt = 0;
    int isright = 1;
    uint8_t tempResult = 0;

    lcd1.clear();
    lcd1.noBlink();
    lcd1.print(F("super user mode"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("F1: clear memory"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("F2: change passwd"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("## keep power on ##"));
    while (1)
    {
        keyStatus = button1.buttonCheck();
        if (keyStatus == 8)
            break;
        if (keyStatus == 2)
        {
            lcd1.clear();
            lcd1.print(F("confirn to clear?"));
            lcd1.setCursor(0, 1);
            lcd1.print(F("ok to continue"));
            lcd1.setCursor(0, 2);
            lcd1.print(F("F(1-3) to restart"));
            lcd1.setCursor(0, 3);
            lcd1.print(F("## keep power on ##"));
            while (1)
            {
                keyStatus = button1.buttonCheck();
                if (keyStatus == 1)
                {
                    lcd1.clear();
                    lcd1.print(F("clearing ..."));
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("wait for few secs"));

                    lcd1.setCursor(0, 3);
                    lcd1.print(F("## keep power on ##"));
                    eepromMgr1.eepromInit();
                    lcd1.clear();
                    lcd1.print(F("clearing finished"));
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("please restart MIA"));
                    while(1)
                        {}
                }
                if ((keyStatus & 0b00001110) != 0)
                {
                    return;
                }
            }
        }
        if (keyStatus == 4)
        {
        superUser1:
            lcd1.clear();
            lcd1.print(F("input new passwd"));
            lcd1.setCursor(0, 1);
            lcd1.print(F("ok:act F(1-3):exit"));
            lcd1.setCursor(0, 2);
            lcd1.print(F("<10 words:"));
            lcd1.setCursor(0, 3);
            lcd1.print(F("## keep power on ##"));
            lcd1.setCursor(10, 2);
            lcd1.blink();
            while (1)
            {
                tempResult = button1.buttonCheck();
                if (bitRead(tempResult, 1))
                {
                    nameBuff[passwordCnt] = 0;
                    passwordCnt++;
                    lcd1.print(F("0"));
                }
                if (bitRead(tempResult, 2))
                {
                    nameBuff[passwordCnt] = 1;
                    passwordCnt++;
                    lcd1.print(F("1"));
                }
                if (bitRead(tempResult, 3))
                {
                    nameBuff[passwordCnt] = 2;
                    passwordCnt++;
                    lcd1.print(F("2"));
                }
                if (bitRead(tempResult, 0))
                {
                    break;
                }
                if (passwordCnt == 10)
                {
                    lcd1.print(F("error!!!"));
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("less than 10 words"));
                    lcd1.setCursor(0, 2);
                    lcd1.print(F("please reinput"));
                    lcd1.setCursor(0, 3);
                    lcd1.print(F("## keep power on ##"));
                    for (uint8_t i = 0; i < 10; i++)
                    {
                        nameBuff[i] = 16;
                    }
                    delay(2000);
                    goto superUser1;
                }
            }
            lcd1.clear();
            lcd1.print(F("confirn to change?"));
            lcd1.setCursor(0, 1);
            lcd1.print(F("ok to continue"));
            lcd1.setCursor(0, 2);
            lcd1.print(F("F(1-3) to restart"));
            lcd1.setCursor(0, 3);
            lcd1.print(F("## keep power on ##"));
            while (1)
            {
                keyStatus = button1.buttonCheck();
                if (keyStatus == 1)
                {
                    eepromMgr1.changePassword();
                    return;
                }
                if ((keyStatus & 0b00001110) != 0)
                {
                    return;
                }
            }
        }
    }
    return;
}