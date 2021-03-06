// THIS JS-CODE IS BASED ON THE EXAMPLE CODE GIVEN FROM THE TEACHERS. YOU WILL THEREFORE FIND SEVERAL SIMILARITIES

var socket = io.connect('192.168.1.158:2520', {secure: false}); //This line declares a socket.io object to var "socket" and connects to the server (change the IP-address and port to your own)
//The "secure: false" tells if the connection will be encrypted or not. Since we will not encrypt our connections, this is false.

//Socket.io has several functions. The .on function refers to what will happen when the client receive a call called 'connect' from the server
//View it as calling a function remotley. The server tells the client it wants to call this function with no arguments.
socket.on('connect',function() { //When you connect to the server (and it works) call this function
    console.log('Client has connected to the server!'); //The client prints this message
}); //The 'connect' function/identifier is the standard procedure. To make something more we have to make it ourselves

socket.on('clientConnected',function(id, ip) { //This is our self-made functions. Here we can have the server return arguments (data) that we need
    console.log('Client recevied ID: ' + id); //In this case the server will tell us what our local ID is (auto assigned)
    console.log('Client IP: ' + ip);//And it will tell us what our IP-address

});

socket.on('temperatureData', function(data) { //Received data from the server who is forwarding it to UI from the ESP32
    document.getElementById('temp').innerHTML = data;
    console.log('Received temperature: ' + data + ' ℃');

});

socket.on('pressureData', function(data) { //Received data from the server who is forwarding it to UI from the ESP32
    document.getElementById('pressure').innerHTML = data;
    console.log('Received pressure: ' + data + ' hPa');

});

socket.on('humidityData', function(data) { //Received data from the server who is forwarding it to UI from the ESP32
    document.getElementById('hum').innerHTML = data;
    console.log('Received relative humidity: ' + data + ' %');

});

socket.on('vocData', function(data) { //Received data from the server who is forwarding it to UI from the ESP32
    document.getElementById('voc').innerHTML = data;
    console.log('Received VOC level: ' + data + ' kOhm');

});

//In this function (which is essentially built up the same as a void function in Arduino) we want to send something to the server
//For this we use the other important Socket.io function, .emit(arg). Here we are telling our socket object so call the "changeLEDState" function
//on the server with the "state" argument. By calling the function on the server we mean that we send data to the server that tells it to do something



//This function also emits something to the server. But in this case we want something a little bit more complex to happen.
//Since Arduino has a limited amount of timers, and using millis can be annoying, we have the posibilties of handing that task over to JavaScript on Node.js
//The function we are calling here will tell the server to set up a JavaScript timer, which then will periodically send a message to the ESP32 asking for data.
//Since the ESP32 easily can react to such a request it sends the data with no problems, and with no timers in use.
//This means we dont have to use the delay() function or the millis() function in Arduino, we can just let Node and JavaScript fix the tracking of time for us
//This is the function that will make the ESP32 transmit data to the server, and not the other way around
function requestDataFromBoard(interval) {
    socket.emit('requestDataFromBoard', interval); //Here we tell the server to call the function "requestDataFromBoard" with a argument called "interval"
    //The interval value is the period of time between each data transmit from the ESP32 to the server. Typical values can be everything form 100ms to 100s
    console.log('requestDataFromBoard was called with interval (ms): ' + interval);
} //Be careful to not set the interval value to low, you do not want to overflood your server with data/requests

function stopDataFromBoard() {
    socket.emit('stopDataFromBoard'); //Here we tell the server to call the function "stopDataFromBoard"
    console.log('stopDataFromBoard was called');
}

function resetData() { //This function tells the server to reset its dataArrays (database)
    socket.emit('resetData', 1);
    console.log("resetData was called");
}

function automation(interval) { //This function will be forwarded by the server so the ESP32 enters automation mode
    socket.emit('startAutomation', interval);
    console.log('Automation was called with interval(ms): ' + interval);
}

function manual() { //This function will be forwarded by the server so the ESP32 enters manual mode
    socket.emit('stopAutomation');
    console.log('stopAutomation was called');
}

function sliderValue() { //This function will be forwarded by the server so the ESP32 changes the value of the window servo
    socket.emit('changeWindowState', angle);
    console.log('Slider-value set to: ', angle);
}

function limtemp() { //This function will be forwarded by the server so the ESP32 changes its temperature limit
    socket.emit('maxTemperature', temp_lim);
    console.log("maxTemperature: ", temp_lim);
}

function limhum() { //This function will be forwarded by the server so the ESP32 changes its humidity limit
    socket.emit('maxHumidity', hum_lim);
    console.log("maxHumidity: ", hum_lim);
}

function limVOC() { //This function will be forwarded by the server so the ESP32 changes its VOC-limit
    socket.emit('maxVOC', VOC_lim);
    console.log("maxVOC: ", VOC_lim);
}
