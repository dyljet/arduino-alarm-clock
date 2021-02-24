#include <Adafruit_RGBLCDShield.h>
#include <Time.h>
#include <TimeLib.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


#define Clock 0
#define menu 1
#define alarmWindow 2


#define RED 0x1
#define YELLOW 0x3
#define BLUE 0x4
#define WHITE 0x7

#define SetTime 0
#define setAlarm 1
#define alarmMode 2
#define showAlarm 3

byte arrowR[] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11110,
  B11100,
  B11000,
  B10000
};
byte arrowL[] = {
  B00010,
  B00110,
  B01110,
  B11110,
  B11110,
  B01110,
  B00110,
  B00010
};
byte arrow[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100
};
byte arrowD[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000
};

bool buttonPush = false;
char window = Clock;
char menuMode = SetTime;
uint8_t alarm_hour = 0; 
uint8_t alarm_minute = 0;
uint8_t alarm_second = 0;

void setup() {
  
Serial.begin(9600);
lcd.begin(16,2);

window = Clock;
setTime(14, 14, 40 ,14, 02, 1991);

}

void loop() {
  
  switch (window)
{

case Clock:
loadClock();
checkAlarmTime();
break;


case menu:
     lcd.setBacklight(BLUE);
allowMenuButtons();
break;


case alarmWindow:
snooze();
  lcd.setBacklight(YELLOW);
    lcd.setBacklight(RED);
break;

}

uint8_t buttons = lcd.readButtons();

    if (buttons & BUTTON_UP) {
      loadMenu();
      
         lcd.createChar(2, arrowL);
          lcd.createChar(3, arrowR);

lcd.setCursor(2,1);

lcd.write(2);

lcd.write(3);
    }

    



}
void menuButtons(uint8_t buttons) {

if (buttons & BUTTON_RIGHT)
{
menuRightButton();
}

else if (buttons & BUTTON_DOWN)
{
goClock();
}

else if (buttons & BUTTON_LEFT)
{
menuLeftButton();
}

else if (buttons & BUTTON_SELECT)
{
menuSelectButton();
}
}

void loadClock(){

  actualClock();
lcd.setCursor(0,0);
lcd.print("Menu");
lcd.setCursor(7,0);
lcd.print("Time");
lcd.createChar(4, arrow);
lcd.setCursor(4,0);
lcd.write(4);
  
}

void loadMenu() {

  window = menu;
lcd.clear();


lcd.setCursor(7, 0);
lcd.print("Menu");

lcd.setCursor(0,0);
lcd.print("Time");

lcd.createChar(4, arrowD);
lcd.setCursor(4,0);
lcd.write(4);  
}

void actualClock(){
 
 lcd.setCursor(6,1);
 lcd.print(hour());
  
 lcd.print(":");
 
 if(minute() < 10) 
 lcd.print('0');
 lcd.print(minute());
   
 lcd.print(":");
 
 if(second() < 10) 
 lcd.print('0');
 lcd.print(second());
 
}






void allowMenuButtons() {

uint8_t buttons = lcd.readButtons();

if (buttons)

{
if (!buttonPush) {
menuButtons(buttons);
}

buttonPush = true;
}

else {
buttonPush = false;
}

}


char previousMode(char mode, char min, char max) {
if (mode > min) {
return --mode;
}
return max;
}

char nextMode(char mode, char min, char max) {
if (mode < max) {
return ++mode;
}
return min;
}




void menuSelectButton()

{
if (window != menu) {
loadMenu();
}
else {

switch (menuMode)
{
case SetTime:
goClock();
break;

case setAlarm:
setAlarm_hour();
setAlarm_minute();
setAlarm_second();
break;

case alarmMode:
offAlarm();
break;

case showAlarm:
ShowAlarmm();
break;
}

}

}

void menuRightButton()
{
if (window == menu) {
menuMode = nextMode(menuMode, 0, 3);
menuModeDesign(menuMode);
}
}

void menuLeftButton()
{
if (window == menu) {
menuMode = previousMode(menuMode, 0, 3);
menuModeDesign(menuMode);
}
}


void goClock() {
  lcd.clear();
  window = Clock;
  lcd.setBacklight(WHITE);
  
}


void menuModeDesign(char modeDesign) {
lcd.setCursor(5, 1);
lcd.print("-");

switch (modeDesign)
{
case SetTime:
lcd.setCursor(6, 1);
lcd.print("Set Time      ");
break;

case setAlarm:
lcd.setCursor(6, 1);
lcd.print("Set Alarm   ");
break;

case alarmMode:
lcd.setCursor(6, 1);
lcd.print("Alarm Off");
break;

case showAlarm:
lcd.setCursor(6, 1);
lcd.print("Show Alarm ");
break;
}
}


void setAlarm_hour()
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("Hour:    0");
lcd.setCursor(5,0);
lcd.print("Alarm");

lcd.createChar(14, arrowL);
lcd.setCursor(14,1);
lcd.write(14);

lcd.createChar(15, arrowR);
lcd.setCursor(15,1);
lcd.write(15);
    
uint8_t setHour = 0;
   
    
while ( (millis() - millis()) < 5000 )
    {
        uint8_t buttons = lcd.readButtons();

           if (buttons & BUTTON_RIGHT )
              {
                setHour = setHour < 23 ? setHour + 1 : setHour;
                lcd.setCursor(8,1);
                if ( setHour < 10 ) {
                   lcd.print(" ");
                }
                lcd.print(setHour);
              }
        else if ( buttons & BUTTON_LEFT )
        {
            setHour = setHour > 0 ? setHour - 1 : setHour;
            lcd.setCursor(8,1);
            if ( setHour < 10 ) { 
              lcd.print(" ");
            }
            lcd.print(setHour);
        }
         
        else if ( buttons & BUTTON_SELECT )
        {
         alarm_hour = setHour;
         break;
        }
        delay(100);
    }
}

void setAlarm_minute()
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("Minute:   0");
lcd.setCursor(5,0);
lcd.print("Alarm");

lcd.createChar(14, arrowL);
lcd.setCursor(14,1);
lcd.write(14);

lcd.createChar(15, arrowR);
lcd.setCursor(15,1);
lcd.write(15);
uint8_t setMinute = 0;
uint8_t setHour = 0 ;
       
    while ( (millis() - millis()) < 5000 )
    {
        uint8_t buttons = lcd.readButtons();
        
        if ( buttons & BUTTON_RIGHT )
        {
            setMinute = setMinute < 59 ? setMinute + 1 : setMinute;
            lcd.setCursor(9,1);
            if ( setMinute < 10 ) {
              lcd.print(" ");
            }
            lcd.print(setMinute);
        }
        else if ( buttons & BUTTON_LEFT )
        {
            setMinute = setMinute > 0 ? setMinute - 1 : setMinute;
            lcd.setCursor(9,1);
            if ( setMinute < 10 ) {
              lcd.print(" ");
            }
            lcd.print(setMinute);
        }
        else if ( buttons & BUTTON_SELECT )
        {
        alarm_minute = setMinute;
            break; 
        }
        delay(100);
    }
}

void setAlarm_second()
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("Second:   0");
lcd.setCursor(5,0);
lcd.print("Alarm");

lcd.createChar(14, arrowL);
lcd.setCursor(14,1);
lcd.write(14);

lcd.createChar(15, arrowR);
lcd.setCursor(15,1);
lcd.write(15);

uint8_t setMinute = 0;
uint8_t setHour = 0 ;
uint8_t setSecond = 0 ;

    while ( (millis() - millis()) < 5000 )
    {
        uint8_t buttons = lcd.readButtons();
        
        if ( buttons & BUTTON_RIGHT )
        {
            setSecond = setSecond < 59 ? setSecond + 1 : setSecond;
            lcd.setCursor(9,1);
            if ( setSecond < 10 ) {
              lcd.print(" ");
            }
            lcd.print(setSecond);
        }
        else if ( buttons & BUTTON_LEFT )
        {
            setSecond = setSecond > 0 ? setSecond - 1 : setSecond;
            lcd.setCursor(9,1);
            if ( setSecond < 10 ) {
              lcd.print(" ");
            }
            lcd.print(setSecond);
        }
        else if ( buttons & BUTTON_SELECT )
        {
        alarm_second = setSecond;
        goClock();
            break;
        }
        delay(100);
    }
}

void checkAlarmTime()
{
    if ( hour() == alarm_hour && minute() == alarm_minute && second() == alarm_second) {

window = alarmWindow;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("ALERT ALERT ALERT");
lcd.setCursor(0,1);
lcd.print("Snooze");
lcd.setCursor(9,1);
lcd.print("Dismiss");
}
}


void ShowAlarmm()
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Menu");

lcd.createChar(4, arrow);
lcd.setCursor(4,0);
lcd.write(4);
 
lcd.print("   Alarm");
lcd.setCursor(6,1);
lcd.print(String( alarm_hour < 9 ? "0" : "" ) + alarm_hour + ":" + ( alarm_minute < 9 ? "0" : "" ) + alarm_minute + ":" +  ( alarm_second < 9 ? "0" : "" ) + alarm_second) ;

uint8_t buttons = lcd.readButtons();
        
  if ( buttons & BUTTON_UP )
        {
      loadMenu();
        }
}


void snooze(){

uint8_t buttons = lcd.readButtons();
        
        if ( buttons & BUTTON_RIGHT )
        {
          window = Clock;
          lcd.clear();
          lcd.setBacklight(WHITE);
        }
        else if ( buttons & BUTTON_LEFT )
        {
              alarm_second += 10;
              window = Clock;
              lcd.clear();
              lcd.setBacklight(WHITE);
        }
}

  
void offAlarm(){

         alarm_hour = false;
         alarm_minute = false;
         alarm_second = false;
 lcd.clear();
lcd.setCursor(0,0);
lcd.print("Menu");

lcd.createChar(4, arrow);
lcd.setCursor(4,0);
lcd.write(4);

lcd.setCursor(4,1);
lcd.print("ALARM OFF");

        



        }


