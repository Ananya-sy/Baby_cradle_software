#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>


const char* ssid = "Baby123";
const char* password = "123456789";

WiFiClient  client;

// Telegram Bot Token (from BotFather)
#define BOT_TOKEN "7634618037:AAHCRclktBMJakNLLheU_My_dhqufHHNAVI"

// Your Telegram Chat ID
#define CHAT_ID "1384719449"

// Initialize Telegram BOT
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

unsigned long lastTime;
unsigned long botRequestDelay = 1000; // Check for new messages every second

int str_len;
char textmessage[20];
void Connect_wifi();


unsigned long temp=0;

char ch;
#define heart 13


#include<dht.h>  
#define dht_dpin 4


dht DHT;


int data=34;
int count=0;

int IN1=18;
int IN2=19;

int Voice=5;

int Relay=2;

void SWING_START();

char mystr[10];


byte customChar1[8] = {0b00000,0b00000,0b00011,0b00111,0b01111,0b01111,0b01111,0b01111};
byte customChar2[8] = {0b00000,0b11000,0b11100,0b11110,0b11111,0b11111,0b11111,0b11111};
byte customChar3[8] = {0b00000,0b00011,0b00111,0b01111,0b11111,0b11111,0b11111,0b11111};
byte customChar4[8] = {0b00000,0b10000,0b11000,0b11100,0b11110,0b11110,0b11110,0b11110};
byte customChar5[8] = {0b00111,0b00011,0b00001,0b00000,0b00000,0b00000,0b00000,0b00000};
byte customChar6[8] = {0b11111,0b11111,0b11111,0b11111,0b01111,0b00111,0b00011,0b00001};
byte customChar7[8] = {0b11111,0b11111,0b11111,0b11111,0b11110,0b11100,0b11000,0b10000};
byte customChar8[8] = {0b11100,0b11000,0b10000,0b00000,0b00000,0b00000,0b00000,0b00000};
float tempc,Humidity;

int i;
void setup() 
{

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(Voice,OUTPUT);
   pinMode(Relay,OUTPUT);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(Voice,LOW);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(1, customChar1);
    lcd.createChar(2, customChar2);
    lcd.createChar(3, customChar3);
    lcd.createChar(4, customChar4);
    lcd.createChar(5, customChar5);
    lcd.createChar(6, customChar6);
    lcd.createChar(7, customChar7);
    lcd.createChar(8, customChar8);
    
    pinMode(data,INPUT);
   
 pinMode(heart, OUTPUT);
  lcd.clear();
  lcd.print("   SMART BABY ");
  lcd.setCursor(0, 1);
  lcd.print("     CRADLE.");
  delay(1000);
   WiFi.mode(WIFI_STA);   
 
  Connect_wifi();


      // Initialize secure client
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Required for HTTPS
  // If certificate issues, use:
  // secured_client.setInsecure();

  // Send a startup message to Telegram
  bot.sendMessage(CHAT_ID, "Baby Monitoring System...");

 }
void Connect_wifi()
{



  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

 void loop()
 {

    WET_CHECK();
    SOUND_CHECK();
    TEMP_CHECK();
   HEARTBEAT_CHECK();
      if (millis() - lastTime > botRequestDelay) 
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      for (int i=0; i<numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;

        Serial.println("Message received: " + text);

        // Command handling
        if (text == "/hello") {
          bot.sendMessage(chat_id, "Hello! ESP32 is online.", "");
        }
        else if (text == "/status") 
        {
          String status = "HeartBeat:"+String(count)+'\n'+"Temperature:"+String(tempc)+'\n'+"Humidity:"+String(Humidity);
          bot.sendMessage(chat_id, status, "");
        }
        
         else if (text == "/FanOn") 
        {
          digitalWrite(Relay, HIGH);
          bot.sendMessage(chat_id, "Fan turned On.", "");
        }
        else if (text == "/FanOff") 
        {
          digitalWrite(Relay, LOW);
          bot.sendMessage(chat_id, "Fan turned OFF.", "");
        }
         else if (text == "/CradleOn") 
        {
          //digitalWrite(Relay, High);
          bot.sendMessage(chat_id, "Swinging On.", "");
          SWING_START();
        }
       
        else {
          bot.sendMessage(chat_id, "Unknown command. Available commands:\n/hello\n/status\n/FanOn\n/FanOff\n/CradleOn\n/CradleOff", "");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTime = millis();
  }
  

 }
void TEMP_CHECK()
{
 DHT.read11(dht_dpin);
      Humidity=DHT.humidity;
      tempc=DHT.temperature;
      Serial.print("$Humidity: ");
      Serial.print(DHT.humidity);   // printing Humidity on LCD
      Serial.print(" %");
      Serial.print(" # ");
  
      Serial.print("$Temperature:");
      Serial.print(DHT.temperature);   // Printing temperature on LCD
      Serial.print(" C");
      Serial.println(" # ");

      lcd.clear();
      lcd.print("Humidity:"+String(DHT.humidity));
      lcd.setCursor(0,1);
      lcd.print("Temp:"+String(DHT.temperature));
      
      delay(2000);
  

  if(tempc>33.0)
  {
    lcd.clear();
    lcd.print("Temp is More");
    digitalWrite(Relay,HIGH);
    Serial.println("$Baby Temperature is More#");
     bot.sendMessage(CHAT_ID, "Temperature is More...");
    delay(1000);
    
  }
  if(tempc<32.0)
  {
     digitalWrite(Relay,LOW);
  }
SOUND_CHECK();
}

void WET_CHECK()
{
  int Wet_val=analogRead(39);
   Wet_val=4095-Wet_val;
  lcd.clear();
  lcd.print("Wet:");
  lcd.print(Wet_val);
  Serial.print("Wet:");
  Serial.println(Wet_val);
  delay(1000);
  if(Wet_val>600)
  {
    lcd.clear();
    lcd.print("Wet Detected");
    Serial.println("$Wet Detected#");
      bot.sendMessage(CHAT_ID, "Wet Detected...");
    
    delay(1000);
    
  }
SOUND_CHECK();
}
void SOUND_CHECK()
{
  int Sound_val=analogRead(36);
  Sound_val=4095-Sound_val;
  lcd.clear();
  lcd.print("Sound:");
  lcd.print(Sound_val);
  Serial.print("Sound:");
  Serial.println(Sound_val);
  delay(1000);

  if(Sound_val>1200)
  {
    lcd.clear();
    lcd.print("Baby Crying..:");
    Serial.println("$Baby Crying..#");
     bot.sendMessage(CHAT_ID, "Baby Crying Detected...");
    delay(1000);
    SWING_START();
    

  }

}
void SWING_START()
{
  lcd.clear();
  lcd.print("Swinging....");
   digitalWrite(Voice,HIGH);
   delay(1000);
   for(i=0;i<5;i++)
  {
     digitalWrite(Voice,HIGH);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    delay(300);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    delay(200);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    delay(300);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    delay(200);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    delay(300);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    delay(200);
     digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    delay(300);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    delay(200);
  }

 Serial.println("HIII...");
  digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    delay(100);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    delay(200);
 digitalWrite(Voice,LOW);

}
void HEARTBEAT_CHECK()
{
   count=0;
   lcd.setCursor(0, 0);
   lcd.print("Place The Finger");
   lcd.setCursor(0, 1);
   lcd.print("to check HB");
   delay(2000);
 // while(digitalRead(start)>0);
  
   lcd.clear();
   temp=millis();
   
   while(millis()<(temp+10000))
   {
      if(analogRead(data)<100)
        {
         count=count+1;
         
         lcd.setCursor(6, 0);
         lcd.write(byte(1));
         lcd.setCursor(7, 0);
         lcd.write(byte(2));
         lcd.setCursor(8, 0);
         lcd.write(byte(3));
         lcd.setCursor(9, 0);
         lcd.write(byte(4));

         lcd.setCursor(6, 1);
         lcd.write(byte(5));
         lcd.setCursor(7, 1);
         lcd.write(byte(6));
         lcd.setCursor(8, 1);
         lcd.write(byte(7));
         lcd.setCursor(9, 1);
         lcd.write(byte(8));
         
         while(analogRead(data)<100);

         lcd.clear();
        }
   }
   
         lcd.clear();
         lcd.setCursor(0, 0);
         count=count*5;
         lcd.setCursor(2, 0);
         lcd.write(byte(1));
         lcd.setCursor(3, 0);
         lcd.write(byte(2));
         lcd.setCursor(4, 0);
         lcd.write(byte(3));
         lcd.setCursor(5, 0);
         lcd.write(byte(4));

         lcd.setCursor(2, 1);
         lcd.write(byte(5));
         lcd.setCursor(3, 1);
         lcd.write(byte(6));
         lcd.setCursor(4, 1);
         lcd.write(byte(7));
         lcd.setCursor(5, 1);
         lcd.write(byte(8));
         lcd.setCursor(7, 1);
         if(count>130)
         count=70;
         lcd.print(count);
         lcd.print(" BPM");

      sprintf(mystr, "HB:%d", count);
      Serial.println(mystr);
  
      delay(1000);
      if(count>120)
      {
        lcd.clear();
        lcd.print("More Heartbeat..");
         Serial.println("$More Heartbeat#"); 
          bot.sendMessage(CHAT_ID, "More Heartbeat Detected...");
   
      delay(1000); 
        
      }

      temp=0;
      count=0;
      delay(300);
SOUND_CHECK();
}