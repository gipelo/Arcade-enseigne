#include "Arduino.h"
#include "Letter.h"
#include <Freenove_WS2812_Lib_for_ESP32.h>

extern Freenove_ESP32_WS2812 strip;


  Letter::Letter(String Name,int Nb_Pixels, uint32_t Default_Color)
  {
    this->Name=Name;
    this->Nb_Pixels=Nb_Pixels;
    this->Default_Color=Default_Color;
    
  }
 
  void Letter::Set_PixelsPosition(int First, int Last)
  {
    Serial.print("Letter creation "+this->Name+" Start:");
    this->Pixel_First=First;
    Serial.print(First);
    Serial.print(" End:");
    this->Pixel_Last=Last;
    Serial.println(Last);
  }
  /*************Set Letter color***************************/
  void Letter::Set_Letter_Color(uint32_t Couleur)
  {
    this->Previous_Color=Get_Letter_Color();
    this->Current_Color=Couleur;
    for(int i=this->Pixel_First; i<this->Pixel_Last;i++)
    {
      strip.setLedColorData(i, Couleur);
    }
    Serial.println("[*] Letter " +this->Name+" Set Color to"+Couleur);
    strip.show();
  }

  /****************Return current color set****************/
  uint32_t Letter::Get_Letter_Color()
  {
    return this->Current_Color;
  }
  
  uint32_t Letter::Get_Previous_Letter_Color()
  {
    return this->Previous_Color;
  }
  
  int Letter::Get_Nb_Pixels()
  {
    return this->Nb_Pixels;
  }

  uint32_t Letter::Get_Default_Color()
  {
    return this->Default_Color;
  }
  
  void Letter::Off()
  {
    this->Set_Letter_Color(0x000000);
  }

  void Letter::Neon_Blink()
  {
    for (int i=0;i<random(30);i++)
    {
      this->Set_Letter_Color(0x000000);
      delay(random(100)); 
      this->Set_Letter_Color(this->Get_Previous_Letter_Color());
      delay(random(100));
    }
  }

  void Letter::Scintillement()
  {
    Serial.println("[*] Letter " +this->Name+" Scintillement");
    int LePixel=random(this->Pixel_Last-this->Pixel_First);
    strip.setLedColorData(this->Pixel_First+LePixel, 0xFFFFFF);
    strip.show();
    delay(50);
    this->Set_Letter_Color(this->Get_Letter_Color());
  }


    void Letter::Fire()
  {
    uint32_t Fire_color[]={0xFF2E00,0xFF4D00,0xFF6F00,0xFF9200,0xFFB100, 0xFFDC00,0x000000, 0x000000};
    Serial.println("[*] Letter " +this->Name+" Fire");
    int LePixel=random(this->Pixel_Last-this->Pixel_First);
    strip.setLedColorData(this->Pixel_First+LePixel, Fire_color[random(8)]);
    strip.show();
  }

    void Letter::Eclair()
  {
    this->Off();
    this->Scintillement();
  }

  
