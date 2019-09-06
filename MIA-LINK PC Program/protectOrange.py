"""
//func2
//copyright @ MIA project
//version 1.0
"""
##game0001
import serial
import pyautogui
import time

ser=serial.Serial("com13",9600)

data=''
sensibility=10

while True:
    count = ser.inWaiting()
    if count > 0:
        data = ser.read(1)
        if (data==b'a'):
            print(data)
            ser.write(data)
            time.sleep(0.5)
            
            ser.write(b'game0001')
            
            continue
        if (data==b'1'):
            pyautogui.moveRel(0,-sensibility)
            print(data)
        if (data==b'2'):
            pyautogui.moveRel(0, sensibility)
            print(data)
        if (data==b'3'):
            pyautogui.moveRel(-sensibility,0)
            print(data)
        if (data==b'4'):
            pyautogui.moveRel(sensibility, 0)
            print(data)
        if (data==b'5'):
            pyautogui.press('r')
            print(data)
        if (data==b'6'):
            pyautogui.click()
            print(data)
        if (data==b'7'):
            sensibility=50
            print(data)
        if (data==b'8'):
            sensibility=10
            print(data)
        
        if(data==b'b'):
            continue
        if (data==b'z'):
            break

        ser.write(data)
        
print ("end")


