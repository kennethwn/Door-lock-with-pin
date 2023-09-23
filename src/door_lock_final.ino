#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);

char password[4];
char initial_password[4],new_password[4];
int vcc=11;
int i=0;
int relay_pin = 11;
char key_pressed=0;
const byte rows = 4; 
const byte columns = 4; 
char hexaKeys[rows][columns] = {
  {'1','2','A','3'},
  {'4','5','B','6'},
  {'7','8','C','9'},
  {'*','0','D','#'}
};
byte row_pins[rows]={2,3,4,5};
byte column_pins[columns]={6,7,8,9};

Keypad keypad_key=Keypad( makeKeymap(hexaKeys),row_pins,column_pins,rows,columns);

void setup(){
  lcd.init();
  lcd.backlight();
  pinMode(relay_pin, OUTPUT);
  pinMode(vcc, OUTPUT);

  lcd.setCursor(0,0);
  lcd.print(" MOUNT DYNAMICS ");
  lcd.setCursor(0,1);
  lcd.print("Electronic Lock ");
  delay(3000);
  lcd.clear();
  initialpassword();
}

void loop(){
    digitalWrite(relay_pin,HIGH);

    key_pressed = keypad_key.getKey();
    if(key_pressed=='#') {
      change();
    }
    else if (key_pressed) {
      password[i++]=key_pressed;
      lcd.print(key_pressed);
    }
    else if(i==4) {
      delay(200);
      for(int j=0;j<4;j++) {
        initial_password[j]=EEPROM.read(j);
      }
    
      if(!(strncmp(password, initial_password,4))){
        lcd.clear();
        lcd.print("Pass Accepted");
        digitalWrite(relay_pin,LOW);
        delay(2000);
        lcd.setCursor(0,0);
        lcd.print("Pres >START< to");
        lcd.setCursor(0,1);
        lcd.print("change the pass");
        delay(3000);
        lcd.clear();
        lcd.print("Enter Password:");
        lcd.setCursor(0,1);
        i=0;
      } else{
        digitalWrite(relay_pin, HIGH);
        lcd.clear();
        lcd.print("Wrong Password");
        lcd.setCursor(0,0);
        lcd.print("Pres >#< to");
        lcd.setCursor(0,1);
        lcd.print("change the pass");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password");
        lcd.setCursor(0,1);
        i=0;
    }
  }
}


void change(){
  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4){
    char key=keypad_key.getKey();
    if(key){
      new_password[j++]=key;
      lcd.print(key);
    }
    key=0;
  }
  
  delay(500);

  if((strncmp(new_password, initial_password, 4))){
      lcd.clear();
      lcd.print("Wrong Password");
      lcd.setCursor(0,1);
      lcd.print("Try Again");
      delay(1000);
  } else {
      j=0;
      lcd.clear();
      lcd.print("New Password:");
      lcd.setCursor(0,1);

      while(j<4){
        char key=keypad_key.getKey();
        if(key){
          initial_password[j]=key;
          lcd.print(key);
          EEPROM.write(j,key);
          j++;
        }
      } 
      lcd.print("Pass Changed");
      delay(1000);
  }
 
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  key_pressed=0;
}

void initialpassword(){
  lcd.setCursor(0,0);
  lcd.print("Set Password");
  lcd.setCursor(0,1);
  int j;
  for(j=0;j<4;j++) {
    key_pressed = keypad_key.getKey();
    EEPROM.write(j,key_pressed);
  }
  for(j=0;j<4;j++) {
    initial_password[j]=EEPROM.read(j);
  }
}
