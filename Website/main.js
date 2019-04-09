

var websocket  //websocket-"Object"
var outputDataStream = "";  //message, will be sent, if websocket-server is idle
var connectedToServer //flag, shows if client is connected to server (set by onOpen, reset by onClose)
var serverIdle = false; //flag, shows if server is idle. Is set when server sends "--serverIdle--"


var color= [0,0,0,0,0,0,0,0,0,0,0];
var ip1 = "ws://192.168.1.169/";
var ip ={1:"192.168.1.169",
    2:"99.168.1.169",
    3:"99.168.1.169",
    4:"99.168.1.169",
    5:"99.168.1.169",
    6:"99.168.1.169",
    7:"99.168.1.169",
    8:"99.168.1.169",
    9:"99.168.1.169"}

    
// Funktion for the LED_Buttons

function wechselfarbe(clicked_id) {
    color[clicked_id]++;
    if (color[clicked_id] > 3){
        color[clicked_id] = 0;
    }
    switch(color[clicked_id]){
        case 0:
            document.getElementById(clicked_id).style.backgroundColor = "gray";
            //httpGetAsync(ip[clicked_id]+"/gray",callback);
            websocket.send(color[clicked_id]);
            break;
        case 1:
            document.getElementById(clicked_id).style.backgroundColor = "red";
            //httpGetAsync(ip[clicked_id]+"/red",callback);
            websocket.send(color[clicked_id]);
            break;
        case 2:
            document.getElementById(clicked_id).style.backgroundColor = "green";
            //httpGetAsync(ip[clicked_id]+"/green",callback);
            websocket.send(color[clicked_id]);
            break;
        case 3:
            document.getElementById(clicked_id).style.backgroundColor = "blue";
            //httpGetAsync(ip[clicked_id]+"/blue",callback);
            websocket.send(color[clicked_id]);
            break;
    }
};

// Reset_Button
function rest(){
    for (var i = 1; i < 10; i++){
    document.getElementById(i).style.backgroundColor = "black";
    color[i]= 0;
    //httpGetAsync(ip[i]+"/gray",callback);
    }
}

// Alte Webserver Function
/*
function httpGetAsync(theUrl, callback)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", theUrl, true); // true for asynchronous 
    xmlHttp.send(null);
}

function callback(respond)
{
    if(respond==200){
        break;
    }
    else{
        break;
    }
}
*/



// Function to Connect to all ESP32
function connect() {
    mainWebSocket(ip1);
    /*
    for (var i = 1; i < 9; i++){
        mainWebSocket(ip[i],i);
    }
    */
}


function mainWebSocket(wsUri) {
    websocket = new WebSocket(wsUri);   //creating new WebSocket-Object, constructor needs IP-Adress (here...)
    websocket.onopen = function () { onOpen(); }; //function called by websocket.onopen-method (when websocket-connection is started)
    websocket.onclose = function () { onClose(); };   //function called by websocket.onclose-method (when websocket-connection is closed)
    websocket.onmessage = function (evt) { onMessage(evt); };   //function called by websocket.onmessage-method (when a message is recieved)
    websocket.onerror = function (evt) { onError(evt); };   //function called by websocket.onerror-method (when an error occured)
}

//websocket-action
function onOpen() {
    document.getElementById("disconnect").disabled = false;  //enable "Disconnect"-button
    document.getElementById("connect").disabled = true; //disable IP-textfield
    connectedToServer = true;
    doSend("D:" + Date().substr(16,8),id);    //Senden der Uhrzeit
}

//websocket-action
function onClose() {
    document.getElementById("connect").disabled = false;
    document.getElementById("disconnect").disabled = true;
    connectedToServer = false;
}

//Disconnecting from WebSocket by pressing button
function disconnect() {
    websocket.close();
    for (var i = 1; i < length.ip; i++){
        document.getElementById(i).style.backgroundColor = "black";
        color[i]= 0;
}

//write message to screen and send it via websocket (instantly)
function doSend(message) {
    writeToScreen("SENT: " + message);
    websocket.send(message);
}



//websocket-action
function onMessage(evt) {
    // Einfache Message
    // 0 = RGB(white)
    // 1 = Red
    // 2 = Green
    if (evt.data === "--serverIdle--") {
        serverIdle = true;
        doSend_buffered("");    //try to send buffered message (if it exists)
    } else if ("L" === evt.data.substr(0,1)) {  //if first letter is "B": button-message recieved
        if ("0" === evt.data.substr(1, 1)) { //button zero event (right)
            document.getElementById("Button0Val").innerHTML = evt.data.substr(3, 1);    //write state on website
            if ("1" === evt.data.substr(3, 1))
                document.getElementById("Button0Box").style.backgroundColor = "#00ff00";
            if ("0" === evt.data.substr(3, 1))
                document.getElementById("Button0Box").style = document.getElementsByClassName("mainBoarderBox").style;
        }
        if ("1" === evt.data.substr(1, 1)) { //button one event (left)
            document.getElementById("Button1Val").innerHTML = evt.data.substr(3, 1);    //write state on website
            if ("1" === evt.data.substr(3, 1))
                document.getElementById("Button1Box").style.backgroundColor = "#00ff00";
            if ("0" === evt.data.substr(3, 1))
                document.getElementById("Button1Box").style = document.getElementsByClassName("mainBoarderBox").style;
        }
    } else {    //unknown data are written to screen
        writeToScreen('<span style="color: blue;">RESPONSE: ' + evt.data + '</span>');
    }
    /*
    if (evt.data == '10')
    {
        websocket.close();
    }
    */
}

//websocket-action
function onError(evt) {
    writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}

//put message-data into buffer. Send this buffer if server is idle.
function doSend_buffered(message){
    //only handle if client is connected to Server
    if (connectedToServer) {
        //put message-data only, if it exists.
        if (message !== "") {
            outputDataStream = outputDataStream + message + ";";
        }
        //send, if server is idle and buffer is not empty
        if ((true === serverIdle) && ("" !== outputDataStream)) {
            websocket.send(outputDataStream);
            outputDataStream = "";  //empty stream
            serverIdle = false; //reset serverIdle-flag
        }
    }
}}