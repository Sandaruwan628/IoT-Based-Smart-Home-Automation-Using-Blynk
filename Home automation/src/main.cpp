#include <Arduino.h>

#define BLYNK_TEMPLATE_ID "TMPL61hYCvkBW"
#define BLYNK_TEMPLATE_NAME "wokwi"
#define BLYNK_AUTH_TOKEN "bfin7AtjiRZZZ_6gy1FFltETZTjN_Skr"


// Comment this out to disable prints and save space
// #define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"




char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;


#define button1_pin 26
#define button2_pin 25
#define button3_pin 33
#define button4_pin 32

#define relay1_pin 13
#define relay2_pin 12
#define relay3_pin 14
#define relay4_pin 27

#define ir_pin 15

#define lcd_SDA 2
#define lcd_SCL 4

int relay1_state = 0;
int relay2_state = 0;
int relay3_state = 0;
int relay4_state = 0;


//Change the virtual pins according the rooms
#define button1_vpin    V0
#define button2_vpin    V1
#define button3_vpin    V2 
#define button4_vpin    V3

IRrecv receiver(ir_pin);
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHTesp dhtSensor;

//------------------------------------------------------------------------------
// This function is called every time the device is connected to the Blynk.Cloud
// Request the latest state from the server
BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
  Blynk.syncVirtual(button3_vpin);
  Blynk.syncVirtual(button4_vpin);
}

//--------------------------------------------------------------------------
// This function is called every time the Virtual Pin state change
//i.e when web push switch from Blynk App or Web Dashboard
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(button2_vpin) {
  relay2_state = param.asInt();
  digitalWrite(relay2_pin, relay2_state);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(button3_vpin) {
  relay3_state = param.asInt();
  digitalWrite(relay3_pin, relay3_state);
}
//--------------------------------------------------------------------------
BLYNK_WRITE(button4_vpin) {
  relay4_state = param.asInt();
  digitalWrite(relay4_pin, relay4_state);
}
//--------------------------------------------------------------------------


void lcdPrint(char* text)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  // lcd.print("button pressed:");
  // lcd.setCursor(0, 1);
  lcd.print(text);
  // lcd.print(" code: ");
  // lcd.print(receiver.decodedIRData.command);
}

void translateIR()
{
  // Takes command based on IR code received
  switch (receiver.decodedIRData.command) {
    case 162:
      lcdPrint("POWER");
      break;
    case 226:
      lcdPrint("MENU");
      break;
    case 34:
      lcdPrint("TEST");
      break;
    case 2:
      lcdPrint("PLUS");
      break;
    case 194:
      lcdPrint("BACK");
      break;
    case 224:
      lcdPrint("PREV.");
      break;
    case 168:
      lcdPrint("PLAY");
      break;
    case 144:
      lcdPrint("NEXT");
      break;
    case 104:
      lcdPrint("num: 0");
      break;
    case 152:
      lcdPrint("MINUS");
      break;
    case 176:
      lcdPrint("key: C");
      break;
    case 48:
      delay(200);
      control_relay(1);
      if (relay1_state==0) {
        lcdPrint("LED 1 turned OFF");
      }else{
        lcdPrint("LED 1 turned ON");
      }
      Blynk.virtualWrite(button1_vpin, relay1_state);
      break;
    case 24:
      delay(200);
      control_relay(2);
      if (relay2_state==0) {
        lcdPrint("LED 2 turned OFF");
      }else{
        lcdPrint("LED 2 turned ON");
      }
      Blynk.virtualWrite(button2_vpin, relay2_state);
      break;
    case 122:
      delay(200);
      control_relay(3);
      if (relay3_state==0) {
        lcdPrint("LED 3 turned OFF");
      }else{
        lcdPrint("LED 3 turned ON");
      }
      Blynk.virtualWrite(button3_vpin, relay3_state);
      break;
    case 16:
      delay(200);
      control_relay(4);
      if (relay4_state==0) {
        lcdPrint("LED 4 turned OFF");
      }else{
        lcdPrint("LED 4 turned ON");
      }
      Blynk.virtualWrite(button4_vpin, relay4_state);
      break;
    case 56:
      lcdPrint("num: 5");
      break;
    case 90:
      lcdPrint("num: 6");
      break;
    case 66:
      lcdPrint("num: 7");
      break;
    case 74:
      lcdPrint("num: 8");
      break;
    case 82:
      lcdPrint("num: 9");
      break;
    default:
      lcd.clear();
      lcd.print(receiver.decodedIRData.command);
      lcd.print(" other button");
  }
}

void ir()
{
  if (receiver.decode()) {
    translateIR();
    receiver.resume();  // Receive the next value
  }
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void listen_push_buttons(){
    //--------------------------------------------------------------------------
    if(digitalRead(button1_pin) == LOW){
      delay(200);
      control_relay(1);
      if (relay1_state==0) {
        lcdPrint("LED 1 turned OFF");
      }else{
        lcdPrint("LED 1 turned ON");
      }
      Blynk.virtualWrite(button1_vpin, relay1_state); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(button2_pin) == LOW){
      delay(200);
      control_relay(2);
      if (relay2_state==0) {
        lcdPrint("LED 2 turned OFF");
      }else{
        lcdPrint("LED 2 turned ON");
      }
      Blynk.virtualWrite(button2_vpin, relay2_state); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(button3_pin) == LOW){
      delay(200);
      control_relay(3);
      if (relay3_state==0) {
        lcdPrint("LED 3 turned OFF");
      }else{
        lcdPrint("LED 3 turned ON");
      }
      Blynk.virtualWrite(button3_vpin, relay3_state); //update button state
    }
    //--------------------------------------------------------------------------
    else if (digitalRead(button4_pin) == LOW){
      delay(200);
      control_relay(4);
      if (relay4_state==0) {
        lcdPrint("LED 4 turned OFF");
      }else{
        lcdPrint("LED 4 turned ON");
      }
      Blynk.virtualWrite(button4_vpin, relay4_state); //update button state
    }
    //--------------------------------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
void control_relay(int relay){
  //------------------------------------------------
  if(relay == 1){
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 2){
    relay2_state = !relay2_state;
    digitalWrite(relay2_pin, relay2_state);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 3){
    relay3_state = !relay3_state;
    digitalWrite(relay3_pin, relay3_state);
    delay(50);
  }
  //------------------------------------------------
  else if(relay == 4){
    relay4_state = !relay4_state;
    digitalWrite(relay4_pin, relay4_state);
    delay(50);
  }
  //------------------------------------------------
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

void temp()
{
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println("DHT22 Error: Check wiring or sensor!");
    lcdPrint("Sensor Error!");
    return;
  }
  // Serial.print("Temp: ");
  // Serial.print(data.temperature);
  // float humidity = dht.readHumidity();
  // Blynk.virtualWrite(V4, temp);
}


void setup()
{
  // Debug console
  Serial.begin(115200);
  //--------------------------------------------------------------------
  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  pinMode(button4_pin, INPUT_PULLUP);
  //--------------------------------------------------------------------
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);
  //--------------------------------------------------------------------
  //During Starting all Relays should TURN OFF
  digitalWrite(relay1_pin, LOW);
  digitalWrite(relay2_pin, LOW);
  digitalWrite(relay3_pin, LOW);
  digitalWrite(relay4_pin, LOW);

  Wire.begin(lcd_SDA, lcd_SCL);
  lcd.init();
  lcd.backlight();
  lcd.print("Welcome to Smart Home");
  receiver.enableIRIn();

  //--------------------------------------------------------------------

  dhtSensor.setup(22, DHTesp::DHT22);
  delay(2000);

  Blynk.begin(auth, ssid, pass);

  // timer.setInterval(1000L, temp);
}

void loop()
{
  Blynk.run();
  timer.run();
  temp();
  ir();
  listen_push_buttons();
}