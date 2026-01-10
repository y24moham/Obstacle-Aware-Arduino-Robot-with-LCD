// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int trig = 8, echo = 9;
int motorR1 = 6, motorL1 = 7, motorR2 = 10, motorL2 = 13;
int buttonenter =A5;
bool state=0;

void stop();
void fw();
void bw();
int usdistcm();
bool checkbutton(int button);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  pinMode(motorL1,OUTPUT);
  pinMode(motorR1,OUTPUT);
  pinMode(motorL2,OUTPUT);
  pinMode(motorR2,OUTPUT);

  pinMode(buttonenter,INPUT);


}


void loop() {
  
  if (state==0){
    stop();

  int distance_cm = usdistcm();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Press ENTER");

  while (checkbutton(buttonenter)==0){
    delay(50);
  }
  
  state = 1;
  }
  
  lcd.clear();
  fw();


  lcd.setCursor(0, 0);
  lcd.print(usdistcm());


  if (usdistcm()<10){
    delay(100);
  }
  while (usdistcm()<=7){
    RR();
    lcd.clear();
    lcd.print("turning right");
    delay(500);
  }

  if (checkbutton(buttonenter)==1){
    state = 0;

  }
  
  //delay(200);

}


void stop(){
  digitalWrite(motorL1,LOW);
  digitalWrite(motorL2,LOW);
  digitalWrite(motorR2,LOW);
  digitalWrite(motorR1,LOW);
  
}

void fw(){
  digitalWrite(motorL1,HIGH);
  digitalWrite(motorL2,LOW);
  digitalWrite(motorR2,HIGH);
  digitalWrite(motorR1,LOW);
  
}

void bw(){
  digitalWrite(motorL1,LOW);
  digitalWrite(motorL2,HIGH);
  digitalWrite(motorR2,LOW);
  digitalWrite(motorR1,HIGH);
  
}

void RR(){
  digitalWrite(motorL1,HIGH);
  digitalWrite(motorL2,LOW);
  digitalWrite(motorR2,LOW);
  digitalWrite(motorR1,HIGH);
  
}


int usdistcm(){
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  return 0.017*pulseIn(echo,HIGH);
}

bool checkbutton(int button){
  if (digitalRead(buttonenter)==1)
    delay(2);
  if (digitalRead(buttonenter)==1){
    while (digitalRead(buttonenter)==HIGH){
      delay(50);
    }
  return 1;

  }
  else return 0;

}

