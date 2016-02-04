//Security System using ATmega328P
// written on October 18, 2013, Written by Lomas
#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
const unsigned char motor_FW = 14;
const unsigned char motor_BW = 15;
const unsigned char buzzer = 16;
unsigned char key;
unsigned int chance = 0;
unsigned char code_user[4], code_temp[4];
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 3, 2, 1, 0 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 6, 5, 4 };
// Create the Keypad
Keypad my_keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  lcd.begin(16, 2);
  pinMode(motor_FW, OUTPUT);
  pinMode(motor_BW, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
void loop()
{
  key = 0;
  key = my_keypad.getKey();
  while(!key)
  {
    lcd.setCursor(0,0);
    lcd.print("Press any key...");
    lcd.setCursor(0,1);
    lcd.print(".....to Proceed.");
    key = my_keypad.getKey();
    if(key)  break;
  }
  
  lcd.clear();
  lcd.print("Enter Code:");
  key = my_keypad.getKey();
  int i=0;
      key=0;
        while(i<4)         // there are only four digit code
        {
          key = my_keypad.getKey();  // Read Key from keypad
          if(key)                    // if Key value is not zero
          {
            code_user[i]=key;        // then store the first digit first array element
            i++;                    
            lcd.setCursor(i,1);
            lcd.print("*");          // Display '*' (Asterik) at each key stroke
          }
          key=0;                     // initiliaze key value for next key stroke
        }
        delay(500);                  // wait for half second after all key are pressed
        int count = 0;               // define a new count variable to compare two arrays
        for(i=0; i<4; i++)           // i.e. one EEPROM and other code_user
        {
          if(EEPROM.read(i) == code_user[i])  count++;  // Compare each code here
        }
        if(count == 4)
        {
          while(~key)
          {
            //lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("1.LOCK  "); // A menu view of the system 
            lcd.print("2.UNLOCK");
            lcd.setCursor(0,1);        // at the begning
            lcd.print("3.CHANGE CODE ");
            key = my_keypad.getKey();
            if(key)  break;
          }
          
          switch(key)
          {
            case '1':
            {
               lcd.clear();
               lcd.print("Closing Door...");
               digitalWrite(motor_BW, HIGH);
               digitalWrite(motor_FW, LOW);
               delay(1000); // for 2 seconds
               digitalWrite(motor_BW, LOW);
               break;
             }
             
             case '2' :
             {
               
               
              lcd.clear();
              lcd.print("Opening Door....");
              digitalWrite(motor_FW, HIGH);
              digitalWrite(motor_BW, LOW);
              delay(1000); // for 2 seconds
              digitalWrite(motor_FW, LOW);
              break;
             }
             
             case'3':
             {
               lcd.clear();
               lcd.print("Enter New Code.");
               i = key = 0;
               while(i<4)
               {
                  key = my_keypad.getKey();
                  if(key)
                  {
                    code_temp[i] = key;  
                    i++;
                    lcd.setCursor(i,1);
                    lcd.print("*");
                  }
                  key=0;
               }
               delay(500); 
               lcd.clear();
               lcd.print("Verify New Code.");
               i = key = count = 0;
               while(i<4)
               {
                  key = my_keypad.getKey();
                  if(key)
                  {
                    if(code_temp[i] == key)
                    {
                      count++;
                    }
                    i++;
                    lcd.setCursor(i,1);
                    lcd.print("*");
                   }
                   key=0;
                }
                delay(500);
                if(count==4)
                {
                   for(i=0; i<4; i++)
                   {
                      EEPROM.write(i,code_temp[i]);
                   }
                   lcd.clear();
                   lcd.setCursor(6,0);
                   lcd.print("DONE!");
                 }
                 else
                {
                  lcd.clear();
                  lcd.setCursor(4,0);
                  lcd.print("NOT DONE!");
                }
                delay(500);
               break;
             }
          }
        }
        else
        {
          lcd.clear();
          lcd.print("DID NOT MATCHED!");
          chance++;
          if(chance>=3)
          {
            chance = 0;
            lcd.clear();
            lcd.print("ACCESS DENIED!");
            lcd.setCursor(0,1);
            lcd.print("Try later!");
            digitalWrite(buzzer, HIGH);
            delay(3000);
            digitalWrite(buzzer, LOW);
//            tone(buzzer, 500);
//            delay(3000);
//            noTone(buzzer);
          }
          delay(500);
        }      
}
