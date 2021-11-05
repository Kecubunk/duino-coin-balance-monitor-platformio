#include <WiFi.h>
#include <HTTPClient.h>
#include <Ticker.h>
#include <Update.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
//#include <Ticker.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

const char* ssid     = "SSID";
const char* password = "password";

//change DUCO_Username with youre Duinocoin user name
const char* server = "https://server.duinocoin.com/users/DUCO_Username" ;
unsigned long last_time = 0;
unsigned long timer_delay = 10000;
String json_array;
int sgnl=2;

//Ticker ticker;
HTTPClient http;

/*
void tick(){
  int state = digitalRead(sgnl);
  digitalWrite(sgnl, !state);
}
*/

void Init(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(7, 1);
    lcd.print("ESP32");  
    lcd.setCursor(4, 2);
    lcd.print("Duco Monitor"); 
    lcd.setCursor(0, 3);
    lcd.print("====================");
    return;
}

void Connected(){
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("SSID : ");
   lcd.setCursor(7,1);   
   lcd.print(ssid);
   lcd.setCursor(0,2);
   lcd.print("IP   : ");
   lcd.setCursor(7,2);
   lcd.print(WiFi.localIP());
}

void setup() {
  pinMode(sgnl,OUTPUT);
  digitalWrite(sgnl,HIGH);
  Serial.begin(115200);
  while (!Serial) continue;
  //ticker.attach(0.1, tick);
  Init();
  delay(500);
  WiFi.begin(ssid, password);
  //connecting();
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
  }
  //ticker.detach();
  Connected();
  delay(750);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

String GET_Request(const char* server) {
  HTTPClient http;    
  //http.begin(server,ca);
  http.begin(server);
  int httpResponseCode = http.GET();
  
  String payload = ""; 

  //ticker.attach(0.1, tick);
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  //ticker.detach();
  
  http.end();
  return payload;
}


void loop() {
      GET_Request(server);
      //ticker.attach(0.1, tick);
      json_array = GET_Request(server);
      
      Serial.println(json_array);
      JSONVar obj = JSON.parse(json_array);
      if (JSON.typeof(obj) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      Serial.print("User : ");
      Serial.println(obj["result"]["balance"]["username"]);     
      Serial.print("Balance : ");
      Serial.print(obj["result"]["balance"]["balance"]);
      Serial.println(" Duco");
      Serial.print("Verified : ");      
      Serial.println(obj["result"]["balance"]["verified"]);
      Serial.println("");
      int a = obj["result"]["balance"]["balance"];
      
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Duco Monitor");
      lcd.setCursor(0, 1);
      lcd.print("User    : ");
      lcd.setCursor(10, 1);
      lcd.print(obj["result"]["balance"]["username"]);
      lcd.setCursor(0, 2);
      lcd.print("Verified: ");
      lcd.setCursor(10, 2);   
      lcd.print(obj["result"]["balance"]["verified"]);         
      lcd.setCursor(0, 3);
      lcd.print("Balance : ");
      lcd.setCursor(10, 3);
      lcd.print(a);
      //ticker.detach();      
      delay(10000);
}

