 #include <ESP8266WiFi.h>
 

const char* ssid = "rodentgopro";
const char* password = "zzuubb";
 
int ledPin = 16; // GPIO13

long ledTimer = 0;
long photoTimer = 0;
long aliveTimer = 0;

const int nodeD0 = 16;
const int nodeD1 = 14;
bool lastActive = false;

const char* host = "10.5.5.9";
String url2 = "/gp/gpControl/command/shutter?p=1";
String url = "/gp/gpControl/setting/17/0";


WiFiServer server(80);
WiFiClient client;



long timer = millis();

void doCommand(String command){

if (client.connect(host, 80))
{
  Serial.println("WiFi connected to the host!");
}
else
{
  Serial.println("WiFi connection failure!!");
}

  
    //http://10.5.5.9/gp/gpControl/command/shutter?p=1

  
  Serial.print("making photo Requesting URL:");
  Serial.println(command);
  
  // This will send the request to the server
  #if 0
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "Connection: close\r\n\r\n");
  delay(10);
  #endif
  client.print(String("GET ") + command + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  "Connection: close\r\n\r\n"
  //"\r\n"
  );
  Serial.println();
  Serial.println("closing connection");
  
//  delay(2000);// - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=2912#sthash.pJ9wfHZn.dpuf
}


void doCommandNoClose(String command){

if(!client.connected()){
  

if (client.connect(host, 80))
{
  Serial.println("WiFi connected to the host!");
}
else
{
  Serial.println("WiFi connection failure!!");
  return;
}
}
  
 
  // This will send the request to the server
  
  client.print(String("GET ") + command + " HTTP/1.1\r\n" +
  "Host: " + host + "\r\n" +
  //"Connection: close\r\n" 
  + "\r\n"
  //"\r\n"
  );
  delay(10);
  //Serial.println();

  client.flush();
 
}

void doPhoto(){
  doCommand(url2);  
}

void doPhotoNoClose(){
  doCommandNoClose(url2);  
}


void doMultiEVPhoto(){
  setEVMinusOne();
  delay(400); 
  //delay(100);
  //doPhoto();
  doPhotoNoClose();
  delay(900); // 1000 ok, maybe less?
  setEVZero();
  delay(400); 
  //delay(100);
  //doPhoto();
  doPhotoNoClose();

  return;
  delay(100);
  //setEVMinusHalf();
  setEVMinusOne();
  delay(50);
  doPhoto();
  //delay(100);
}


void setPhotoMode(){
 //doCommandNoClose(urlPhotoMode);   
}

void setEVMinusHalf(){
  doCommandNoClose("http://10.5.5.9/gp/gpControl/setting/26/5");
}

void setEVMinusOne(){
  doCommandNoClose("http://10.5.5.9/gp/gpControl/setting/26/6");
}

void setEVPlusHalf(){
  doCommandNoClose("http://10.5.5.9/gp/gpControl/setting/26/3");
}

void setEVZero(){
  doCommandNoClose("http://10.5.5.9/gp/gpControl/setting/26/4");
}
 
void setup() {
  Serial.begin(115200);
  delay(10);

  
  pinMode(nodeD0, INPUT);
  pinMode(nodeD1, INPUT);



  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);


 
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
//  WiFi.begin(ssid);

  
 
  while (WiFi.status() != WL_CONNECTED) {
    //while (true) {
    delay(500);
    //digitalWrite(ledPin, HIGH);
    //delay(200);
    //digitalWrite(ledPin, LOW);
    Serial.print(". ");
  }
  Serial.println("");
  Serial.println("WiFi connected");

 ledTimer = millis();
 photoTimer = millis();
 aliveTimer = millis();

return;
 //  

 /*
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 */
}
 
void loop() {

if(millis() - aliveTimer > 4000){
  aliveTimer = millis();
  int m = millis()%10;
  Serial.print("#");
  Serial.println(m);  
}

//while(1){
 bool val1 = digitalRead(nodeD0); // D0 na plytce, w gornym rogu z prawej strony, dziala OK
 bool val2 = digitalRead(nodeD1); 

bool usedSignal = val2;

  //Serial.println("val2=" + val2);
  //Serial.println("val1=" + val1);

/*
  if(val2)
    digitalWrite(ledPin, LOW);
  else   
    digitalWrite(ledPin, HIGH);
//}
*/
  if(usedSignal && lastActive == false){
    if(millis() - photoTimer > 1000){
      Serial.println("doing photo!");
      //doPhoto();
      doMultiEVPhoto();
      ledTimer = millis();   
      photoTimer = millis(); 
    }
  }
  if(millis() - ledTimer < 1000){
    digitalWrite(ledPin, HIGH);
  } else {
     digitalWrite(ledPin, LOW);
  }
  
  lastActive = usedSignal;

return;
  
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
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
