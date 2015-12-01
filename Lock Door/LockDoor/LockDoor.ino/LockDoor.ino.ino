#include <Password.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h> 


#define NOTE_AS2 117
#define NOTE_G3  196
#define NOTE_DS8 4978
int notice =5;
int count=0;
int speakerPin = A3;
int timeWaitWrong=5000;
int noteDurations = 4;
//12,11,10->RS,RW,E
//9,8,7,6->D4,D5,D6,D7
LiquidCrystal lcd(12,11,10,9,8,7,6);
Password password = Password( "6969" );
Servo myservo;
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3',},
  {'4','5','6',},
  {'7','8','9',},
  {'*','0',' ',}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {5,3,4, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0, A1, A2}; //connect to the column pinouts of the keypad

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledPin 13
#define ledError A5
void setup(){
  myservo.attach(A4);
  myservo.write(0);
  lcd.begin(16, 2);
  digitalWrite(ledError,LOW);
  digitalWrite(ledPin, LOW);   
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  lcd.print("Enter PIN: ");
  
}

void loop(){  
  keypad.getKey();
}

//take care of some special events
void keypadEvent(KeypadEvent eKey){
      switch (keypad.getState()){
        case PRESSED:        	
          Serial.print("Press: ");
          Serial.println(eKey);
        	switch (eKey){
        	  case ' ': guessPassword();break;
        	   default:
               lcd.print("*");
        		   password.append(eKey);
           }
      }


}
void guessPassword(){
     if (password.evaluate()){
       timeWaitWrong=5000;
       count=0;
	     digitalWrite(ledPin,HIGH); //activates garaged door relay
       lcd.clear();   
	     lcd.print("VALID PASSWORD "); //
              password.reset(); //resets password after correct entry
              myservo.write(90);
              tone(speakerPin, NOTE_DS8,1000/4);             
              delay(600);
              lcd.setCursor(0,1);
              lcd.print("Welcome 13TCLC! ");              
              delay(6000);
              lcd.clear();
              digitalWrite(ledPin,LOW);
              myservo.write(0);
              lcd.print("Enter PIN: ");              
              noTone(speakerPin);
     }
     else{
       password.reset();
       count++;
      digitalWrite(ledError,HIGH);
      lcd.setCursor(0,1);
      lcd.print("INVALID PASSWORD ");
               
      if(count<notice){


                tone(speakerPin,NOTE_G3,1000);
                delay(500);
                noTone(speakerPin);     
        }
       else{
          timeWaitWrong*=2;
          if (timeWaitWrong>300000) timeWaitWrong=300000;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("INVALID PASSWORD ");
          lcd.setCursor(0,1);
          lcd.print("Plz wait ");
          lcd.print(timeWaitWrong/1000+5);
          lcd.print("s");
          tone(speakerPin,NOTE_AS2,1000);
          delay(500);
          noTone(speakerPin);
          tone(speakerPin,NOTE_DS8,2000);
          delay(500);
          noTone(speakerPin);
          tone(speakerPin,NOTE_AS2,1000);
          delay(500);
          noTone(speakerPin);
          tone(speakerPin,NOTE_DS8,2000);
          noTone(speakerPin);
          delay(timeWaitWrong);

          delay(timeWaitWrong);      
       }
        delay(2000);
        lcd.clear();
        digitalWrite(ledError,LOW);
        lcd.print("Enter PIN: ");
     }
}
 


