#include "Arduino.h"
#include <EEPROM.h>
#include <TM1637Display.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include "Letter.h"

#define LEDS_PIN  2
#define DISPLAY_CLOCK 19
#define DISPLAY_DIO 18
#define PUSH_BUTTON 13
#define RND_BUTTON 12
#define EEPROM_SIZE 64

#define CHANNEL   0
#define NB_LETTER 8
#define NB_MODE 6

int Nb_Pixel_A= 19;
int Nb_Pixel_D= 18;
int Nb_Pixel_C= 14;
int Nb_Pixel_R= 19;
int Nb_Pixel_E= 16;
int Nb_Pixel_Ball= 2;
int Nb_Pixel_Doigt= 7;
int Current_Mode=0;
int Mode_Init=0, Rand_Time;
int Mode;
unsigned long MillisTime=0;
int adr_Mode=10;

int LEDS_COUNT= Nb_Pixel_Doigt+Nb_Pixel_Ball+Nb_Pixel_E+Nb_Pixel_R+Nb_Pixel_C+Nb_Pixel_D+(2*Nb_Pixel_A);
int Debounce_Time=0;
#define Pink 0xFF00FF
#define blue 0x0000FF
#define Yellow 0xFFFF00
#define Light_Green 0x00FF20
#define Light_Blue 0x0050FF
#define White 0xFFFFFF

Freenove_ESP32_WS2812 strip(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);
TM1637Display display(DISPLAY_CLOCK, DISPLAY_DIO);

u8 m_color[7][3] = { {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {255, 0, 255}, {255, 255, 0},{0, 255, 255} };
int delayval = 100;
int Mode_Write=0, ToWrite=0;


Letter Array_Letter[NB_LETTER]={
  {Letter("A2",Nb_Pixel_A,Pink)},
  {Letter("R",Nb_Pixel_R,Yellow)},
  {Letter("C",Nb_Pixel_C,0x0000FF)},
  {Letter("A1",Nb_Pixel_A,Light_Green)},
  {Letter("D",Nb_Pixel_D,Pink)},
  {Letter("E",Nb_Pixel_E,Yellow)},
  {Letter("Ball",Nb_Pixel_Ball,White)},
  {Letter("Doigt",Nb_Pixel_Doigt,Light_Blue)}
  };


/*Doigt, Ball, E, D, A1, C, R , A2;
Letter *Array_Letter[8]={&A2,&R,&C,&A1,&D,&E,&Ball,&Doigt};*/
void Init_Letters()
{
  int Pixel_Start=0;
  int Pixel_Stop=0;
  int Taille_tableau=sizeof(Array_Letter);

  for(int i=NB_LETTER-1;i>-1;i--)
  {
      Pixel_Start=Pixel_Stop;
      Pixel_Stop=Pixel_Start+Array_Letter[i].Get_Nb_Pixels()-1;
      Array_Letter[i].Set_PixelsPosition(Pixel_Start,Pixel_Stop);
  }

  int offset=0;
  for(int i=0;i<NB_LETTER;i++)
  {
    uint32_t Color_Code=Array_Letter[i].Get_Default_Color();
    
    for(int j=NB_LETTER-1;j>offset-1;j--)
    {
      Array_Letter[j].Set_Letter_Color(Color_Code);
      delay(300);
      if(j>offset)
      {
        Array_Letter[j].Off();
      }
    }
    
    offset++;
  }

  
}

void Normal_Mode()
{ 
  if(Mode_Init==0){
    Init_Letters();
    Mode_Init=1;
  }
  delay(50);
  Array_Letter[random(7)].Scintillement();
  if((MillisTime+Rand_Time)<millis())
  {
    Array_Letter[random(7)].Neon_Blink();
    MillisTime=millis();
    Rand_Time=random(20000);
  }
}

void Fire_Mode()
{
  Array_Letter[random(7)].Fire();
}

void Rainbow()
{
  for (int j = 0; j < 255; j += 2) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      if(Mode!=3){break;}
      strip.setLedColorData(i, strip.Wheel((i * 256 / LEDS_COUNT + j) & 255));
    }
    strip.show();
    delay(5);
    if(Mode!=3){break;}
  }
}

void RGBW()
{
  for (int j = 0; j < 7; j++) {
    for (int i = LEDS_COUNT; i > -1; i--) {
       if(Mode!=4){break;}
      strip.setLedColorData(i, m_color[j][0], m_color[j][1], m_color[j][2]);
      strip.show();
      delay(delayval);
    }
    delay(400);
    if(Mode!=4){break;}
  }
}

void Update_Mode()
{
  Mode_Init=0;
  if(Debounce_Time+500<millis())
  {
    Mode++;
    
    Debounce_Time=millis();
    if (Mode>NB_MODE-1)
  {
    Mode=0;
  }
  display.showNumberDec(Mode, true);
  }
  Mode_Write=millis()+3000;
  ToWrite=1;
}

void Police()
{
  uint32_t m_color[6] = { 0x0000FF, 0xFF0000,0x0000FF, 0xFF0000,0x0000FF, 0xFF0000};
  int Couleur=random(5);
  Array_Letter[random(7)].Set_Letter_Color(m_color[Couleur]);
}

void setup()
{
  Serial.begin(115200); //Init serial monitor
  delay(200);
  Serial.println("Starting....");
  EEPROM.begin(EEPROM_SIZE);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), Update_Mode, RISING);
  display.setBrightness(0x0f);
  display.showNumberDec(Mode, true);
  strip.begin();
  strip.setBrightness(30);  
  /*Init_Letters(); //Init letters*/
  Serial.println("[*] Read EEPROM");
  Mode= EEPROM.read(adr_Mode);
  Serial.println(Mode);
  display.showNumberDec(Mode, true);
}

void loop() {
  if (Mode_Write>millis()&&ToWrite==1)
  {
    EEPROM.write(adr_Mode, Mode);
    EEPROM.commit();
    Serial.println("[*] Mode saved");
    ToWrite=0;
  }
switch (Mode) {
  case 0:
    Normal_Mode();
    break;
  case 1:
    Fire_Mode();
    break;
  case 2:
    Array_Letter[random(7)].Eclair();
    break;
  case 3:
    Rainbow();
    break;
   case 4:
    RGBW();
   break;
   case 5:
    Police();
   break;
}
}
