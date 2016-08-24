#FUSES XT,NOWDT
#use delay(clock=4000000) // 4MHz
 
// LCD pins init.
#define LCD_ENABLE_PIN PIN_B6
#define LCD_RS_PIN PIN_B4
#define LCD_RW_PIN PIN_B5
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
 
#define trig PIN_C0
#define echo PIN_C1 
#include <lcd.c>
  
float distance,time;
 
void main()
{
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
  set_tris_b(0x00); // port B as output
  set_tris_c(0b00000010); // port C as output except pin 1
  set_tris_d(0x00); // port D as output
  output_b(0x00);  
  output_c(0x00); 
  output_d(0x00);
   
  output_high(PIN_C2); // something like starting whistle by buzzer for one sec
  delay_ms(1000);
  output_low(PIN_C2);
   
  lcd_init();
  lcd_gotoxy(1,1);
  lcd_putc("yasincetin.net");
  delay_ms(1500);
   
  while(1)
  {
    output_high(trig);
    delay_us(10); // 10us pulse
    output_low(trig);
     
    while(input(echo)!=1); // until the echo pin become high
    set_timer1(0); // set timer to 0 for fresh count
    while(input(echo)!=0); // continue to the counting until echo pin become low
    time=get_timer1();
    distance=time*0.017; // f = 4MHz/4 = 1MHz -> T = 1/1MHz = 1us. 
                         // speed of sound in air 340m/s = 34.000cm/1.000.000us = 0.034 cm/us
                         // distance = speed * time = 0.034 * time 
                         // actual distance = (0.034*time)/2 = 0.017*time (go to the target and return)
    if(distance<5) // make some noise if distance lower than 5cms. (a kind of parking sensor)
      output_high(PIN_C2);
    else
      output_low(PIN_C2);
     
    lcd_init();
    lcd_gotoxy(1,1);
    lcd_putc("\fDistance:");
    lcd_gotoxy(1,2);
    printf(LCD_PUTC,"%.2fcm",distance);
    delay_ms(500); // delay 0.5 sec for new measurement
  }
}
