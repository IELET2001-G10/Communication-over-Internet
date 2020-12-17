###PUT THE LIBRARIES IN THEIR RESPECTIVE FOLDER

Put every library except SocketIO in C:\Users\YOURUSERNAME\Documents\Arduino\libraries.
SocketIO library goes to C:\Program Files (x86)\Arduino\libraries.

If you run into errors, be sure to check that the compiler can reach all the libraries.

###PIN-CONNECTION GUIDE:
  1. BME680 breakout board
      i.   Connect Vcc to 5V on ESP32 DevKit-C
      ii.  Connect GND to GND on ESP32 DevKit-C
      iii. Connect SCL to pin number 22 on ESP32 DevKit-C
      iv.  Connect SDA to pin number 21 on ESP32 DevKit-C
      
  2. SG90 servomotor
      i. Connect brown wire to GND on ESP32 DevKit-C
      ii. Connect red wire to 5V on ESP32 DevKit-C
      iii. Connect orange wire to pin number 16 on ESP32 DevKit-C
      
 ###WIFI CREDENTIALS
 1. Be sure to provide your WiFi-credentials in line 16 and 17 i main.ino!

###3RD PARTY LIBRARIES
The libraries we have used can be found here:
  - https://github.com/adafruit/Adafruit_BME680
  - https://github.com/adafruit/Adafruit_Sensor
  - https://madhephaestus.github.io/ESP32Servo/files.html
  - https://github.com/timum-viw/socket.io-client
