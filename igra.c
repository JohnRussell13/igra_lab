#include <LiquidCrystal.h>
#include <util/delay.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
byte start_flag = 0;
char pos = 0;
char znaci[4] = {0,0,0,0};
char tacni_znaci[4] = {0,0,0,0};
int seed = 0;
byte  srce [8] = {
  B00000 ,
  B01010 ,
  B11111 ,
  B11111 ,
  B01110 ,
  B00100 ,
  B00000 ,
  B00000};
byte  karo [8] = {
  B00100 ,
  B01110 ,
  B11111 ,
  B01110 ,
  B00100 ,
  B00000 ,
  B00000 ,
  B00000};
byte  pik [8] = {
  B00100 ,
  B01110 ,
  B11111 ,
  B11111 ,
  B10101 ,
  B00100 ,
  B00000 ,
  B00000 ,};
byte  tref [8] = {
  B00100 ,
  B01010 ,
  B10101 ,
  B01010 ,
  B00100 ,
  B01110 ,
  B00000 ,
  B00000 ,};

char ocitaj_taster()
{
  char taster = 0;
  unsigned char row;
  
  MCUCR &= ~0x10;
  PORTD = 0x0f; 
  
  for(row = 0x80; row != 0x08; row >>= 1)
  {
    DDRD = row;
	_delay_ms(2);
    
    switch(row | ((~PIND) & 0x0f))
    {
      case 0x88:
      	taster = '1'; break;
      case 0x84:
      	taster = '2'; break;
      case 0x82:
      	taster = '3'; break;
      case 0x81:
      	taster = 'A'; break;
		
	  case 0x48:
      	taster = '4'; break;
      case 0x44:
      	taster = '5'; break;
      case 0x42:
      	taster = '6'; break;
      case 0x41:
      	taster = 'B'; break;
		
	  case 0x28:
      	taster = '7'; break;
      case 0x24:
      	taster = '8'; break;
      case 0x22:
      	taster = '9'; break;
      case 0x21:
      	taster = 'C'; break;
		
	  case 0x18:
      	taster = '*'; break;
      case 0x14:
      	taster = '0'; break;
      case 0x12:
      	taster = '#'; break;
      case 0x11:
      	taster = 'D'; break;
    }
  }
  
  return taster;
}

//stvara i printa tacne odgovore
void generator()
{
  randomSeed(seed);
  tacni_znaci[0] = random(0,3);
  tacni_znaci[1] = random(0,3);
  tacni_znaci[2] = random(0,3);
  tacni_znaci[3] = random(0,3);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(tacni_znaci[0]));
  lcd.write(byte(tacni_znaci[1]));
  lcd.write(byte(tacni_znaci[2]));
  lcd.write(byte(tacni_znaci[3]));
  _delay_ms(2000);
}

int main()
{
  	//pravi karaktere i pocetnu poruku
  	lcd.begin(16, 2);
  	lcd.createChar (0, srce);
  	lcd.createChar (1, karo);
  	lcd.createChar (2, pik);
  	lcd.createChar (3, tref);
    lcd.setCursor(0, 0);
  	lcd.print("Pritisni '*'");
  
	while(1)
    {
      //ocitava taster i inc seed za rand
      char taster = ocitaj_taster();
      seed++;
      
      //inicijalizuje igru
      if(taster == '*' && !start_flag)
      {
        generator();
        lcd.clear();
  		lcd.print("----");
        lcd.setCursor(pos, 0);
        _delay_ms(500);
        start_flag = 1;
        lcd.cursor();
        lcd.blink();
      }
      
      //pomera kursor levo
      else if(start_flag)
      {
        if(taster == '4' && pos > 0)
        {
          pos--;
          lcd.setCursor(pos, 0);
          _delay_ms(500);
        }
        
      //pomera kursor desno
        else if(taster == '6' && pos < 3)
        {
          pos++;
          lcd.setCursor(pos, 0);
          _delay_ms(500);
        }
          
        //pevecava znak
        else if(taster == '2')
        {
          znaci[pos] = (znaci[pos]+1)%4;
          lcd.write(byte (znaci[pos]));
          lcd.setCursor(pos, 0);
          _delay_ms(500);
        }
          
        //smanjuje znak
        else if(taster == '8')
        {
          znaci[pos] = (znaci[pos]+3)%4;
          lcd.write(byte (znaci[pos]));
          lcd.setCursor(pos, 0);
          _delay_ms(500);
        }
          
        //kraj igre
        else if(taster == '5')
        {
          lcd.setCursor(0, 1);
          lcd.write(byte(tacni_znaci[0]));
  		  lcd.write(byte(tacni_znaci[1]));
    	  lcd.write(byte(tacni_znaci[2]));
  		  lcd.write(byte(tacni_znaci[3]));
          
          if(tacni_znaci[0] == znaci[0] && tacni_znaci[1] == znaci[1] && tacni_znaci[2] == znaci[2] && tacni_znaci[3] == znaci[3])
          {
            lcd.setCursor(9, 0);
            lcd.print("E svaka"); //ipak nemam inspiracije...
            lcd.setCursor(9, 1);
            lcd.print("cast...");
          }
          
          else
          {
            lcd.setCursor(11, 0);
            lcd.print("Nidje");
            lcd.setCursor(12, 1);
            lcd.print("veze");
          }
          while(1);
        }
      }
    }
  
  	return 0;
}