"""
//func3
//copyright @ MIA project
//version 1.0
"""
##game0002
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
            ser.write(b'game0002')
            continue
        if (data==b'1'):
            pyautogui.press('up')
            print(data)
        if (data==b'2'):
            pyautogui.press('down')
            print(data)
        if (data==b'3'):
            pyautogui.press('left')
            print(data)
        if (data==b'4'):
            pyautogui.press('right')
            print(data)
        if (data==b'5'):
            pyautogui.press('r')
            print(data)
        if (data==b'6'):
            pyautogui.click()
            print(data)
        
        if(data==b'b'):
            continue
        if (data==b'z'):
            break

        ser.write(data)
        
print ("end")

