#include <Adafruit_Keypad.h>
#include <Adafruit_Keypad_Ringbuffer.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <LCD_ST7032.h>
#include <Keypad.h>
#include <EEPROM.h>//The EEPROM library is used to save the modified password, and it will not be lost after power off
SoftwareSerial mySerial(11, 12);

char id;


int pinBuzzer = 13; //Digital pin 13 is connected to the signal pin of the buzzer module
//Created instances
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int a=0;
int relayPin = 10;
LCD_ST7032 lcd;
const byte ROWS = 4; // rows
const byte COLS = 4; // columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','F'},
  {'4','5','6','E'},
  {'7','8','9','D'},
  {'A','0','B','C'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
Keypad customKeypad =Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//initialize an instance of class NewKeypad
String oldPassword = "";//Save old password
String newPassword = "";//Save the entered modified password
int addNum = 0;//Error count
String inPut = "";//Save input string
String n = "";//Variables for string conversion
unsigned long now;
unsigned long before;
unsigned long delayTime = 30000;


void setup() {
 pinMode(relayPin, OUTPUT);
 digitalWrite(relayPin, LOW);
 Serial.begin(9600);
  pinMode(beep, OUTPUT);
  lcd.begin();
//The password is read from the EEPROM at startup, and the four-digit password is stored in the EEPROM address 0-3
  for (int i = 0; i < 4; i++)
  {
    oldPassword += EEPROM.read(i);
  }
//Serial port printing test information
  Serial.print("Old Password is : ");
  Serial.println(oldPassword);
  delay(2000);
  lcd.setCursor(0, 0);//LCD output position, format row, column
  lcd.print("Please input :");
 Serial.begin(9600);
  while (!Serial);  
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}
int i = 6;//

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop() {
   uint8_t p = finger.getImage();
 switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
       char customKey = customKeypad.getKey();//Get button
    if (customKey)//There is a key press
    {
    inPut += customKey;//Save the input
    lcd.setCursor(1, i);//
    lcd.print('*');
    Serial.println("inPut is : ");
    Serial.println(inPut);
    Serial.println("onece");
    i++;
    if (i > 9)
    {
      i = 6;
    }
    }

  //Compare the password
   if (inPut == oldPassword) //Correct password
  {
    addNum = 0;//Zero error count
    inPut = "";
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("  Door Unlocked");
     lcd.setCursor(1, 0);
     lcd.print("    Welcome");
     digitalWrite(relayPin, HIGH);
     delay(3000);
     digitalWrite(relayPin, LOW);
     lcd.clear();
    delay(500);
  }


  
   if(inPut=="E273"){
    lcd.setCursor(0, 0); //1列1行
    lcd.print("Change Password?");
    Serial.println("change password?");
    lcd.setCursor(1, 0);
    lcd.print("1.YES       2.NO");
    int a = 1;
    while (a)//等待按键按下
    {
      customKey = customKeypad.getKey();
      Serial.println("while");
      if (customKey == '1') //Choose 1 to change password
      {
        a = 0;//Jump out of while loop
        Serial.println("select 1");
        inPut = "";//Clear input string
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Input the new:");
        for (int b = 0; b < 4;)//Input the new password
        {
          customKey = customKeypad.getKey();
          if (customKey)
          {
            lcd.setCursor(1, b + 4);
            lcd.print(customKey);
            newPassword += customKey;
            n = customKey;
            int num = n.toInt();//ToInt () function is used to convert the input character to integer
            n = "";
//EEPROM saves the password
           EEPROM.write(b, num );
            Serial.print("EEPROM ");
            Serial.print(b);
            Serial.print(" is : ");
            Serial.println(EEPROM.read(b));
           Serial.print("new password is ");
            Serial.println(newPassword);
            b++;
            if (b == 4 )//满足4位
            { newPassword = "";
              lcd.clear();
              lcd.setCursor(0, 0);//格式列、行
              lcd.print("Save the new !");
              delay(1000);//延时两秒
              oldPassword = "";
              for (int i = 0; i < 4; i++) 
              {
                oldPassword += EEPROM.read(i);
              }
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Please input :");
            }
          }
        }
      }
      else if (customKey == '2') //Choose to exit
      {
        a = 0;
        inPut = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please input :");
        Serial.println("back to home");
      }
    }
   }


   if(inPut=="0000")
   {
     lcd.setCursor(0, 0); 
    lcd.print("Edit fingerprint?");
    Serial.println("Edit fingerprint?");
    lcd.setCursor(1, 0);
    lcd.print("1.YES       2.NO");
    int a = 1;
    while (a)//Waiting for the key press
    {
      customKey = customKeypad.getKey();
      Serial.println("while");
      if (customKey == '1') 
      {
        a = 0;
        Serial.println("select 1");
        inPut = "";
        lcd.clear(); 
        Serial.println("1.Add  2.Delete  3.Exit");
        lcd.setCursor(0, 0);
        lcd.print("1.Add  2.Delete");
        lcd.setCursor(1, 0);
        lcd.print("3.Exit");
        int s = 1;
        while (s)
        {
           customKey = customKeypad.getKey();
         
           if (customKey == '1') 
          {
           s = 0;
           inPut = "";
            lcd.clear(); 
            lcd.setCursor(0, 0);
            lcd.print("Input the ID");
             Serial.println("while");
             int e = 1;
           while (e)
        {
         customKey = customKeypad.getKey();
         Serial.println("while");
         if (customKey == '1'){e=0;id='1';}
        else if(customKey == '2'){e=0;id='2';}
        else if(customKey == '3'){e=0;id='3';}
        else if(customKey == '4'){e=0;id='4';}
        else if(customKey == '5'){e=0;id='5';}
        else if(customKey == '6'){e=0;id='6';}
        else if(customKey == '7'){e=0;id='7';}
        else if(customKey == '8'){e=0;id='8';}
        else if(customKey == '9'){e=0;id='9';}
        }
        
            Serial.println(id);
            
              lcd.clear(); 
              Serial.print("Enrolling ID #");
              Serial.println(id);
             int p = -1;
            lcd.setCursor(0, 0);
            lcd.print("Waiting for");
            lcd.setCursor(1, 0);
            lcd.print("valid finger");
            while (p != FINGERPRINT_OK) {
             p = finger.getImage();
           switch (p) {
           case FINGERPRINT_OK:
           Serial.println("Image taken");
           lcd.clear(); 
           lcd.setCursor(0, 0);
           lcd.print("Image taken");
           break;
           case FINGERPRINT_NOFINGER:
           Serial.println(".");
           break;
           case FINGERPRINT_PACKETRECIEVEERR:
           Serial.println("Communication error");
           break;
           case FINGERPRINT_IMAGEFAIL:
           Serial.println("Imaging error");
           break;
           default:
          Serial.println("Unknown error");
          break;
          }
         }

  // OK success!

   p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }    
  lcd.clear();       
   lcd.setCursor(0, 0);
   lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
   lcd.clear(); 
     lcd.setCursor(0, 0);
   lcd.print("Place the");
       lcd.setCursor(1, 0);
       lcd.print("same finger");
 
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear(); 
       lcd.setCursor(0, 0);
       lcd.print("Image Stored");
       delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
     lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  } 
         
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Please input :");
         } 
     
             if (customKey == '2') 
          {
             s=0;
            inPut = "";
             lcd.clear();
         lcd.setCursor(0, 0);//格式列、行
         lcd.print("Please input ID:");
         int e=1;
           while (e）
        {
         customKey = customKeypad.getKey();
         Serial.println("while");
         if (customKey == '1'){e=0;id='1';}
        else if(customKey == '2'){e=0;id='2';}
        else if(customKey == '3'){e=0;id='3';}
        else if(customKey == '4'){e=0;id='4';}
        else if(customKey == '5'){e=0;id='5';}
        else if(customKey == '6'){e=0;id='6';}
        else if(customKey == '7'){e=0;id='7';}
        else if(customKey == '8'){e=0;id='8';}
        else if(customKey == '9'){e=0;id='9';}
        }
              uint8_t p = -1;
  
         p = finger.deleteModel(id);
  
        if (p == FINGERPRINT_OK){
        Serial.println("Deleted!");
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Deleted");
         delay(2000);}
          else if (p == FINGERPRINT_PACKETRECIEVEERR) {
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Error");
          return p;} 
         else if (p == FINGERPRINT_BADLOCATION) {
         Serial.println("Could not delete in that location");
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Error");
         return p; } 
         else if (p == FINGERPRINT_FLASHERR) {
         Serial.println("Error writing to flash");
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Error");
         return p;} else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;}
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Please input :");
            }


            
             if (customKey == '3') 
          {
            s=0;
            inPut = "";
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Please input :");
          }
        
      }
      }

      
       if (customKey == '2') 
        {
        a = 0;
        inPut = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please input :");
      }
    }
   }
    
   
   



   
else if (inPut.length() == 4)//Incorrect password
  {
    Serial.println("ERROR");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ERROR!");
    inPut = "";
    addNum++;//One error count
    Serial.print("addNum is : ");
    Serial.println(addNum);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please input :");
    if (addNum > 2)
  {
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("ERROR!");
     pinMode(pinBuzzer, OUTPUT); 
     long frequency = 300; //Frequency, unit Hz
     tone(pinBuzzer, frequency );
     delay(10000); //Wait 1000 milliseconds
    noTone(pinBuzzer);//Stop sounding
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wait for 30s");
    before = millis();
    now = millis();
    Serial.print("before is : ");
    Serial.println(before);
    Serial.print("now is : ");
    Serial.println(now);//Lock the keyboard and wait for 30 seconds
    while (now - before < delayTime)
    {
      lcd.setCursor(0, 0);
      lcd.print("Wait for   s");
      lcd.setCursor(9, 0);
      lcd.print((30000 - (now - before)) / 1000);
      now = millis();
      Serial.print("n-b= ");
      Serial.println(now - before);
      char key = customKeypad.getKey();
      if (key)//If there is a key press to reset the time
      {
        before = millis();
        now = millis();
        Serial.print("key~~~before is  ");
        Serial.println(now - before);
      }
    }
    addNum = 0;//When the locking time is up, the number of errors is cleared
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please input :");
  }
  }
  lcd.setCursor(0, 0);
    lcd.print("Please input :");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
        return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
        return p;
  }
  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
   return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
 p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    addNum = 0;
     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("  Door Unlocked");
     lcd.setCursor(1, 0);
     lcd.print("    Welcome");
     digitalWrite(relayPin, HIGH);
     delay(3000);
     digitalWrite(relayPin, LOW);
     lcd.clear();
    delay(500);  
     lcd.setCursor(0, 0);//格式列、行

    lcd.print("Please input :");
  }
  else if(p != FINGERPRINT_OK)
  {
    Serial.println("ERROR");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ERROR!");
    inPut = "";
    addNum++;
    Serial.print("addNum is : ");
    Serial.println(addNum);
    delay(1000);
    lcd.clear();
  }
 

  if (addNum > 2)
  {
     pinMode(pinBuzzer, OUTPUT); 
     long frequency = 300; 
     tone(pinBuzzer, frequency );
     delay(10000); 
noTone(pinBuzzer);
delay(2000); 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wait for 30s");
    before = millis();
    now = millis();
    Serial.print("before is : ");
    Serial.println(before);
    Serial.print("now is : ");
    Serial.println(now);
    while (now - before < delayTime)
    {
      lcd.setCursor(0, 0);
      lcd.print("Wait for   s");
      lcd.setCursor(9, 0);
      lcd.print((30000 - (now - before)) / 1000);
      now = millis();
      Serial.print("n-b= ");
      Serial.println(now - before);
      char key = customKeypad.getKey();
      if (key)
      {
        before = millis();
        now = millis();
        Serial.print("key~~~before is  ");
        Serial.println(now - before);
      }
    }
    addNum = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please input :");
  }
}
