#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define INFO_SCREEN_DELAY 3000

/*
  Linear Congruential Generator (LCG)
  z = (a*z + c) % m;  
  m = 256 (8 Bit)
  
  for period:
  a-1: dividable by 2
  a-1: multiple of 4
  c: not dividable by 2
  
  c = 17
  a-1 = 64 --> a = 65
*/
uint8_t z = 127;	// start value
uint32_t lcg_rnd(void) {
  z = (uint8_t)((uint16_t)65*(uint16_t)z + (uint16_t)17);
  return (uint32_t)z;
}

void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_unifont_t_japanese1);
}

void draw(int is_blank)
{
  int i, j;
  int n;
  char s[4];
  
  for( j = 0; j < 20; j++ )
  {
    // random number
    n = lcg_rnd();
    
    // random string
    for( i = 0; i < 3; i++ )
    {
      s[i] = lcg_rnd()>>3;
      if ( s[i] < 16 )
	s[i] += 'a';
      else
	s[i] += 'A';
    }
    s[3] = '\0';
    
    // print number
    u8g2.setCursor(0,15);
    u8g2.print("Number: ");
    if ( is_blank )
      u8g2.print("       ");
    u8g2.setCursor(70,15);
    u8g2.print(n);
    
    
    // print string
    u8g2.setCursor(0,30);
    u8g2.print("Text: ");
    u8g2.setCursor(70,30);
    u8g2.print(s);
    if ( is_blank )
      u8g2.print("        ");
    
    // make the result visible
    u8g2.sendBuffer();
    
    // delay, so that the user can see the result
    delay(200);
  }
}

void draw_m1_t()
{
  u8g2.clearBuffer();

  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		

  u8g2.setCursor(0,15);
  u8g2.print(F("setFontMode(1);"));
  u8g2.setCursor(0,30);
  u8g2.print(F("setFont(..._tr);"));
  u8g2.setCursor(0, 55);
  u8g2.print(F("Very Bad"));
    
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(1);		
  u8g2.setFont(u8g2_font_cu12_tr);		
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}

void draw_m0_t()
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		
  
  u8g2.setCursor(0,15);
  u8g2.print(F("setFontMode(0);"));
  u8g2.setCursor(0,30);
  u8g2.print(F("setFont(.._tr);"));
  u8g2.setCursor(0, 55);
  u8g2.print(F("Wrong"));
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_cu12_tr);		  
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}

void draw_m1_h()
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		
  
  u8g2.setCursor(0,15);
  u8g2.print(F("setFontMode(0);"));
  u8g2.setCursor(0,30);
  u8g2.print(F("setFont(.._hr);"));  
  u8g2.setCursor(0, 55);
  u8g2.print(F("Still bad"));
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_hr);		
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}

void draw_m0_h()
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		
  
  u8g2.setCursor(0,15);
  u8g2.print(F("setFontMode(0);"));
  u8g2.setCursor(0,30);
  u8g2.print(F("setFont(.._hr);"));  
  u8g2.setCursor(0, 55);
  u8g2.print(F("Almost ok"));
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_cu12_hr);		
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}

void draw_m0_h_with_extra_blank()
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		
  
  u8g2.setCursor(0,15);
  u8g2.print(F("setFontMode(0);"));
  u8g2.setCursor(0,30);
  u8g2.print(F("setFont(.._hr);"));  
  u8g2.setCursor(0, 55);
  u8g2.print(F("Extra blank --> Ok"));
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_cu12_hr);		
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}

void draw_japanese()
{
  u8g2.clearBuffer();
  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_unifont_t_japanese1);		
  
  u8g2.setCursor(0,15);
  u8g2.print(F("日本語表示"));
  u8g2.setCursor(0,30);
  u8g2.print(F("できました"));
  u8g2.setCursor(0, 55);
  u8g2.print(F("Good!"));
  
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_unifont_t_japanese1);		
  u8g2.clearBuffer();					// clear the internal memory once
  draw(0);
}


void loop(void) {

  // This problem applies only to full buffer mode
  u8g2.clearBuffer();  
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_cu12_tr);		  
  u8g2.setCursor(0,15);
  u8g2.print(F("Problems with"));
  u8g2.setCursor(0,30);
  u8g2.print(F("full buffer mode"));
  u8g2.setCursor(0,45);
  u8g2.print(F("and skipped clear."));
  u8g2.sendBuffer();
  delay(INFO_SCREEN_DELAY);
  

  draw_m1_t();			// fontmode 1, t font --> wrong
  draw_m1_h();			// fontmode 1, h font --> wrong
  draw_m0_t();			// fontmode 0, t font --> wrong
  draw_m0_h();			// fontmode 1, h font --> ok
  draw_m0_h_with_extra_blank();	// fontmode 1, h font with extra blank --> correct
  draw_japanese();		// fontmode 0, japanese font --> ok
  delay(1000);  
}

