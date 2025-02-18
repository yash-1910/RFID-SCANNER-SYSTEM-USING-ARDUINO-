#include <SPI.h> 
#include <MFRC522.h> 
#include <LiquidCrystal_I2C.h> 
#include <Servo.h> 
 
#define RST_PIN 9 
#define SS_PIN 10 
 
MFRC522 mfrc522(SS_PIN, RST_PIN); 
String MasterTag1 = "D9 F5 BB 99"; String MasterTag2 = "39 94 89 65"; 
//String MasterTag3 = "39 94 89 65"; 
 
 
String UIDCard = ""; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo servo; 
#define GreenLED 4 //defines the pin no. 
#define RedLED 3 #define Buzzer 5 void setup() { 
Serial.begin(9600); 
SPI.begin(); mfrc522.PCD_Init(); 
 
lcd.init(); lcd.backlight(); 
lcd.clear(); 
 
servo.attach(6); //on the pin 6 
servo.write(120); //servo motor initial angle 
 
pinMode(GreenLED, OUTPUT); pinMode(RedLED, OUTPUT); 
 
pinMode(Buzzer, OUTPUT); 
lcd.clear(); 
	lcd.print(" 	HELLO 	"); 
delay(2500); //in ms as default 
 
lcd.clear(); 
lcd.print("SECURITY CONTROL"); 
lcd.setCursor(0, 1); lcd.print("using RFID Tech"); delay(2500); 
 
lcd.clear(); lcd.print(" TO GAIN Access "); lcd.setCursor(0, 1); lcd.print("<Scan Your Card>"); 
} 
 
void loop() { 
 
digitalWrite(RedLED, LOW); digitalWrite(GreenLED, LOW); noTone(Buzzer); 
servo.write(120); 
 
while (getUID()) 
{ 
Serial.print("UID: "); Serial.println(UIDCard); lcd.clear(); 
lcd.setCursor(0, 0); 
lcd.print(" SCANNING "); 
lcd.setCursor(0, 1); delay(500); 
 
if (UIDCard == MasterTag1 || UIDCard == MasterTag2) 
{ lcd.print("Access Granted->"); digitalWrite(GreenLED, HIGH); digitalWrite(RedLED, LOW); servo.write(10); delay(50); for (int i = 0; i < 2; i++) { 
tone(Buzzer, 2000); delay(250); noTone(Buzzer); delay(250); 
} 
delay(300); lcd.clear(); 
lcd.setCursor(0, 0); 
	lcd.print(" YOU MAY 	"); 
lcd.setCursor(0, 1); 
lcd.print(" WALK IN "); delay(500); 
 
} else { lcd.print("Access Denied !!"); digitalWrite(GreenLED, LOW); tone(Buzzer, 1000); servo.write(120); for(int i = 0; i < 6;i++){ //blink for 6 times 
digitalWrite(RedLED, HIGH); delay(250); digitalWrite(RedLED, LOW); 
delay(250); 
} 
noTone(Buzzer); 
} 
 
delay(700); 
 
lcd.clear(); 
lcd.print(" TO GAIN Access "); lcd.setCursor(0, 1); lcd.print("<Scan Your Card>"); 
} 
} 
boolean getUID() 
{ if (! mfrc522.PICC_IsNewCardPresent()) { 
return false; 
} 
 
if (! mfrc522.PICC_ReadCardSerial()) { 
return false; 
} 
UIDCard = ""; for (byte i = 0; i < mfrc522.uid.size; i++) { 
UIDCard.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); 
UIDCard.concat(String(mfrc522.uid.uidByte[i], HEX)); } 
UIDCard.toUpperCase(); 
UIDCard = UIDCard.substring(1); 
 
mfrc522.PICC_HaltA(); return true; 
} 
