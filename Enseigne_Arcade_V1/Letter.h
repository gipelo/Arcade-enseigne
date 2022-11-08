#ifndef Letter_h
#define Letter_h
#include "Arduino.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"


class Letter
{
  public:
    Letter(String Name,int Nb_Pixels, uint32_t Default_Color);
    void Set_PixelsPosition(int First, int Last);
    void Set_Letter_Color(uint32_t Color);
    int Get_Nb_Pixels();
    uint32_t Get_Letter_Color();
    uint32_t Get_Default_Color();
    uint32_t Get_Previous_Letter_Color();
    void Off();
    void Neon_Blink();
    void Scintillement();
    void Fire();
    void Eclair();
  
  private:
    int Pixel_First;
    int Pixel_Last;
    String Name;
    uint32_t Current_Color;
    uint32_t Previous_Color;
    uint32_t Default_Color;
    int Nb_Pixels;
};

#endif
