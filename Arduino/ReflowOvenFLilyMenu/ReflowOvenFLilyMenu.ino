/*********************************************************************
ReflowOvenFLily - Written to work with ReflowControllerC
LilyPadUSB.bootloader.extended_fuses=0xce
LilyPadUSB.bootloader.high_fuses=0xd8
LilyPadUSB.bootloader.low_fuses=0xff

           Magic Happens Here!
* 225-|                                               x  x  
*     |                                            x        x
*     |                                         x              x
*     |                                      x                    x
* 200-|                                   x                          x
*     |                              x    |                          |   x   
*     |                         x         |                          |       x
*     |                    x              |                          |
* 150-|               x                   |                          |
*     |             x |                   |                          |
*     |           x   |                   |                          | 
*     |         x     |                   |                          | 
*     |       x       |                   |                          | 
*     |     x         |                   |                          |
*     |   x           |                   |                          |
* 30 -| x             |                   |                          |
*     |<  60 - 90 s  >|<    90 - 120 s   >|<       90 - 120 s       >|
*     | Preheat Stage |   Soaking Stage   |       Reflow Stage       | Cool
*  0  |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
*                                                                Time (Seconds)
*********************************************************************/

#include <Wire.h>
// Includes
#include <Adafruit_GFX.h>       // GFX Library - https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>   // SSD Library - https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_MAX31855.h>  // Thermocouple Library - https://github.com/rocketscream/MAX31855
#include <PID_v1.h>             // PID Library - http://playground.arduino.cc/Code/PIDLibrary
                                //             - https://github.com/br3ttb/Arduino-PID-Library/
#include <EEPROMEx.h>           // EEPROMex Library - http://playground.arduino.cc/Code/EEPROMex
// OHARARP FAVICON (LCD ASSISTANT, Byte Orientation - Horizontal Width = 64 Height = 64
static unsigned char PROGMEM logo[] = { 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0xC0,
0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0xF0, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFC,
0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFE,
0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFE,
0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF,
0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF,
0x7F, 0xFF, 0xFF, 0xE0, 0x03, 0xE7, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0x80, 0x03, 0xCF, 0xFF, 0xFF,
0x7F, 0xFF, 0xFE, 0x00, 0x07, 0xCF, 0xFF, 0xFF, 0x7F, 0xFF, 0xF8, 0x00, 0x07, 0x8F, 0xFF, 0xFF,
0x7F, 0xFF, 0xF0, 0x00, 0x0F, 0x87, 0xFF, 0xFF, 0x7F, 0xFF, 0xE0, 0x00, 0x1F, 0x03, 0xFF, 0xFF,
0x7F, 0xFF, 0xC0, 0x00, 0x3E, 0x01, 0xFF, 0xFF, 0x7F, 0xFF, 0x80, 0x00, 0x7E, 0x00, 0xFF, 0xFF,
0x7F, 0xFF, 0x80, 0x00, 0x7C, 0x00, 0x7F, 0xFF, 0x7F, 0xFF, 0x00, 0x00, 0xFC, 0x00, 0x7F, 0xFF,
0x7F, 0xFF, 0x00, 0x01, 0xFF, 0xC0, 0x3F, 0xFF, 0x7F, 0xFE, 0x00, 0x03, 0xFF, 0xC0, 0x3F, 0xFF,
0x7F, 0xFE, 0x00, 0x07, 0xFF, 0x80, 0x1F, 0xFF, 0x7F, 0xFC, 0x00, 0x07, 0xFF, 0x80, 0x1F, 0xFF,
0x7F, 0xFC, 0x00, 0x0F, 0xFF, 0x00, 0x0F, 0xFF, 0x7F, 0xFC, 0x00, 0x10, 0xFE, 0x00, 0x0F, 0xFF,
0x7F, 0xFC, 0x00, 0x01, 0xFE, 0x00, 0x0F, 0xFF, 0x7F, 0xF8, 0x00, 0x03, 0xFC, 0x00, 0x0F, 0xFF,
0x7F, 0xF8, 0x00, 0x03, 0xFC, 0x00, 0x0F, 0xFF, 0x7F, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x0F, 0xFF,
0x7F, 0xF8, 0x00, 0x07, 0xF8, 0x00, 0x0F, 0xFF, 0x7F, 0xF8, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xFF,
0x7F, 0xF8, 0x00, 0x0F, 0xE0, 0x00, 0x0F, 0xFF, 0x7F, 0xF8, 0x00, 0x1F, 0xE0, 0x00, 0x0F, 0xFF,
0x7F, 0xFC, 0x00, 0x1F, 0xC0, 0x00, 0x0F, 0xFF, 0x7F, 0xFC, 0x00, 0x3F, 0xFC, 0x00, 0x0F, 0xFF,
0x7F, 0xFC, 0x00, 0x7F, 0xF8, 0x00, 0x0F, 0xFF, 0x7F, 0xFC, 0x00, 0x7F, 0xF0, 0x00, 0x1F, 0xFF,
0x7F, 0xFE, 0x00, 0xFF, 0xF0, 0x00, 0x1F, 0xFF, 0x7F, 0xFE, 0x00, 0xFF, 0xE0, 0x00, 0x3F, 0xFF,
0x7F, 0xFF, 0x01, 0xFF, 0xC0, 0x00, 0x3F, 0xFF, 0x7F, 0xFF, 0x01, 0x9F, 0x80, 0x00, 0x7F, 0xFF,
0x7F, 0xFF, 0x80, 0x1F, 0x80, 0x00, 0x7F, 0xFF, 0x7F, 0xFF, 0xC0, 0x3F, 0x00, 0x00, 0xFF, 0xFF,
0x7F, 0xFF, 0xC0, 0x3E, 0x00, 0x01, 0xFF, 0xFF, 0x7F, 0xFF, 0xE0, 0x7C, 0x00, 0x03, 0xFF, 0xFF,
0x7F, 0xFF, 0xF0, 0x78, 0x00, 0x07, 0xFF, 0xFF, 0x7F, 0xFF, 0xFC, 0xF8, 0x00, 0x0F, 0xFF, 0xFF,
0x7F, 0xFF, 0xFC, 0xF0, 0x00, 0x3F, 0xFF, 0xFF, 0x7F, 0xFF, 0xF9, 0xE0, 0x00, 0xFF, 0xFF, 0xFF,
0x7F, 0xFF, 0xFB, 0xF0, 0x03, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x7F, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x7F, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0x3F, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
0x0F, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x03, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

typedef enum REFLOW_STATE
{
  REFLOW_STATE_IDLE,
  REFLOW_STATE_PREHEAT,
  REFLOW_STATE_SOAK,
  REFLOW_STATE_REFLOW,
  REFLOW_STATE_COOL,
  REFLOW_STATE_COMPLETE,
  REFLOW_STATE_TOO_HOT,
  REFLOW_STATE_ERROR
} reflowState_t;

typedef enum REFLOW_STATUS
{
  REFLOW_STATUS_OFF,
  REFLOW_STATUS_ON
} reflowStatus_t;

typedef	enum SWITCH
{
	SWITCH_NONE,
	SWITCH_1,	
	SWITCH_2,
}	switch_t;

// ***** CONSTANTS *****
#define OLED_RESET 4
float TEMPERATURE_COOL       = 100;
float TEMPERATURE_SOAK_MIN   = 150;
float TEMPERATURE_SOAK_MAX   = 180;
float TEMPERATURE_REFLOW_MAX = 220;
float TEMP_VALUE_MENU = 0;

#define SENSOR_SAMPLING_TIME   1000
#define SOAK_TEMPERATURE_STEP  5
#define SOAK_MICRO_PERIOD      9000
#define DEBOUNCE_PERIOD_MIN    50

// ***** PID PARAMETERS *****
// ***** PRE-HEAT STAGE *****
float PID_KP_PREHEAT         = 300;
float PID_KI_PREHEAT         = 0.05;
float PID_KD_PREHEAT         = 350; 
// ***** SOAKING STAGE *****
float PID_KP_SOAK            = 300;
float PID_KI_SOAK            = 0.05;
float PID_KD_SOAK            = 350;
// ***** REFLOW STAGE *****
float PID_KP_REFLOW          = 300;
float PID_KI_REFLOW          = 0.05;
float PID_KD_REFLOW          = 350; 
#define PID_SAMPLE_TIME 1000

// ***** LCD MESSAGES *****
const char* ssdMessagesReflowStatus[] = {
  "  Ready  ",
  " Preheat ",
  "   Soak  ",
  "  Reflow ",
  "   Cool  ",
  " Complete",
  "  !HOT!  ",
  "  Error  "
};

// Pin Definitions
int TopPin = 8;  // Top    Element 
int BotPin = 7;  // Bottom Element
int DoorPin =6;  // Door Sense
int BuzPin = A2; // Buzzer Pin
int Led = 13;    // Heartbeat Led

int But1 = 9; // Start Button
int But2 = 10; // Menu  Button
int But3 = 11; // Plus  Button
int But4 = 12; // Minus Button

int thermocoupleCLKPin = A3;
int thermocoupleSOPin  = A4;
int thermocoupleCSPin  = A5;

// ***** PID CONTROL VARIABLES *****
double setpoint;
double input;
double inputOld; //Store old Temperature
double output;
double kp = PID_KP_PREHEAT;
double ki = PID_KI_PREHEAT;
double kd = PID_KD_PREHEAT;
int windowSize;
unsigned long windowStartTime;
unsigned long nextCheck;
unsigned long nextRead;
unsigned long timerSoak;
unsigned long buzzerPeriod;
boolean StartTest;

// Reflow oven controller state machine state variable
reflowState_t reflowState;
// Reflow oven controller status
reflowStatus_t reflowStatus;

//Button Variables
float MenuArray[14] = {TEMPERATURE_COOL,TEMPERATURE_SOAK_MIN,TEMPERATURE_SOAK_MAX,TEMPERATURE_REFLOW_MAX,
                       PID_KP_PREHEAT,PID_KI_PREHEAT,PID_KD_PREHEAT,
                       PID_KP_SOAK,PID_KI_SOAK,PID_KD_SOAK,
                       PID_KP_REFLOW,PID_KI_REFLOW,PID_KD_REFLOW,0};
float PromTest = 0;                       
byte  ArrowArray[5] = {24,32,40,48,56};
byte  ArrowIdx[13]  = {0,1,2,3,0,1,2,0,1,2,0,1,2};
byte  MathIdx[13]   = {0,0,0,0,0,1,0,0,1,0,0,1,0};
boolean MenuFlag;
boolean PlusFlag;
boolean MinusFlag;

byte ButtCount  = 0;
int  MenuCount  = -1;
int  PlusMinus  = 0;

// tc Error Counter
int tcErrorCtr = 0;

// Seconds timer
int timerSeconds;

// Library Setup
// Specify PID control interface
PID reflowOvenPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_MAX31855 thermocouple(thermocoupleCLKPin, thermocoupleCSPin, thermocoupleSOPin);

// Setup
void setup()   {        
  // Setup Serial Baudrate  
  Serial.begin(9600);

  // EEPROM Read
  PromTest = EEPROM.readFloat(0);
  if (PromTest == 0) // Assume EEPROM has never been written to
  {
    // Save Hard Coded Data to EEPROM
    for (int i = 0; i<12;i++){
      EEPROM.updateFloat(i*4, MenuArray[i]);  
    }
  }
  else 
  {
    // Read Previously Saved Values and Update Pid Variables
    for (int i = 0; i<12;i++){
      MenuArray[i] = EEPROM.readFloat(i*4);  
    }
    
      TEMPERATURE_COOL        = MenuArray[0];    
      TEMPERATURE_SOAK_MIN    = MenuArray[1];    
      TEMPERATURE_SOAK_MAX    = MenuArray[2];    
      TEMPERATURE_REFLOW_MAX  = MenuArray[3];    
      PID_KP_PREHEAT          = MenuArray[4];    
      PID_KI_PREHEAT          = MenuArray[5];    
      PID_KD_PREHEAT          = MenuArray[6];      
      PID_KP_SOAK             = MenuArray[7];    
      PID_KI_SOAK             = MenuArray[8];      
      PID_KD_SOAK             = MenuArray[9];     
      PID_KP_REFLOW           = MenuArray[10];    
      PID_KI_REFLOW           = MenuArray[11];    
      PID_KD_REFLOW           = MenuArray[12];
  }
      
  // initialize the Led pin as an output.
  digitalWrite(BotPin, HIGH);
  pinMode(Led, OUTPUT);
 
 // Init Buttons
  pinMode(But1, INPUT); 
  pinMode(But2, INPUT);
  pinMode(But3, INPUT);
  pinMode(But4, INPUT);

  // Init Elements
  pinMode(TopPin, OUTPUT);
  pinMode(BotPin, OUTPUT);
  pinMode(BuzPin, OUTPUT);
    
  digitalWrite(TopPin, LOW);
  digitalWrite(BotPin, LOW);
  digitalWrite(BuzPin, LOW);
    
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  
  //Display OHARARP Splash Screen
  display.clearDisplay();
  display.drawBitmap(31,0, logo, 64, 64, 1);
  display.display();
  delay(2000);
  
  // Set window size
  windowSize = 5000;
  // Initialize time keeping variable
  nextCheck = millis();
  // Initialize thermocouple reading variable
  nextRead = millis();
  // Set Relow Status = OFF
  reflowStatus = REFLOW_STATUS_OFF;
  StartTest = false;  
}

// Begin Main Loop 
void loop()
{
   // Current time
  unsigned long now;

  // Test for Start Button Input
   if (digitalRead(But1) == HIGH)
  {

    for (int i = 0; i<5;i++){
        if (digitalRead(But1) == HIGH)
        {
          ButtCount++;
          tone(BuzPin,4100,100); //Buzz the Buzzer
          delay(250);
          //Serial.println( ButtCount); 
        }
      }
      
      if (ButtCount>=5)
      {
        reflowState = REFLOW_STATE_IDLE;
        StartTest = true;
      }
      else
      {
        ButtCount = 0;
        StartTest = false;
      }
  }
  else
  {
    ButtCount = 0;
    StartTest = false;    
  }

// Read the Menu Button
  if (digitalRead(But2) == HIGH)
  { 
    MenuCount++; 
    tone(BuzPin,3100,100); //Buzz the Buzzer
    Serial.print("MenuCount = "); Serial.println(MenuCount);
    delay(250);
    MenuFlag = true;
    
    //Menu Overflow
    if (MenuCount>=14)
    {
      MenuCount = 0;
    }
  }
  
  // Read the Plus Button
  if (digitalRead(But3) == HIGH)
  { 
    tone(BuzPin,3500,100); //Buzz the Buzzer
    delay(100);

    if (MathIdx[MenuCount]==1)    
    {
      MenuArray[MenuCount]=MenuArray[MenuCount]+.005;
      //Serial.print(MenuCount);Serial.print(MenuArray[MenuCount]);
    }
    else
    {
      MenuArray[MenuCount]=MenuArray[MenuCount]+5;
      //Serial.print(MenuCount);Serial.print(MenuArray[MenuCount]);
    }
    MenuFlag = true;
  }
  
  // Read the Minus Button
  if (digitalRead(But4) == HIGH)
  {
    tone(BuzPin,3700,100); //Buzz the Buzzer
    delay(100);    
    
    if (MathIdx[MenuCount]==1)    
    {
      MenuArray[MenuCount]=MenuArray[MenuCount]-.005;
    }
    else
    {
      MenuArray[MenuCount]=MenuArray[MenuCount]-5;
    }
    MenuFlag = true;
  } 
 
  if (MenuFlag == true)
  {
    if(MenuCount >= 0 && MenuCount <=3)
    {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE); 
      display.println(" Temp Menu");
      display.drawLine(0, 16, 127, 16, WHITE);

      display.setTextSize(1);
      display.setCursor(0,ArrowArray[0]);  
      display.print("  Cool Min   = ");display.print(MenuArray[0],0);display.println(" C");
      display.print("  Soak Min   = ");display.print(MenuArray[1],0);display.println(" C");
      display.print("  Soak Max   = ");display.print(MenuArray[2],0);display.println(" C");
      display.print("  Reflow Max = ");display.print(MenuArray[3],0);display.println(" C");
    }
  
    if(MenuCount >= 4 && MenuCount <=6)
    {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE); 
      display.println("  Preheat");
      display.drawLine(0, 16, 127, 16, WHITE);
      
      display.setTextSize(1);
      display.setCursor(0,ArrowArray[0]);  
      display.print("  KP   = ");display.println(MenuArray[4]);
      display.print("  KI   = ");display.println(MenuArray[5],3);
      display.print("  KD   = ");display.println(MenuArray[6]);
    }
    
    if(MenuCount >= 7 && MenuCount <=9)
    {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE); 
      display.println("   Soak");
      display.drawLine(0, 16, 127, 16, WHITE);

      display.setTextSize(1);
      display.setCursor(0,ArrowArray[0]);  
      display.print("  KP   = ");display.println(MenuArray[7]);
      display.print("  KI   = ");display.println(MenuArray[8],3);
      display.print("  KD   = ");display.println(MenuArray[9]);
    }
    
    if(MenuCount >= 10 && MenuCount <=12)
    {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE); 
      display.println("   Reflow");
      display.drawLine(0, 16, 127, 16, WHITE);
      display.setTextSize(1);
      
      display.setCursor(0,ArrowArray[0]);  
      display.print("  KP   = ");display.println(MenuArray[10]);
      display.print("  KI   = ");display.println(MenuArray[11],3);
      display.print("  KD   = ");display.println(MenuArray[12]);
    }
    
    if(MenuCount >= 13) // Store Settings to Memory and EEPROM
    {
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE); 
      display.println(" Settings ");
      display.println("  Stored  ");
      
      TEMPERATURE_COOL        = MenuArray[0];    
      TEMPERATURE_SOAK_MIN    = MenuArray[1];    
      TEMPERATURE_SOAK_MAX    = MenuArray[2];    
      TEMPERATURE_REFLOW_MAX  = MenuArray[3];    
      PID_KP_PREHEAT          = MenuArray[4];    
      PID_KI_PREHEAT          = MenuArray[5];    
      PID_KD_PREHEAT          = MenuArray[6];      
      PID_KP_SOAK             = MenuArray[7];    
      PID_KI_SOAK             = MenuArray[8];      
      PID_KD_SOAK             = MenuArray[9];     
      PID_KP_REFLOW           = MenuArray[10];    
      PID_KI_REFLOW           = MenuArray[11];    
      PID_KD_REFLOW           = MenuArray[12];
      
      // Save Data to EEPROM
      for (int i = 0; i<12;i++){
        EEPROM.updateFloat(i*4, MenuArray[i]);  
      }
    }
    
    if(MenuCount < 13) 
    {
      display.setCursor(0,ArrowArray[ArrowIdx[MenuCount]]);  
      display.print("->");  
    }
    display.display();  
    MenuFlag = false; // Stop Updating Display
  }
  
//********************************************************************************************************
  // Time to read thermocouple?
  if (millis() > nextRead)
  {
    // Read thermocouple next sampling period
    nextRead += SENSOR_SAMPLING_TIME;
    // Read current temperature
    inputOld = input; //Store Old Temperature
    input = thermocouple.readCelsius();
    if(isnan(input))
    {
      input=inputOld;  
      tcErrorCtr++;
      
      if (tcErrorCtr >= 5)
      {
        // Illegal operation
        reflowState  = REFLOW_STATE_ERROR;
        reflowStatus = REFLOW_STATUS_OFF;
      }
    }
    else
    {
      tcErrorCtr = 0;  
    }
  }
  
  if (millis() > nextCheck)
  {
    // Check input in the next seconds
    nextCheck += 1000;
    // If reflow process is on going
    if (reflowStatus == REFLOW_STATUS_ON)
    {
      // Toggle LED as system heart beat
      digitalWrite(Led, !(digitalRead(Led)));
      // Increase seconds timer for reflow curve analysis
      timerSeconds++;
      // Send temperature and time stamp to serial 
      Serial.print(timerSeconds);
      Serial.print(",");
      Serial.println(thermocouple.readInternal());
      Serial.print(",");
      Serial.print(setpoint);
      Serial.print(",");
      Serial.print(input);
      Serial.print(",");
      Serial.println(output);
      
      // Send temperature and time stamp to display
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(BLACK,WHITE); 
      display.println(ssdMessagesReflowStatus[reflowState]);
      
      display.setTextColor(WHITE);
      display.setTextSize(1.5);
      display.println();
      display.print("Time  = ");display.print(timerSeconds);display.println(" s");
      display.print("Chip  = ");display.print(thermocouple.readInternal());display.println(" C");
      display.print("SP    = ");display.print(setpoint);display.println(" C");
      display.print("Board = ");display.print(input);display.println(" C");
      display.print("PID   = ");display.print(output);display.println(" ");
      display.display();       
    }
    else
    {
      // Turn off LED
      digitalWrite(Led, HIGH);
    }

    // If currently in error state
    if (reflowState == REFLOW_STATE_ERROR)
    {
      // No thermocouple wire connected
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.print("TC Error!");
      display.display(); 
    }
  }
  
  // Refresh The Display
  
  
  // Reflow oven controller state machine
  switch (reflowState)
  {
  case REFLOW_STATE_IDLE:
    // If oven temperature is still above room temperature
    if (input >= TEMPERATURE_COOL)
      {
        reflowState = REFLOW_STATE_TOO_HOT;
        // No thermocouple wire connected
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(2);
        display.setTextColor(BLACK,WHITE);
        display.println(" - HOT - ");
        display.println(input);
        display.display(); 
      }
      else
      {
        if (StartTest == true)
        {
          display.clearDisplay();
          display.setCursor(0,0);
          display.setTextSize(2);
          display.setTextColor(BLACK,WHITE);
          display.print("  Begin  ");
          display.display(); 
       
          // Send header for CSV file
          //Serial.println("Time Setpoint Input Output");
          // Intialize seconds timer for serial debug information
          timerSeconds = 0;
          // Initialize PID control window starting time
          windowStartTime = millis();
          // Ramp up to minimum soaking temperature
          setpoint = TEMPERATURE_SOAK_MIN;
          // Tell the PID to range between 0 and the full window size
          reflowOvenPID.SetOutputLimits(0, windowSize);
          reflowOvenPID.SetSampleTime(PID_SAMPLE_TIME);
          // Turn the PID on
          reflowOvenPID.SetMode(AUTOMATIC);
          // Proceed to preheat stage
          reflowState = REFLOW_STATE_PREHEAT;
          StartTest = false;
          }
      }
  break;

  case REFLOW_STATE_PREHEAT:
    reflowStatus = REFLOW_STATUS_ON;
    // If minimum soak temperature is achieve       
    if (input >= TEMPERATURE_SOAK_MIN-2.5)
    {
      // Chop soaking period into smaller sub-period
      timerSoak = millis() + SOAK_MICRO_PERIOD;
      // Set less agressive PID parameters for soaking ramp
      reflowOvenPID.SetTunings(PID_KP_SOAK, PID_KI_SOAK, PID_KD_SOAK);
      // Ramp up to first section of soaking temperature
      setpoint = TEMPERATURE_SOAK_MIN + SOAK_TEMPERATURE_STEP;   
      // Proceed to soaking state
      reflowState = REFLOW_STATE_SOAK; 
    }
  break;

  case REFLOW_STATE_SOAK:     
    // If micro soak temperature is achieved       
    if (millis() > timerSoak)
    {
      timerSoak = millis() + SOAK_MICRO_PERIOD;
      // Increment micro setpoint
      setpoint += SOAK_TEMPERATURE_STEP;
      if (setpoint > TEMPERATURE_SOAK_MAX)
      {
        // Set agressive PID parameters for reflow ramp
        reflowOvenPID.SetTunings(PID_KP_REFLOW, PID_KI_REFLOW, PID_KD_REFLOW);
        // Ramp up to first section of soaking temperature
        setpoint = TEMPERATURE_REFLOW_MAX;   
        // Proceed to reflowing state
        reflowState = REFLOW_STATE_REFLOW; 
      }
    }
  break; 

  case REFLOW_STATE_REFLOW:
    // We need to avoid hovering at peak temperature for too long
    // Crude method that works like a charm and safe for the components
    if (input >= (TEMPERATURE_REFLOW_MAX-5.0))
    {
      // Set PID parameters for cooling ramp
      reflowOvenPID.SetTunings(PID_KP_REFLOW, PID_KI_REFLOW, PID_KD_REFLOW);
      // Ramp down to minimum cooling temperature 
      setpoint = TEMPERATURE_COOL;   
      // Proceed to cooling state
      reflowState = REFLOW_STATE_COOL;
      
      // Turn Elements Off
      digitalWrite(TopPin, LOW);
      digitalWrite(BotPin, LOW);
      
      // Signal to Open Door
      for (int i = 0; i<10;i++){
        tone(BuzPin,4100,100); //Buzz the Buzzer
        delay(250);
      }       
      
    }
  break;   

  case REFLOW_STATE_COOL:
    // If minimum cool temperature is achieve       
    if (input <= TEMPERATURE_COOL)
    {
      // Retrieve current time for buzzer usage
      buzzerPeriod = millis() + 1000;
      
      for (int i = 0; i<10;i++){
        tone(BuzPin,4100,500); //Buzz the Buzzer  
        delay(250);
      }
      
      reflowStatus = REFLOW_STATUS_OFF;                
      // Proceed to reflow Completion state
      reflowState = REFLOW_STATE_COMPLETE; 
    }         
  break;     

  case REFLOW_STATE_COMPLETE:
    if (millis() > buzzerPeriod)
    {
      reflowState = REFLOW_STATE_IDLE; 
    }
  break;

  case REFLOW_STATE_TOO_HOT:
  // If oven temperature drops below room temperature
    if (input < TEMPERATURE_COOL)
    {
    // Ready to reflow
    reflowState = REFLOW_STATE_IDLE;
    }
  break;

  case REFLOW_STATE_ERROR:
    // If thermocouple problem is still present
    if(isnan(input))
    {
      // Wait until thermocouple wire is connected
      reflowState = REFLOW_STATE_ERROR; 
    }
    else
    {
      // Clear to perform reflow process
      reflowState = REFLOW_STATE_IDLE; 
    }
    break;    
  }    

  // PID computation and SSR control
  if (reflowStatus == REFLOW_STATUS_ON)
  {
    now = millis();

    reflowOvenPID.Compute();

    if((now - windowStartTime) > windowSize)
    { 
      // Time to shift the Relay Window
      windowStartTime += windowSize;
    }
    if(output > (now - windowStartTime)) 
    {
      digitalWrite(TopPin, HIGH);
      digitalWrite(BotPin, HIGH);
    }
    else 
    {
      digitalWrite(TopPin, LOW);
      digitalWrite(BotPin, LOW);
    }
  }
  // Reflow oven process is off, ensure oven is off
  else 
  {
    digitalWrite(TopPin, LOW);
    digitalWrite(BotPin, LOW);
  }
  
}

