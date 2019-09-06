//func0.cpp
//12864显示
//copyright @ MIA project
//version 1.0
#include "showu8g2.h"
#define PHOTO_RHYTHM
const unsigned char colRhythm[] U8X8_PROGMEM = {
    #include "resouses.h"
    };
#undef PHOTO_RHYTHM
#define PHOTO_GAME
const unsigned char colGame[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_GAME
#define PHOTO_SINGLETONE
const unsigned char colSingleTone[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_SINGLETONE
#define PHOTO_SYSTEM
const unsigned char colSystem[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_SYSTEM
#define PHOTO_NEWTEC
const unsigned char colNewTec[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_NEWTEC
#define PHOTO_CONGRATULATION
const unsigned char colCongratulation[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_CONGRATULATION
//上箭头
#define PHOTO_UPARROW
const unsigned char col1[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_UPARROW
//下箭头
#define PHOTO_DOWNARROW
const unsigned char col2[] U8X8_PROGMEM = {
#include "resouses.h"
};
#undef PHOTO_DOWNARROW

void showEnemy(unsigned int n, uint8_t x, uint8_t y)
{
    if (n >= 1)
    {
        u8g2.drawDisc(x, y, 4);
        u8g2.drawLine(x, y + 2.5, x, y + 16);
        u8g2.drawLine(x, y + 16, x - 10, y + 23);
        u8g2.drawLine(x, y + 16, x + 10, y + 23);
        u8g2.drawLine(x, y + 7, x + 10, y + 4);
        u8g2.drawLine(x, y + 10, x + 10, y + 13);
    }
    if (n == 3)
    {
        u8g2.drawBox(x + 10, y, 20, 17);
    }
    if (n == 2)
    {
        u8g2.drawFrame(x + 10, y, 20, 17);
    }
}

void showTarget(uint8_t x, uint8_t y)
{

    u8g2.drawDisc(x, y, 1);
    u8g2.drawCircle(x, y, 10);
    u8g2.drawLine(x - 3, y, x - 12, y);
    u8g2.drawLine(x + 3, y, x + 12, y);
    u8g2.drawLine(x, y - 3, x, y - 12);
    u8g2.drawLine(x, y + 3, x, y + 12);
}

void showCongratulation()
{
    u8g2.firstPage();
    do
    {
        u8g2.drawXBMP(0, 0, 128, 64, colCongratulation);
    } while (u8g2.nextPage());
}

void showtone(int Num)
{
    switch (Num)
    {
    case 4:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "1");

        break;
    case 5:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "2");

        break;
    case 6:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "3");

        break;
    case 7:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "4");

        break;
    case 8:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "5");

        break;
    case 9:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "6");

        break;
    case 10:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "7");

        break;
    case 11:

        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 52, "1");
        u8g2.drawDisc(37, 22, 2);

        break;
    default:

        u8g2.setFont(u8g2_font_ncenB18_tr);
        u8g2.drawStr(10, 45, "WRONG");

        break;
    }
}

void showMenuCover(int Num)
{
    u8g2.firstPage();
    do
    {
        switch (Num)
        {
        case 1:
            u8g2.drawXBMP(0, 0, 128, 64, colGame);
            break;
        case 2:
            u8g2.drawXBMP(0, 0, 128, 64, colSingleTone);
            break;
        case 3:
            u8g2.drawXBMP(0, 0, 128, 64, colRhythm);
            break;
        case 4:
            u8g2.drawXBMP(0, 0, 128, 64, colNewTec);
            break;
        case 5:
            u8g2.drawXBMP(0, 0, 128, 64, colSystem);
            break;
        }
    } while (u8g2.nextPage());
}

void showBArrow(uint8_t direction, int column)
{
    if (direction == 0)
    {

        u8g2.drawXBMP(column, 0, 32, 64, col2);
    }

    if (direction == 1)
    {

        u8g2.drawXBMP(column, 0, 32, 64, col1);
    }
}

void showSpecificTone(uint8_t result, uint8_t x, uint8_t y)
{
    for (int i = 0; i < 5; i++)
    {
        if (bitRead(result, i))
        {
            switch (i)
            {
            case 0:
                u8g2.drawLine(x + 3 + 5 , y - 12 + 3 + 4, x + 3, y - 12 + 4);
                u8g2.drawLine(x + 3 + 5 +1, y - 12 + 3 + 4, x + 3+1, y - 12 + 4);//
                
            case 1:
                u8g2.drawLine(x + 3, y - 12, x + 3 + 5, y - 12 + 3);
                u8g2.drawLine(x + 4, y - 12, x + 3 + 5+1, y - 12 + 3);//
            case 2:
                u8g2.drawFilledEllipse(x, y, 3, 2);
            case 3:
                u8g2.drawLine(x + 3, y, x + 3, y - 12);
                u8g2.drawLine(x + 4, y, x + 4, y - 12);//
            case 4:
                u8g2.drawEllipse(x, y, 3, 2);
                break;
            default:
                break;
            }
        }
    }
}

void showBigTonea(uint8_t Num, uint8_t x, uint8_t y)
{
    for (int i = 0; i < 5; i++)
    {
        if (bitRead(Num, i))
        {
            switch (i)
            {
            case 0:
                u8g2.drawLine(x + 6 + 10 , y - 32 + 6 + 8, x + 6, y - 32 + 8);
                u8g2.drawLine(x + 6 + 10 +1, y - 32 + 6 + 8, x + 6+1, y - 32 + 8);//
                
            case 1:
                u8g2.drawLine(x + 6, y - 32, x + 6 + 10, y - 32 + 6);
                u8g2.drawLine(x + 7, y - 32, x + 6 + 10+1, y - 32 + 6);//
            case 2:
                u8g2.drawFilledEllipse(x, y, 6, 4);
            case 3:
                u8g2.drawLine(x + 6, y, x + 6, y - 32);
                u8g2.drawLine(x + 7, y, x + 7, y - 32);//
            case 4:
                u8g2.drawEllipse(x, y, 6, 4);
                break;
            default:
                break;
            }
        }
    }
}
void showBigTone(int Num, uint8_t x, uint8_t y)
{

    switch (Num)
    {
    case 0:
                u8g2.drawLine(x + 6 + 10 , y - 32 + 6 + 8, x + 6, y - 32 + 8);
                u8g2.drawLine(x + 6 + 10 +1, y - 32 + 6 + 8, x + 6+1, y - 32 + 8);//
                
            case 1:
                u8g2.drawLine(x + 6, y - 32, x + 6 + 10, y - 32 + 6);
                u8g2.drawLine(x + 7, y - 32, x + 6 + 10+1, y - 32 + 6);//
            case 2:
                u8g2.drawFilledEllipse(x, y, 6, 4);
            case 3:
                u8g2.drawLine(x + 6, y, x + 6, y - 32);
                u8g2.drawLine(x + 7, y, x + 7, y - 32);//
            case 4:
                u8g2.drawEllipse(x, y, 6, 4);
                break;
            default:
                break;
    }
}
