/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */
// app used ESP8266 RGBLED//
#include <ESP8266WiFi.h>

const char* ssid     = "Kapil_Pana";
const char* password = "Pana@sonic2017";
//
//pin map
int clockpin =D0;
int datapin =D1;
//
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
//
void updatestring(int R,int B,int G)
{
    shiftOut(datapin, clockpin, MSBFIRST, R);
    shiftOut(datapin, clockpin, MSBFIRST, B);
    shiftOut(datapin, clockpin, MSBFIRST, G);
 
}
//
int Red_,Green_,Blue_;
void setup() {
  Serial.begin(115200);
  delay(10);
   
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("strip/status/")!= -1)
    Serial.println("On/Off");
  else if (req.indexOf("/") != -1)
  {client.print(" ok amit");
  Serial.println("ok amit");
  int i=0;
  while(req[i]!='r' & req[i+1]!='='){
    i++;
  }
  i=i+2;
  char rd[5],j=0;
  while(req[i]!='&')
  {
    rd[j]=req[i];
    i++;
    j++;
    rd[j]='\0';
  }
    while(req[i]!='g' & req[i+1]!='='){
      i++;
    }
  i=i+2;
  char gn [5],k=0;
  while(req[i]!='&')
  {
    gn[k]=req[i];
    i++;
    k++;
    gn[k]='\0';
  }
    while(req[i]!='b' & req[i+1]!='='){
    i++;
    }
  i=i+2;
  char bl [5],l=0;
  while(req[i]!=' ')
  {
    bl[l]=req[i];
    i++;
    l++;
    bl[l]='\0';
  }
   
Red_=(int)(255-atoi(rd)/4);
Green_=(int)(255-atoi(gn)/4);
Blue_=(int)(255-atoi(bl)/4);

Serial.println("we got data");Serial.println(Red_);Serial.println(Green_);Serial.println(Blue_);
updatestring(Red_,Blue_,Green_);
  }
  else if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

