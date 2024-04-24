#include <Servo.h>
#include <Keypad.h>
#include<EEPROM.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

Servo myservo; //craete servo object to control a servo
char password[4];
char initial_password[4],new_password[4];
int i=0;
int led=12;
int angle=0;
char key_pressed=0;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {11,10,8,7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,5,4,2};//connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
    //Setup Channel A
pinMode(12, OUTPUT); //Initiates Motor Channel A pin
pinMode(9, OUTPUT); //Initiates Brake Channel A pin
pinMode(3,OUTPUT);
myservo.attach(9); // attaches the servo on pin 9 to the servo object  
pinMode(led,OUTPUT);
initialpassword();
Serial.begin(9600);
lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("WELLCOME");
delay(10000);
lcd.clear();
lcd.begin(16,2);
lcd.setCursor(0,0);
lcd.print("Enter Password");
lcd.setCursor(0,1);
}
  void loop(){
  digitalWrite(10,LOW);
  key_pressed = keypad_key.getKey();
 if(key_pressed=='#')
change();


if (key_pressed)
{
password[i++]=key_pressed;
lcd.print(key_pressed);
}
if(i==4)
{
delay(200);
for(int j=0;j<4;j++)
initial_password[j]=EEPROM.read(j);
if(!(strncmp(password, initial_password,4)))
{
lcd.clear();
lcd.print("Pass Accepted");
digitalWrite(12,HIGH);
delay(2000);
digitalWrite(12,LOW);
//forward @ full speed
digitalWrite(12, HIGH); //Establishes forward direction of Channel A
digitalWrite(9, LOW);   //Disengage the Brake for Channel A
analogWrite(3, 230);   //Spins the motor on Channel A at full speed
  delay(1000);
  digitalWrite(9, HIGH); //Eengage the Brake for Channel A
delay(1000);
  
  //backward @ half speed
digitalWrite(12, LOW); //Establishes backward direction of Channel A
digitalWrite(9, LOW);   //Disengage the Brake for Channel A
analogWrite(3, 230);   //Spins the motor on Channel A at half speed
  delay(1000);
  digitalWrite(9, HIGH); //Eengage the Brake for Channel A
  
  //delay(1000);
  for(angle = 0; angle < 180; angle += 1)// command to move from 0 degrees to 180 degrees 
  { 
  myservo.write(angle);//command to rotate the servo to the specified angle
  delay(15);
  }
  delay(1000);
  
  for(angle = 180; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
  {                                
    myservo.write(angle);              //command to rotate the servo to the specified angle
    delay(5);                       
  } 


lcd.clear();
lcd.print("Enter Password:");
lcd.setCursor(0,1);
i=0;

}

else
{
lcd.clear();
lcd.print("Access denied");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Pres # to change");
delay(2000);
lcd.clear();
lcd.print("Enter Password:");
lcd.setCursor(0,1);
i=0;


}
}
}


void change()
{
int j=0;
lcd.clear();
lcd.print("Current Password");
lcd.setCursor(0,1);
while(j<4)
{
char key=keypad_key.getKey();
if(key)
{
new_password[j++]=key;
lcd.print(key);
}
key=0;
}
delay(500);
if((strncmp(new_password, initial_password, 4)))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Try Again");
delay(1000);

}
else
{
j=0;
lcd.clear();
lcd.print("New Password:");
lcd.setCursor(0,1);

delay(1000);

while(j<4)
{
char key=keypad_key.getKey();
if(key)
{
initial_password[j]=key;
lcd.print(key);
EEPROM.write(j,key);
j++;
}
}
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Pass Changed");
delay(1000);

}
lcd.clear();
lcd.print("Enter Password");
lcd.setCursor(0,1);
key_pressed=0;

}

void initialpassword()
{
for(int j=0;j<4;j++)
EEPROM.write(j, j+49);
for(int j=0;j<4;j++)
initial_password[j]=EEPROM.read(j);

}








 





 


