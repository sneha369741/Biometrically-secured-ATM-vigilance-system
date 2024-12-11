
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<EEPROM.h>
#include<LiquidCrystal.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
SoftwareSerial fingerPrint(10, 11);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

uint8_t getFingerprintEnroll() ;
char pin1[10]={"1234"};

char code[10],buff1[20],sum[100]; 
#define Password_Length 5
//char Data[Password_Length];
byte data_count = 0, master_count = 0;  
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char textmessage[60];
char keybuf[6],msg_buff[100];

char buff[4];
char Data[Password_Length]; 
char Master[Password_Length] = "1234"; 
//byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey=0;
String message="";
char ch;
const byte ROWS = 4;
const byte COLS = 4;
char one,two,three,messege;
//char hexaKeys[ROWS][COLS] = {
//  {'1', '2', '3', 'A'},
//  {'4', '5', '6', 'B'},
//  {'7', '8', '9', 'C'},
//  {'*', '0', '#', 'D'}
//};

char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', '$'}
};

//byte rowPins[ROWS] = {10, 11, 14, 15};
//byte colPins[COLS] = {16, 17, 18, 19};
byte colPins[COLS] ={36, 34, 32, 30};
byte rowPins[ROWS] ={28, 26, 24, 22}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int key_num = 0,m=0,l=1,z=0,x=0;
void RFID_CHECKING();
char key=0,amountbuf[20],amountbuf1[40], buff_rfid[10];

int i,str_len;
uint8_t id;

int w=0,y=0;

int total_amount = 10000,amount_temp;
#define uart0_putc(x) Serial1.write(x)

#define BUFSIZE    0x10
char get_char_Finger(void);
int FINGER_SCAN(void);
void FINGER_ENROL(void);
void bank_selection();
int var_1,  var_2;
int UART0Count = 0;
char UART0Buffer[BUFSIZE];
int c_1=0;
int cnt1, cnt2;
int ID;
int c=0;


#define enroll 38
#define del 40
#define up 42
#define down 44


int Buzzer=7;
int IN1=8;
int IN2=9;

#define records 25

void cash_handling(void);
void cash_handling1(void);
void setup()
{
 

 
  Serial.begin(9600);
 
    Serial1.begin(9600);
//    Serial2.begin(9600);
        finger.begin(9600);
//    lcd.begin(16,2);
    pinMode(IN1,OUTPUT);
     pinMode(IN2,OUTPUT);
     pinMode(Buzzer,OUTPUT);
     
     pinMode(enroll, INPUT_PULLUP);
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);
    pinMode(del, INPUT_PULLUP);

    digitalWrite(IN1,LOW);
     digitalWrite(IN2,LOW);
     digitalWrite(Buzzer,LOW);
    
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MULTI ACCONT      ");
  lcd.setCursor(0,1);
  lcd.print("ATM SYSTEM  ");
  
  delay(2000);
 

}
void loop()
{
  START();
}

void Door_Open()
{
  lcd.clear();
  lcd.print("Door Open");
  delay(1000);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  delay(1000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  delay(1000);
 
}
void Door_Close()
{
  lcd.clear();
  lcd.print("Door Close");
  delay(1000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  delay(1000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  delay(1000);
 
}

void START()
{
  Door_Open();
  delay(2000);
  Door_Close();
  
   while(1)
   {
     lcd.setCursor(0,0);
     lcd.print("Press UP/Down ");
     lcd.setCursor(0,1);
     lcd.print("to start System");
     if(digitalRead(up)==0 || digitalRead(down)==0)
     {
      while(1)
      {
        main_function();    
      }
     }
     checkKeys();
     delay(1000);
   
   
   }  


}
int main_function (void)
{
   
    RFID_CHECKING();
 
}     
void clearData()
{
  while(data_count !=0)
  {
    Data[data_count--] = 0; 
  }
  return;
}


void RFID_CHECKING()
{ 
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCAN YOUR ");
    lcd.setCursor(0,1);
    lcd.print("CARD......   ");
 while(1)
 {
    if(Serial1.available()>0)
    {
     
      message=Serial1.readString();
      int str_len = message.length() + 1;
      char textmessage[12];
      message.toCharArray(textmessage,str_len);
      Serial.println(textmessage);
      textmessage[12]='\0';
      if((strcmp(textmessage,"54006E2C091F"))==0)
      { 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("VALID CARD");
        
          delay(1000);
          PASSWARD();
      
         // break;
      }
      else
      {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("INVALID CARD");
          digitalWrite(Buzzer,HIGH);
          delay(1000);
           digitalWrite(Buzzer,LOW);
        
      }
       
    }
 }
}


void PASSWARD()
{ 

            
  cnt1=0;
          l=1;
          m=0;   
          while(l)
         {
          while(cnt1<3)
          {
            lcd.clear();
            lcd.print("ENTER YOUR  ");
            lcd.setCursor(0,1);
            lcd.print("PIN:");
            
            
            while(m<4)
            {
              customKey = customKeypad.getKey();
              if(customKey)
              {
                 lcd.print(customKey);
                 keybuf[m++]=customKey;
              }
            }
            
            keybuf[4]=0;  
            l=0;
            m=0;
            
            w=strcmp(keybuf, pin1);
            if(w==0)
            {
    
                lcd.clear();
              lcd.print("VALID PIN ");
              lcd.setCursor(0,1);
              lcd.print("    ");
               delay(2000); 
              l=1;
               m=0;
               
              lcd.clear();
              lcd.print("WAIT FINGER  ");
              lcd.setCursor(0,1);
              lcd.print("AUTHENTICATING.. ");
              delay(1000); 
              PEOPLE_SERACHING();
//              lcd.clear();
//              lcd.print("WEL COME TO ATM    ");
//              lcd.setCursor(0,1);
//              lcd.print("NAME ");
//              delay(1000);
              lcd.clear();
              lcd.print("WAIT  ");
              lcd.setCursor(0,1);
              lcd.print("AUTHENTICATING   ");
           
              Serial.println("$YOUR CARD HAS ACCESSED IN ATM , SEND RANDOM PASSWORD# " );
              delay(1000);
              cash_handling1();
              } 
          else
            {
              lcd.clear();
              lcd.print("INVALID PIN    ");
              lcd.setCursor(0,1);
              lcd.print("ENTER PIN AGAIN    ");
          
              cnt1++;
              delay(1000);
               lcd.clear();
              lcd.print("WRONG PIN      ");
               lcd.setCursor(0,1);
               lcd.print("TRANS DECLINED       ");
             delay(1000);
            }

        }
        digitalWrite(Buzzer,HIGH);
          delay(1000);
           digitalWrite(Buzzer,LOW);

       }
}
void PEOPLE_SERACHING(void)
{

     for(int i=0;i<5;i++)
       {
    lcd.clear();
    lcd.print("Place Finger");
    
    delay(2000);
    int result=getFingerprintIDez();
    if(result>=0)
    Serial.println(result);
    if(result>=0&&result<8)
    {
      if(result==1)
      {
    
        lcd.clear();
        lcd.print("FINGER1 MATCHED");
        delay(1000);
        cash_handling();
        break;
      }
      if(result==2)
      {
    
        lcd.clear();
        lcd.print("FINGER2 MATCHED");
        delay(1000);
        cash_handling();
        break;
      }
       if(result==3)
      {
    
        lcd.clear();
        lcd.print("FINGER3 MATCHED");
        delay(1000);
        cash_handling();
        break;
      }
       if(result==4)
      {
    
        lcd.clear();
        lcd.print("FINGER4 MATCHED");
        delay(1000);
        cash_handling();
        break;
      }
      if(result==0)
      {
    
        lcd.clear();
        lcd.print("FINGER NOT");
        lcd.setCursor(0,1);
        lcd.print("MATCHED");
        digitalWrite(Buzzer,HIGH);
          delay(1000);
           digitalWrite(Buzzer,LOW);
        break;
        
      }
     
     }
      
     }
}
void checkKeys()
{
   if(digitalRead(enroll) == 0)
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while(digitalRead(enroll) == 0);
    Enroll();
   }
 
   else if(digitalRead(del) == 0)
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
   }  
}

void Enroll()
{
   int count=0;
   lcd.clear();
   lcd.print("Enter Finger ID:");
   
   while(1)
   {
    lcd.setCursor(0,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }
 
     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          getFingerprintEnroll();
          for(int i=0;i<records;i++)
          {
            if(EEPROM.read(i+10) == 0xff)
            {
              EEPROM.write(i+10, id);
              break;
            }
          }
          return;
     }
 
       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}
 
void delet()
{
   int count=0;
   lcd.clear();
   lcd.print("Enter Finger ID");
   
   while(1)
   {
    lcd.setCursor(0,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }
 
     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          deleteFingerprint(id);
          for(int i=0;i<records;i++)
          {
            if(EEPROM.read(i+10) == id)
            {
              EEPROM.write(i+10, 0xff);
              break;
            }
          }
          return;
     }
       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}
 
uint8_t getFingerprintEnroll()
{
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No Finger");
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      //Serial.println("Unknown error");
       lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }
 
  // OK success!
 
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
       lcd.clear();
       lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
            lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
            lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
                  lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      //Serial.println("Unknown error");
                  lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }
 
  //Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); //Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
   lcd.clear();
      lcd.print("Place Finger");
      lcd.setCursor(0,1);
      lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      return;
    }
  }
 
  // OK success!
 
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
 
  // OK converted!
  //Serial.print("Creating model for #");  //Serial.println(id);
 
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }  
 
  //Serial.print("ID "); //Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  }
  else {
    //Serial.println("Unknown error");
    return p;
  }  
}
 
 
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
 
  if (p != FINGERPRINT_OK)  
  return -1;
 
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;
 
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
   lcd.clear();
   lcd.print("Finger Not Found");
   lcd.setCursor(0,1);
   lcd.print("Try Later");
   delay(2000);  
  return 0;
  }
  // found a match!
//  Serial.print("Found ID #");
 // Serial.print(finger.fingerID);
 while(1)
 {
    switch(finger.fingerID)
       {
      case 1:lcd.clear();
             lcd.print("NAME:VALID FINGER");
           
              delay(50);
//              digitalWrite(door,LOW);  
             return finger.fingerID;
             break;

       case 2:lcd.clear();
             lcd.print("NAME:BBBB");
             lcd.setCursor(0,1);
             lcd.print("AGE:21");
//               digitalWrite(door,HIGH);
//              delay(50);
//                digitalWrite(door,LOW);   
             return finger.fingerID;
             break;
//
      case 3:lcd.clear();
             lcd.print("NAME:CCCC");
             lcd.setCursor(0,1);
             lcd.print("AGE:22");
//              digitalWrite(door,HIGH);
//             delay(50);  
//              digitalWrite(door,LOW);  
             return finger.fingerID;
             break;

      case 4:lcd.clear();
             lcd.print("NAME:DDDDD");
             lcd.setCursor(0,1);
             lcd.print("AGE:23");
//              digitalWrite(door,HIGH);
//             delay(50);
//              digitalWrite(door,LOW);    
             return finger.fingerID;
             break;
//
//       case 5:lcd.clear();
//             lcd.print("CCCCCC");
//             lcd.setCursor(0,1);
//             lcd.print("AGE:24");
////              digitalWrite(door,HIGH);
////             delay(50); 
////              digitalWrite(door,LOW);   
//             return finger.fingerID;
//             break;
//
//       case 6:lcd.clear();
//             lcd.print("DDDDDD");
//             lcd.setCursor(0,1);
//             lcd.print("AGE:25");
////              digitalWrite(door,HIGH);
////             delay(50);
////              digitalWrite(door,LOW);  
//             return finger.fingerID;
//             break;
//
//       case 7:lcd.clear();
//             lcd.print("EEEEE");
//             lcd.setCursor(0,1);
//             lcd.print("AGE:26");
////             digitalWrite(door,HIGH);
////             delay(50);
////              digitalWrite(door,LOW);  
//             return finger.fingerID;
//             break;
       
  }  
 }
  return finger.fingerID;
}
 
uint8_t deleteFingerprint(uint8_t id)
{
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK)
  {
    //Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Finger Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
  }
 
  else
  {
    //Serial.print("Something Wrong");
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }  
}

void cash_handling(void)
{    
     l=1;
     m=0;
     bank_selection();
     lcd.clear();
     lcd.print("ENTER AMOUNT:    ");
     lcd.setCursor(0,1);
    

       l=1;
       m=0;
       while(l)
       {
          customKey = customKeypad.getKey();
          if (customKey)
          {
       
             if(customKey != '$')
            {
              lcd.print(customKey);
              
            }
            amountbuf[m++]=customKey;
            if(customKey=='$')
            {   m--;
              amountbuf[m]='\0';
              l=0;
            } 
          }
       }
          amount_temp = atoi((char *)amountbuf);
       if(amount_temp < total_amount)
      {
          l=1;
          z=0;
          lcd.clear();
           lcd.print("PLEASE COLLECT  ");
           lcd.setCursor(0,1);
           lcd.print(" YOUR MONEY  ");
          delay(1000);
           lcd.clear();
           lcd.print("TRANSACTION OVR  ");
           lcd.setCursor(0,1);
           lcd.print(" THANK YOU   ");  
       
          
          amount_temp = atoi(amountbuf);
      
          total_amount = total_amount - amount_temp;


          String one =   "BALANCE:";
        //  String two = amountbuf; 
        
          String message = one + total_amount;
          int str_len = message.length() + 1;
          char textmessage[str_len];
          message.toCharArray(textmessage,str_len);      
           Serial.print("$");
          Serial.print(textmessage);  
          Serial.println("#");     
          
           lcd.clear();
           lcd.print(" THANK YOU        ");
           Serial.println("$THANK YOU#");
           lcd.clear();
           lcd.print(textmessage);
                
           delay(1000);
          l=0;
          main_function();
      
        } 
      else
      {
      
               Serial.println("$SORRY, YOU HAVE INSUFFICIENT BALANCE#");
                lcd.clear();
                lcd.print("sorry, You have ");
                lcd.setCursor(0,1);
                lcd.print("insufficient balance"); 
                delay(1000);
                main_function();
      }
}
void cash_handling1(void)
{    
    l=1;
     m=0;
     z=1;
     while(l)
     { 
          lcd.clear();
          lcd.print("WAITING FOR CLIE   ");
          lcd.setCursor(0,1);
          lcd.print("NT OTP....  ");
          MESSAGE_RECEIVE_PERMISSION();
          delay(1000);
          m=0;
          if(z==1)
          {
                z=0;
                lcd.clear();
                lcd.print("ENTER CLIENT    ");
                lcd.setCursor(0,1);
                lcd.print("OTP:  ");
                while(l)
                {
                  customKey = customKeypad.getKey();
                  if(customKey)
                 {
                       lcd.print(customKey);
                       
                        keybuf[m++]=customKey;
                        //delay(1000);
                        if(m==4)
                       {
                           keybuf[m]='\0';
                            l=0;
                        }
                  }
                   //delay(1000);
                }
               while(1 )
              { 
                if((strcmp((char *)msg_buff,keybuf))==0)
     
                {
                      bank_selection();
                       lcd.clear();
                      lcd.print("ENTER AMOUNT:");
                      lcd.setCursor(0,1);

                 
                  
                         l=1;
                         m=0;
                         while(l)
                         {
                            customKey = customKeypad.getKey();
                            if (customKey)
                            {
                       
                               if(customKey != '$')
                              {
                                lcd.print(customKey);
                                
                              }
                              amountbuf[m++]=customKey;
                              if(customKey=='$')
                              {   m--;
                                amountbuf[m]='\0';
                                l=0;
                              } 
                            }
                         }
              
                 amount_temp = atoi(amountbuf);
                 if(amount_temp < total_amount)
                 {
                  lcd.clear();
                lcd.print("WAIT...      ");
                lcd.setCursor(0,1);
                lcd.print("AUTHENTICATING..  ");



          String one =   "AMOUNT ENTERED:";
          String two = amountbuf; 
          // String three = " PERMISION YES OR NO";
          String message = one +two;
          int str_len = message.length() + 1;
          char textmessage[str_len];
          message.toCharArray(textmessage,str_len);
          Serial.print("$");
          Serial.print(textmessage);  
          Serial.println("#");      
          delay(2000);
      
          l=1;
          z=0;    
          while(l)
          {  
                
                 if(z==0)
                {
                  
                      z=1;
                     lcd.clear();
                     lcd.print("  PLEASE COLLECT  ");
                     lcd.setCursor(0,1);
                      lcd.print(" YOUR MONEY  ");
                     delay(1000);
                     lcd.clear();
                     lcd.print(" TRANSACTION OVR  ");
                     lcd.setCursor(0,1);
                      lcd.print("THANK YOU  ");
                     delay(1000);
                     amount_temp = atoi((char *)amountbuf);
      
                     total_amount = total_amount - amount_temp;
           
                    String one =   "BALANCE:";
                    //String two = total_amount; 
                  
                    String message = one + total_amount;
                    int str_len = message.length() + 1;
                    char textmessage[str_len];
                    message.toCharArray(textmessage,str_len);
                    Serial.print("$");
                    Serial.print(textmessage);   
                    Serial.println("#");     
                    Serial.println("$THANK YOU#");
                    lcd.clear();
                    lcd.print(textmessage);
                
                     delay(1000);
                 
                    l=0;
                    z=0;
                    main_function();
                  
              }   
            }
          }
          else
          {  
           Serial.println("$sorry, You have insufficient balance#");
           lcd.clear();
           lcd.print("sorry, You have ");
           lcd.setCursor(0,1);
           lcd.print("insufficient balance");
           delay(1000);
          }
     
           main_function();
        }

        else
        { 
           lcd.clear();
           lcd.print("WRONG  ");
           lcd.setCursor(0,1);
           lcd.print("PASSWORD  ");
           delay(1000);
     
            main_function();
        } 
      }
    }
  }            

}
char Serial_read(void)
{
      char ch;
      while(Serial.available() == 0);
      ch = Serial.read(); 
      return ch;
}
void MESSAGE_RECEIVE_PERMISSION()   
{
 Serial1.end();
// finger.end();
 Serial.begin(9600);
     while(Serial.available()>0);
      {
        
          while(Serial_read()!='*');
          i=0;                                               
          while((ch=Serial_read())!='#')
          {
            msg_buff[i] = ch;
             i++;
          }  
          msg_buff[i]='\0';
       }
     Serial.println(msg_buff);

     delay(1000);

}
void MESSAGE_RECEIVE_PERMISSION1()   
{
 
     while(Serial.available()>0);
      {
        
          while(Serial_read()!='*');
          i=0;                                               
          while((ch=Serial_read())!='#')
          {
            msg_buff[i] = ch;
             i++;
          }  
          msg_buff[i]='\0';
       }
     Serial.println(msg_buff);

     delay(1000);

}



void bank_selection()
{

   lcd.clear();
   lcd.print("  SELECT YOUR  ");
   lcd.setCursor(0,1);
   lcd.print("    BANKS    ");
   delay(1000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("1: SBI   2: SBM ");
   lcd.setCursor(0,1);
   lcd.print("3: KAR   4: IND ");
    
   customKey = customKeypad.getKey();
    while(!customKey)
    {
       customKey = customKeypad.getKey();
       //lcd.print(customKey);
      // keybuf[m++]=customKey;
    }

    if(customKey == '1')
    {
      lcd.clear();
      lcd.print(" WELCOME TO SBI  ");
      lcd.setCursor(0,1);
      lcd.print("     BANK    ");
      delay(1000);
    }
    else if(customKey == '2')
    {
     lcd.clear();
      lcd.print(" WELCOME TO SBM    ");
      lcd.setCursor(0,1);
      lcd.print("     BANK    ");
      delay(1000);
    }
  
    else if(customKey == '3')
    {
      lcd.clear();
      lcd.print(" WELCOME TO KAR    ");
      lcd.setCursor(0,1);
      lcd.print("     BANK    ");
      delay(1000);
    }

    else if(customKey == '4')
    {
      lcd.clear();
      lcd.print(" WELCOME TO IND    ");
      lcd.setCursor(0,1);
      lcd.print("     BANK    ");
      delay(1000);
    }

}
void buff_clear()
{
  for(int i=0;i<4;i++)
  buff[i]=0;
}
