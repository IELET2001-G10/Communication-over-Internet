#include <analogWrite.h>                                          //Include the different libraries needed for the project
#include <Wire.h>                                                 //SDA and SCL pins are declared in Wire.h library (SDA: 21, SCL: 22)
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

WiFiMulti WiFiMulti;                                              //Declare instances of the libraries that was included
SocketIoClient webSocket;
Adafruit_BME680 bme;
Servo servo;

const char* SSID = "";                                            //Pointers to make all necessary configurations visible
const char* PASS = "";                                            //Provide your AP name and password, and provide your server IP-adress as well as port number
const char* IP = "192.168.1.158";
const int PORT = 2520;

/**
 * Function that is run when the ESP32 receives clientConnected identifier on webSocket.
 * Takes the incoming char array, formats it to string with printf command and prints
 * in serial monitor.
 * @param payload The received message containing the client's ID and IP sent from server
 * @param length The size of the received message
*/
void event(const char * payload, size_t length) {                 //Default event, print the received data in serial monitor
  Serial.printf("Got message: %s from server\n", payload);
}

/**
 * Function that is run when the ESP32 receives LEDStateChange identifier on webSocket. 
 * Takes the incoming char array, formats it to string and prints to serial monitor.
 * Also converts the char array data to a string, converts it to an integer and
 * prints it in the serial monitor. Use the LEDStateData value to change LEDState.
 * @param LEDStateData The received message from the server containing the LEDState
 * @param length The size of the received message
 */
void changeLEDState(const char * LEDStateData, size_t length) {   //What happens when the ESP32 receives an instruction from the server (with variable) to change the LED
  Serial.printf("LED State: %s\n", LEDStateData);                 //First we print the data formated with the "printf" command
  Serial.println(LEDStateData);                                   //Then we just print the LEDStateData which will be a int (0 og 1 so in reality bool) that tells us what to do with the LED

  //Data conversion //We need som data processing to make this work
  String dataString(LEDStateData);                                //First we convert the const char array(*) to a string in Arduino (this makes things easier)
  int LEDState = dataString.toInt();                              //When we have a string we can use the built in Arduino function to convert to an integer

  Serial.print("This is the LED state in INT: ");                 //We print the final state
  Serial.println(LEDState);
  digitalWrite(18, LEDState);                                     //We now use the varible to change the light (1 is on, 0 is off)
}

/**
 * Function that is run when the ESP32 receives windowStateChange identifier on webSocket.
 * Takes the incomng char array, converts it to int for use as bool variable.
 * @param windowStateData The received message from the server containing windowState
 * @param length The size of the received message
 */
void changeWindowState(const char * windowStateData, size_t length) {
  String dataString(windowStateData);                             //Converts the char array to int
  int windowState = dataString.toInt();

  if(windowState >= 0 && windowState <= 180) {
    servo.write(windowState);                                     //Sets the servo to a degree of 0-180 (int can only be 0-180)
  }
}

/**
 * Function that is run when the ESP32 receives dataRequest identifier on webSocket.
 * Takes the incoming char array, formats it to string and prints to serial monitor.
 * Also converts the char array to an int in order to check state of the data request.
 * If requestState == 0, read sensor values and sends them to server using webSocket.
 * @param DataRequestData The received message from the server containing the requestState
 * @param length The size of the received message
 */
void dataRequest(const char * dataRequestData, size_t length) {   //This is the function that is called everytime the server asks for data from the ESP32
  Serial.printf("Datarequest: %s\n", dataRequestData);

  String dataString(dataRequestData);
  int requestState = dataString.toInt();

  if(requestState == 0) {                                         //If DataRequestData gives the value 0, read and send sensor data
    bme.performReading();

    char temperature[33];                                         //Declare char arrays for sensor data to send to server (needs to be char array to send to server)
    char pressure[33];                                            //Char array big enogh to store the resulting string after formatting
    char humidity[33];                                            //Size conveniently set to 
    char voc[33];

    sprintf(temperature, "%4.2f", bme.temperature);               //Use a special formatting function to get the char array from floats to char array that is null terminated
    sprintf(pressure, "%6.2f", bme.pressure / 100.0);             //For each sensor value define how wide the array should at least be (digits and precision and the char array should consist of (maximum 6 digits for %f))
    sprintf(humidity, "%4.2f", bme.humidity);
    sprintf(voc, "%4.2f", bme.gas_resistance / 1000.0);
  
    webSocket.emit("nodeTemperature", temperature);               //Send the values to the server that listens for the identifier
    webSocket.emit("nodePressure", pressure);                     //The values is printed in serial monitor by webSocket, so one can easily verify the outgoing values
    webSocket.emit("nodeHumidity", humidity);                     //These sensor values is sent every timeInterval as specified on the server.js file
    webSocket.emit("nodeVOC", voc);
  }
}

/**
 * Function that is run once at boot. Initialises serial communication, WiFi, BME680-sensor 
 * and webSocket events.
 */
void setup() {
    Serial.begin(9600);                                           //Start the serial monitor
    Serial.setDebugOutput(true);                                  //Set debug to true during ESP32 booting to see what is happening with the Wifi connection (dependent on Wifi.h library)
    Serial.println();
    Serial.println();
    Serial.println();

    for(uint8_t t = 3; t > 0; t--) {                              //Wait three seconds to boot the ESP32
      Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
    }

    WiFiMulti.addAP(SSID, PASS);                                  //Add the WiFi credentials to connect to an access point

    while(WiFiMulti.run() != WL_CONNECTED) {                      //Wait for a successful WiFi connection until before doing anything else
      Serial.println("Not connected to wifi...");
        delay(100);
    }

    Serial.println("Connected to WiFi successfully!");

    while(!bme.begin()) {                                         //Wait for a successful connection to the BME680-sensor
      Serial.println("No BME680 sensor connected. Check wiring.");
      delay(100);
    }

    webSocket.on("clientConnected", event);                       //Declares all the different events the ESP32 should react to on the specified identifier
    webSocket.on("LEDStateChange", changeLEDState);               //When one of the identifiers is sent from the server and received on the client, then the socket will call the associated function on the client (ESP32)
    webSocket.on("windowStateChange", changeWindowState);
    webSocket.on("dataRequest", dataRequest);

    webSocket.begin(IP, PORT);                                    //Starts the connection to the server with the provided ip-address and port (unencrypted)

    bme.setTemperatureOversampling(BME680_OS_8X);                 //Set up oversampling and filter initialization
    bme.setHumidityOversampling(BME680_OS_2X);                    //I.e. how many times the sensor value should be read for this one specific reading
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);                                   //Heat up the gas resistance sensor to 320 *C for 150 ms to start measurement (takes about 30 minutes after boot to stabilize, higher reading gives lower VOC concentration)

    servo.setPeriodHertz(50);                                     //Standard 50 Hz servo (like the SG90 or MG90S) that has a PWM period of 20 ms
    servo.attach(16, 1000, 2000);                                 //Attaches the servo to GPIO16 with a duty cycle of 1-2 ms (1 ms = 0 deg and 2 ms = 180 deg)  NB! Check yours!

    pinMode(18, OUTPUT);                                          //Declare the ESP32 board pin 18 as an output (to use as a warning light for bad indoor climate and the need to open a window)
}

/**
 * Loop function that keeps the webSocket connection running. Do not use
 * delays in this function, as it will interfer with webSocket operations.
 */
void loop() {
  webSocket.loop();
}
