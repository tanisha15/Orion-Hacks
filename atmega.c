#include<LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>

//pin declarations of push buttons:
const int enter = 12;
const int back = 11;
const int down = 10;
const int up = 9;
//pin declarations for Motor
const int in1 = 2;
const int in2 = 1;
//pin delcarations of indicators
const int buzz = 6; 
const int led = 5;

const int Menu = 1;
const int Settings = 2;
const int Start = 3;

int section = 1;
byte buttonStatus = 0;
int userID = 1;

int dispScreen = 1;
int dispUpdate = 0;

int flip = 0;


uint8_t getFingerprintEnroll(int id);
int getFingerprintIDez();

int newFinger = 1;
int finger_idz = 0;
int no_finger = 1;
volatile int id = 0;
boolean match = false;
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//Function declaration
int readButtons();
void enrollUser();
void doorOpen();
void doorClose();

//Create Class instances
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

void setup() 
{
//declare pin modes:
pinMode(enter,INPUT);
pinMode(back,INPUT);
pinMode(up,INPUT);
pinMode(down,INPUT);

pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);

pinMode(buzz,OUTPUT);
pinMode(led,OUTPUT);

digitalWrite(buzz,LOW);
digitalWrite(led,LOW);

lcd.begin(16,2);
lcd.clear();


finger.begin(9600);
if (finger.verifyPassword()) 
{
  lcd.print("Found Sensor");
  delay(1000);
} 
else 
{
  lcd.print(" Connect Sensor ");
  lcd.setCursor(0,1);
  lcd.print("&  Press Reset  ");
  while (1);
}
lcd.clear();
lcd.print("Initializing...");
id = 0;
while(id < 99)
{
  deleteFingerprint(id);
  id++;
}
id=1;
lcd.clear();
lcd.print("FingerPrint Hall");
lcd.setCursor(0,1);
lcd.print("Verification Sys");
delay(1000);



lcd.clear();
lcd.print("-> 1. Enroll");
lcd.setCursor(0,1);
lcd.print("   2. Start");
}

void loop() 
{
  while(section == Menu)
  {
    switch(dispScreen)
    {
      case 1:
        lcd.clear();
        lcd.print("-> 1. Enroll");
        lcd.setCursor(0,1);
        lcd.print("   2. Start");
     //   dispUpdate = 1;
        break;

      case 2:
        lcd.clear();
        lcd.print("   1. Enroll");
        lcd.setCursor(0,1);
        lcd.print("-> 2. Start");
      //  dispUpdate = 1;
    }
    buttonStatus = 0;
    while(buttonStatus == readButtons());
    switch(buttonStatus)
    {
      case 1:
        if(dispScreen == 1)
        section = Settings;
        else
        section = Start;
        break;

      case 2:
        break;

      case 3:
        dispScreen = 1;
        dispUpdate = 0;
        break;

      case 4:
        dispScreen = 2;
        dispUpdate = 0;
        break;

      default:
        break;
    }

    switch(dispScreen)
    {
      case 1:
        lcd.clear();
        lcd.print("-> 1. Enroll");
        lcd.setCursor(0,1);
        lcd.print("   2. Start");
     //   dispUpdate = 1;
        break;

      case 2:
        lcd.clear();
        lcd.print("   1. Enroll");
        lcd.setCursor(0,1);
        lcd.print("-> 2. Start");
      //  dispUpdate = 1;
    }
  }

  while(section == Settings)
  {
    lcd.clear();
    lcd.print("Press Enter to");
    lcd.setCursor(0,1);
    lcd.print("Register Finger");
    buttonStatus = 0;
    while(readButtons()== 0);
    switch(buttonStatus)
    {
      case 1:
        if(id < 99)
        {
          if(match)
          {
            id++;
            match = false;
          }
          enrollUser();
        }
        else
        {
          lcd.clear();
          lcd.print(" Register Limit ");
          lcd.setCursor(0,1);
          lcd.print(" 20 Candidates  ");
        }
        break;

      case 2:
        section = Menu;
        break;

      default :
        break;
    }
  }

  while(section == Start)
  {
    lcd.clear();
    lcd.print(" Press Enter  & ");
    lcd.setCursor(0,1);
    lcd.print("  Place Finger  ");
    buttonStatus = 0;
    while(readButtons() == 0);
    switch(buttonStatus)
    {
      case 1:
        no_finger = 1;
        while(no_finger)
        {
          finger_idz = getFingerprintIDez();
        }
        if(finger_idz == -1)
        {
          lcd.clear();
          lcd.print(" Candidate Not  ");
          lcd.setCursor(0,1);
          lcd.print("  Authorized    ");
        }
        else
        { 
          lcd.clear();
          lcd.print(" Candidate ID ");
          lcd.print(finger_idz);
          lcd.setCursor(0,1);
          lcd.print(" Authenticated  ");
          digitalWrite(buzz,HIGH);
          digitalWrite(led,HIGH);
          doorOpen();
          delay(2000);
          digitalWrite(buzz,LOW);
          digitalWrite(led,LOW);
          delay(3000);
          doorClose();
          delay(2000);
        }
        digitalWrite(in1,LOW);
        digitalWrite(in2,LOW);   
        delay(5000);
        newFinger = 1;
      
        break;
        
      case 2:
        section = Menu;
        break;
        
      default:
        break; 
    }
    
  }
}


//////Function definations-------------------------

int readButtons()
{
  if(!(digitalRead(enter)))
  {
    while(!(digitalRead(enter)));
    buttonStatus = 1;
  }
  
  else if(!(digitalRead(back)))
  {
    while(!(digitalRead(back)));
    buttonStatus = 2;
  }

  else if(!(digitalRead(up)))
  {
    while(!(digitalRead(up)));
    buttonStatus = 3;
  }

  else if(!(digitalRead(down)))
  {
    while(!(digitalRead(down)));
    buttonStatus = 4;
  }
  else
  {
    buttonStatus = 0;
  }

  return buttonStatus;
}

void enrollUser()
{
  
  //delay(1000);
  lcd.clear();
  lcd.print("RegistrCandidate");
  lcd.print("As ID: ");
  lcd.print(id);
  delay(1000);

  no_finger = 1;
    while(no_finger)
    {
      finger_idz = getFingerprintIDez();
    }
    if(finger_idz == -1)
    while(! getFingerprintEnroll(id));
    else
   { 
      lcd.clear();
      lcd.print("Already Regtd ID");
      lcd.setCursor(0,1);
      lcd.print("#");
      lcd.print(finger_id);
   //   id--;
   }
    delay(5000);
    newFinger = 1;
}

void doorOpen()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
}

void doorClose()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
}


uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("  Image taken  ");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(16,1);
      if(flip == 0)
      {
        lcd.print("/");
        flip = 1;
      }
      else
      {
        lcd.print("\\");
        flip = 0;
      }
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
     // lcd.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
    //  lcd.println("Imaging error");
      break;
    default:
     // lcd.println("Unknown error");
      break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(500);
      break;
    case FINGERPRINT_IMAGEMESS:
     // lcd.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    //  lcd.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
     // lcd.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
     // lcd.println("Could not find fingerprint features");
      return p;
    default:
    //  lcd.println("Unknown error");
      return p;
  }
  lcd.clear();
  lcd.print(" Remove Finger ");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.clear();
  lcd.print("PlaceSame Finger");
  lcd.setCursor(0,1);
  lcd.print("Again..");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("   Image taken  ");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.setCursor(16,1);
      if(flip == 0)
      {
        lcd.print("/");
        flip = 1;
      }
      else
      {
        lcd.print("\\");
        flip = 0;
      }
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //lcd.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //lcd.println("Imaging error");
      break;
    default:
      //lcd.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //lcd.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //lcd.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
     // lcd.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
     // lcd.println("Could not find fingerprint features");
      return p;
    default:
     // lcd.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Prints matched!");
   // id++;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   // lcd.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("Fingerprints did");
    lcd.setCursor(0,1);
    lcd.print("not match");
    return p;
  } else {
    lcd.println("Unknown error");
    return p;
  }   

  delay(1000);  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
  lcd.clear();
  lcd.print("Candidate Regtd "); 
  lcd.setCursor(0,1);
  lcd.print("As ID: ");
  lcd.print(id);
  match = true;
  delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //lcd.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //lcd.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //lcd.println("Error writing to flash");
    return p;
  } else {
    //lcd.println("Unknown error");
    return p;
  }   
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      no_finger = 0;
      lcd.clear();
      lcd.print("  Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("NoFingerDetected");
      no_finger = 1;
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //lcd.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //lcd.println("Imaging error");
      return p;
    default:
      //lcd.println("Unknown error");
      return p;
  }


  }
  
  
  // found a match!
  lcd.clear();
  lcd.print("Found ID #"); 
  lcd.print(finger.fingerID); 
  delay(1000);
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  else no_finger = 0;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  lcd.clear();
  lcd.print("Found ID #"); lcd.print(finger.fingerID); 
//  lcd.print(" with confidence of "); lcd.println(finger.confidence);
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
   // lcd.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  //  lcd.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
   // lcd.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
   // lcd.println("Error writing to flash");
    return p;
  } else {
   // lcd.print("Unknown error: 0x"); lcd.println(p, HEX);
    return p;
  }   
}
