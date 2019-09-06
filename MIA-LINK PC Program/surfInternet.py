"""
//func1
//copyright @ MIA project
//version 2.0
"""
##original
import serial
import pyautogui
import time

ser=serial.Serial("com13",9600)

data=''

while True:
    count = ser.inWaiting()
    if count > 0:
        data = ser.read(1)
        if (data==b'a'):
            print(data)
            ser.write(data)
            time.sleep(0.5)
            ser.write(b'original')
            continue
        if (data==b'1'):
            pyautogui.typewrite('1')
            print(data)
        if (data==b'2'):
            pyautogui.typewrite('2')
            print(data)
        if (data==b'3'):
            pyautogui.typewrite('3')
            print(data)
        if (data==b'4'):
            pyautogui.typewrite('4')
            print(data)
        if (data==b'5'):
            pyautogui.typewrite('5')
            print(data)
        if (data==b'6'):
            pyautogui.typewrite('f')
            print(data)
        if (data==b'7'):
            pyautogui.typewrite('k')
            print(data)
        if (data==b'8'):
            pyautogui.typewrite('j')
            print(data)
        if(data==b'b'):
            continue
        if (data==b'z'):
            break

        ser.write(data)
        
print ("end")

