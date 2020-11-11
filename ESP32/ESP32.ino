#include <analogWrite.h>                                          //Include the different libraries needed for the project
#include <Wire.h>                                                 //SDA and SCL pins are declared in Wire.h library (SDA: 21, SCL: 22)
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>

#define SEALEVELPRESSURE_HPA (1013.25)

WiFiMulti WiFiMulti;                                              //Declare instances of the libraries that was included
SocketIoClient webSocket;
Adafruit_BME680 bme;

/**
 * Function that is run when the ESP32 receives clientConnected keyword on webSocket.
 * Takes the incoming char array, formats it to string with printf command,
 * and prints in serial monitor.
*/
void event(const char * payload, size_t length) {                 //Default event, print the received data in serial monitor
  Serial.printf("Got message: %s from server\n", payload);
}

/**
 * Function that is run when the ESP32 receives LEDStateChange keyword on webSocket. 
 * Takes the incoming char array, formats it to string and prints to serial monitor.
 * Also converts the char array data to a string, converts it to an integer and
 * prints it in the serial monitor. 
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
 * 
 * 
 * 
 * 
 */
void dataRequest(const char * DataRequestData, size_t length) {   //This is the function that is called everytime the server asks for data from the ESP32
  Serial.printf("Datarequest Data: %s\n", DataRequestData);
  Serial.println(DataRequestData);

  //Data conversion
  String dataString(DataRequestData);
  int RequestState = dataString.toInt();

  Serial.print("This is the Datarequest Data in INT: ");
  Serial.println(RequestState);

  if(RequestState == 0) {                                         //If the datarequest gives the variable 0, do this (default)
    
    char str[10];                                                 //Decalre a char array (needs to be char array to send to server)
    itoa(analogRead(27), str, 10);                                //Use a special formatting function to get the char array as we want to, here we put the analogRead value from port 27 into the str variable
    Serial.print("ITOA TEST: ");
    Serial.println(str);
    
    webSocket.emit("dataFromBoard", str);                         //Here the data is sendt to the server and then the server sends it to the webpage
                                                                  //Str indicates the data that is sendt every timeintervall, you can change this to "250" and se 250 be graphed on the webpage
    /*
    bme.performReading();
    
    char temperature[10];
    itoa(bme.temperature, temperature, 10);
    webSocket.emit("nodeTemperature", temperature);
    Serial.println(temperature);
    */

    nodeData();
  }
}

void nodeData() {
  bme.performReading();
  nodeTemperature();
  nodePressure();
  nodeHumidity();
  nodeVOC();
}

void nodeTemperature() {
  char temperature[10];
  itoa(bme.temperature, temperature, 10);
  webSocket.emit("nodeTemperature", temperature);
  Serial.println(temperature);
}

void nodePressure() {
  char pressure[10];
  itoa(bme.pressure, pressure, 10);
  webSocket.emit("nodePressure", pressure);
  Serial.println(pressure);
}

void nodeHumidity() {
  char humidity[10];
  itoa(bme.humidity, humidity, 10);
  webSocket.emit("nodeHumidity", humidity);
  Serial.println(humidity);
}

void nodeVOC() {
  char gas_resistance[10];
  itoa(bme.gas_resistance, gas_resistance, 10);
  webSocket.emit("nodeVOC", gas_resistance);
  Serial.println(gas_resistance);
}

void setup() {
    Serial.begin(9600);                                           //Start the serial monitor

    Serial.setDebugOutput(true);                                  //Set debug to true (during ESP32 booting)

    Serial.println();
    Serial.println();
    Serial.println();

      for(uint8_t t = 4; t > 0; t--) {                            //Wait four seconds to boot the ESP32
          Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
          Serial.flush();
          delay(1000);
      }

    WiFiMulti.addAP("SSID", "passwd");                            //Add a WiFi hotspot (addAP = add AccessPoint)

    while(WiFiMulti.run() != WL_CONNECTED) {                      //Here we wait for a successfull WiFi connection untill we do anything else
      Serial.println("Not connected to wifi...");
        delay(100);
    }

    Serial.println("Connected to WiFi successfully!");            //When we have connected to a WiFi hotspot

    while(!bme.begin()) {
      Serial.println("No BME680 sensor connected. Check wiring.");
      delay(100);
    }

    //Here we declare all the different events the ESP32 should react to if the server tells it to.
    //A socket.emit("identifier", data) on the server, with any of the identifieres as defined below will make the socket call the functions in the arguments below
    webSocket.on("clientConnected", event);                       //For example, the socket.io server on node.js calls client.emit("clientConnected", ID, IP) Then this ESP32 will react with calling the event function
    webSocket.on("LEDStateChange", changeLEDState);

    //Send data to server/webpage
    webSocket.on("dataRequest", dataRequest);                     //Listens for the command to send data

    webSocket.begin("192.168.1.158", 2520);                       //This starts the connection to the server with the ip-address/domainname and a port (unencrypted)

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    pinMode(18, OUTPUT);                                          //The LED-pin
}

void loop() {
  webSocket.loop(); //Keeps the WebSocket connection running 
  //DO NOT USE DELAY HERE, IT WILL INTERFER WITH WEBSOCKET OPERATIONS
  //TO MAKE TIMED EVENTS HERE USE THE millis() FUNCTION OR PUT TIMERS ON THE SERVER IN JAVASCRIPT
}
